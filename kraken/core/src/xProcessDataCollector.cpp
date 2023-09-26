#include "xProcessDataCollector.h"

#include <Psapi.h>
#include <Winternl.h>

#include "xDynamicCoreApi.h"
#include "xLog.h"
#include "xPath.h"

#include "xDataCache.h"
#include "xFileDataCollector.h"
#include "xFileVersionDataCollector.h"
#include "xModuleDataCollector.h"
#include "xProcessData.h"
#include "xSignatureData.h"
#include "xSignatureDataCollector.h"



#define X_PROCESS_DATA_COLLECTOR_MAX_PROCESS		4096
#define X_PROCESS_DATA_COLLECTOR_MAX_MODULE			4096

#define X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_X64		L"x64"
#define X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_X86		L"x86"
#define X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_UNKNOWN	L"Unknown"

#define X_DATA_COLLECTOR_FILE_NAME_NTDLL_DLL		L"ntdll.dll"



xProcessDataCollector::xProcessDataCollector()
{

}

xProcessDataCollector::~xProcessDataCollector()
{

}

xResult xProcessDataCollector::Collect(IxProcessDataEnum** processDataEnum)
{
	X_ASSERT(processDataEnum);

	if (!processDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	DWORD pids[X_PROCESS_DATA_COLLECTOR_MAX_PROCESS] = { 0 };

	DWORD pidsNeeded = 0;

	if (!EnumProcesses(pids, sizeof(pids), &pidsNeeded))
	{
		return xResultFromLastError();
	}

	xInterfacePtr<xProcessDataEnum> pde(new xProcessDataEnum);

	if (!pde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	for (uint32_t ui = 0; ui < pidsNeeded / sizeof(uint32_t); ui++)
	{
		if (pids[ui])
		{
			xInterfacePtr<xProcessData> pd;

			if (xSucceeded(GetProcessData(pids[ui], &pd)))
			{
				pde->Add(pd);
			}
		}
	}

	*processDataEnum = pde.Detach();

	return r;
}

xResult xProcessDataCollector::Collect(const uint32_t processId, IxProcessData** processData)
{
	X_ASSERT(processId && processData);

	if (!processId || !processData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xProcessData> pd;

	xResult r = GetProcessData(processId, &pd);

	if (xSucceeded(r))
	{
		*processData = pd.Detach();
	}

	return r;
}

xResult xProcessDataCollector::GetProcessData(const uint32_t processId, xProcessData** processData)
{
	X_ASSERT(processId && processData);

	if (!processId || !processData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (process)
	{
		HMODULE mods[X_PROCESS_DATA_COLLECTOR_MAX_MODULE] = { NULL };

		DWORD modsNeeded = 0;

		if (EnumProcessModules(process, mods, sizeof(mods), &modsNeeded))
		{
			xInterfacePtr<xProcessData> pd;

			if (xSucceeded(r = GetProcessData(process, mods[0], &pd)))
			{
				xInterfacePtr<xModuleDataEnum> mde(new xModuleDataEnum);

				if (mde)
				{
					for (uint32_t ui = 1; ui < modsNeeded / sizeof(HMODULE); ui++)
					{
						xInterfacePtr<IxModuleData> md;

						if (xSucceeded(r = GetModuleData(process, mods[ui], &md)))
						{
							mde->Add(md);
						}
					}

					pd->SetModuleData(mde);

					*processData = pd.Detach();
				}
				else
				{
					r = X_E_OUT_OF_MEMORY;
				}
			}
			else
			{
				X_LOG_WARNING(L"Failed to get process data for process %d (code: " X_RESULT_FORMAT_STRING L")", processId, r);
			}
		}
		else
		{
			r = xResultFromLastError();

			X_LOG_WARNING(L"Failed to enumerate process modules for process %d (code: " X_RESULT_FORMAT_STRING L")", processId, r);
		}

		CloseHandle(process);
	}
	else
	{
		r = xResultFromLastError();

		X_LOG_WARNING(L"Failed to open process %d (code: " X_RESULT_FORMAT_STRING L")", processId, r);
	}

	return r;
}


xResult xProcessDataCollector::GetProcessData(HANDLE process, HMODULE module, xProcessData** processData)
{
	X_ASSERT(process && module && processData);

	if (!process || !module || !processData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString fileName;

	GetModuleFileNameEx(process, module, fileName.GetBuffer(MAX_PATH), MAX_PATH);

	fileName.ReleaseBuffer();

	xInterfacePtr<xProcessData> pd(new xProcessData);

	if (!pd)
	{
		return X_E_OUT_OF_MEMORY;
	}

	pd->SetId(GetProcessId(process));

	const xChar* subsystem = GetProcessSubsystem(process);

	if (subsystem)
	{
		pd->SetSubsystem(subsystem);
	}

	xFileDataCollector fdc;

	xInterfacePtr<IxFileData> fd;

	xResult r = fdc.Collect(fileName, &fd);

	if (xSucceeded(r))
	{
		pd->SetFileData(fd);
	}
	else
	{
		X_LOG_WARNING(L"Failed to get file information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
	}

	uint32_t parentProcessId = 0;

	xPtr<xChar> parentFileName;

	if (xSucceeded(r = GetParentProcessData(process, &parentProcessId, &parentFileName)))
	{
		pd->SetParentId(parentProcessId);

		pd->SetParent(parentFileName);
	}
	else
	{
		X_LOG_WARNING(L"Failed to get parent process information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
	}

	xFileVersionDataCollector fvdc;

	xInterfacePtr<IxFileVersionData> fvd;

	if (xSucceeded(r = fvdc.Collect(fileName, &fvd)))
	{
		pd->SetFileVersionData(fvd);
	}
	else
	{
		X_LOG_WARNING(L"Failed to get file version information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
	}

	xSignatureDataCollector sdc;

	xInterfacePtr<IxSignatureData> sd;

	if (xSucceeded(r = sdc.Collect(fileName, &sd)))
	{
		pd->SetSignatureData(sd);
	}
	else
	{
		X_LOG_WARNING(L"Failed to get file signature information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
	}

	*processData = pd.Detach();

	return r;
}

const xChar* xProcessDataCollector::GetProcessSubsystem(HANDLE process)
{
	X_ASSERT(process);

	if (!process)
	{
		return NULL;
	}

	BOOL wow64 = FALSE;

#ifdef _WIN64
	if (IsWow64Process(process, &wow64))
	{
		return wow64 ? X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_X86 : X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_X64;
	}
	else
	{
		return X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_UNKNOWN;
	}
#elif defined _WIN32
	return X_PROCESS_DATA_COLLECTOR_SUBSYSTEM_X86;
#endif
}

xResult xProcessDataCollector::GetParentProcessData(HANDLE child, uint32_t* parentProcessId, xChar** parentFileName)
{
	X_ASSERT(child && parentProcessId && parentFileName);

	if (!child || !parentProcessId || !parentFileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	const uint32_t pid = GetParentProcessId(child);

	xString fn;

	if (pid)
	{
		HANDLE parent = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);

		if (parent)
		{
			GetModuleFileNameEx(parent, NULL, fn.GetBuffer(X_PATH_MAX), X_PATH_MAX);

			fn.ReleaseBuffer();

			CloseHandle(parent);
		}
	}

	if (fn.IsEmpty())
	{
		fn = L"Unknown";
	}

	*parentProcessId = pid;

	return (*parentFileName = fn.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}

uint32_t xProcessDataCollector::GetParentProcessId(HANDLE child)
{
	X_ASSERT(child);

	if (!child)
	{
		return 0;
	}

	if (X_CORE_NTDLL_IS_AVAILABLE(NtQueryInformationProcess))
	{
		PROCESS_BASIC_INFORMATION pbi;

		xMemoryZero(&pbi, sizeof(pbi));

		ULONG ulSize = 0;

		if (X_CORE_NTDLL_CALL(NtQueryInformationProcess)(child, ProcessBasicInformation, &pbi, sizeof(pbi), &ulSize) >= 0 && ulSize == sizeof(pbi))
		{
			// "Reserved3" contains the parent process ID
			return (uint32_t) (uint64_t) pbi.Reserved3;
		}
	}

	return 0;
}

xResult xProcessDataCollector::GetModuleData(HANDLE process, HMODULE module, IxModuleData** moduleData)
{
	X_ASSERT(process && module && moduleData);

	if (!process || !module || !moduleData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString fileName;

	if (!GetModuleFileNameEx(process, module, fileName.GetBuffer(MAX_PATH), MAX_PATH))
	{
		return xResultFromLastError();
	}

	fileName.ReleaseBuffer();

	xModuleDataCollector mdc;

	xInterfacePtr<IxModuleData> md;

	xResult r = mdc.Collect(fileName, &md);

	if (xSucceeded(r))
	{
		*moduleData = md.Detach();
	}

	return r;
}
