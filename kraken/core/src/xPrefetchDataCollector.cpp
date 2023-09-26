#include "xPrefetchDataCollector.h"

#include <Shlobj.h>
#include <winternl.h>

#include "xConversion.h"
#include "xFile.h"
#include "xFileFind.h"
#include "xHandle.h"
#include "xLog.h"
#include "xPath.h"
#include "xTypeEnum.h"

#include "xDataEnum.h"
#include "xDynamicCoreApi.h"
#include "xFileData.h"
#include "xFileDataCollector.h"
#include "xModuleData.h"
#include "xModuleDataCollector.h"
#include "xPrefetchData.h"

#include "libscca.h"



xPrefetchDataCollector::xPrefetchDataCollector()
{

}

xPrefetchDataCollector::~xPrefetchDataCollector()
{

}

xResult xPrefetchDataCollector::Collect(IxPrefetchDataEnum** prefetchDataEnum)
{
	X_ASSERT(prefetchDataEnum);

	if (!prefetchDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xChar* wstr = NULL;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Windows, 0, NULL, &wstr);

	if (FAILED(hr))
	{
		return xResultFromHresult(hr);
	}

	xPtr<xChar> s;

	xResult r = xPathAppend(wstr, X_CHAR("Prefetch\\*.pf"), &s);

	CoTaskMemFree(wstr);

	xInterfacePtr<xPrefetchDataEnum> pde(new xPrefetchDataEnum);

	if (!pde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if (xSucceeded(r))
	{
		xFileFind ff;

		xInterfacePtr<IxStringEnum> se;

		if (xSucceeded(r = ff.Find(s, true, &se)))
		{
			const uint32_t count = se->GetCount();

			for (uint32_t ui = 0; ui < count; ui++)
			{
				xPtr<xChar> fileName;

				if (xSucceeded(r = se->Get(ui, &fileName)))
				{
					xInterfacePtr<IxPrefetchData> pd;

					if (xSucceeded(r = GetPrefetchData(fileName, pde, &pd)))
					{
						pde->Add(pd);
					}
					else
					{
						X_LOG_WARNING(L"Failed to get prefetch data for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
					}
				}
			}
		}
	}

	*prefetchDataEnum = pde.Detach();

	return r;
}

xResult xPrefetchDataCollector::GetPrefetchData(const xChar* fileName, xPrefetchDataEnum* prefetchDataEnum, IxPrefetchData** prefetchData)
{
	X_ASSERT(fileName && prefetchDataEnum && prefetchData);

	if (!fileName || !prefetchDataEnum || !prefetchData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xPrefetchData> pd(new xPrefetchData);

	if (!pd)
	{
		return X_E_OUT_OF_MEMORY;
	}

	pd->SetFileName(fileName);

	X_LOG_DEBUG(L"Prefetch file name: %s", fileName);

	libscca_file_t* file = NULL;

	if (libscca_file_initialize(&file, NULL) == 1)
	{
		if (libscca_file_open_wide(file, fileName, FILE_GENERIC_READ, NULL) == 1)
		{
			r = GetPrefetchData(file, fileName, prefetchDataEnum, pd);

			libscca_file_close(file, NULL);
		}

		libscca_file_free(&file, NULL);
	}

	*prefetchData = pd.Detach();

	return r;
}

xResult xPrefetchDataCollector::GetPrefetchData(libscca_file_t* file, const xChar* fileName, xPrefetchDataEnum* prefetchDataEnum, xPrefetchData* prefetchData)
{
	X_ASSERT(file && fileName && prefetchDataEnum && prefetchData);

	if (!file || !fileName || !prefetchDataEnum || !prefetchData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	size_t executableFileNameSize = 0;

	if (libscca_file_get_utf16_executable_filename_size(file, &executableFileNameSize, NULL) == 1)
	{
		xPtr<xChar> executableFileName((xChar*)xMemoryAlloc((uint32_t)(executableFileNameSize * sizeof(xChar))));

		if (executableFileName)
		{
			if (libscca_file_get_utf16_executable_filename(file, (uint16_t*) (xChar*) executableFileName, executableFileNameSize, NULL) == 1)
			{
				prefetchData->SetExecutableFileName(executableFileName);

				X_LOG_DEBUG(L"Executable file name: %s", executableFileName);
			}
			else
			{
				X_LOG_WARNING(L"Failed to get executable file name for prefetch file: %s", fileName);
			}
		}
	}

	uint32_t prefetchHash = 0;

	if (libscca_file_get_prefetch_hash(file, &prefetchHash, NULL) == 1)
	{
		xPtr<xChar> ph;

		ph.Attach(xConvertBinaryToHex((uint8_t*) &prefetchHash, sizeof(prefetchHash)));

		if (ph)
		{
			prefetchData->SetHash(ph);

			X_LOG_DEBUG(L"Prefetch hash: %s", ph);
		}
	}
	else
	{
		X_LOG_WARNING(L"Failed to get prefetch hash for prefetch file: %s", fileName);
	}

	uint32_t runCount = 0;

	if (libscca_file_get_run_count(file, &runCount, NULL) == 1)
	{
		xInterfacePtr<xTimeEnum> lastRunTimes(new xTimeEnum);

		if (lastRunTimes)
		{
			for (uint32_t ui = 0; ui < runCount; ui++)
			{
				FILETIME lastRunTime = { 0 };

				if (libscca_file_get_last_run_time(file, ui, (uint64_t*) &lastRunTime, NULL) == 1)
				{
					X_LOG_DEBUG(L"Last run time: %ull", lastRunTime);

					lastRunTimes->Add(xConvertFileTimeToTime(lastRunTime));
				}
				else
				{
					X_LOG_WARNING(L"Failed to get last run time (index: %d) for prefetch file: %s", ui, fileName);
				}
			}
		}

		prefetchData->SetLastRunTimes(lastRunTimes);
	}

	int	fileNameCount = 0;

	if (libscca_file_get_number_of_filenames(file, &fileNameCount, NULL) == 1)
	{
		xInterfacePtr<xPrefetchItemDataEnum> pide(new xPrefetchItemDataEnum);

		if (pide)
		{
			for (int i = 0; i < fileNameCount; i++)
			{
				size_t nativeFileNameSize = 0;

				if (libscca_file_get_utf16_filename_size(file, i, &nativeFileNameSize, NULL) == 1)
				{
					xPtr<xChar> nativeFileName((xChar*) xMemoryAlloc((uint32_t) (nativeFileNameSize * sizeof(xChar))));

					if (nativeFileName)
					{	
						if (libscca_file_get_utf16_filename(file, i, (uint16_t*) (xChar*) nativeFileName, nativeFileNameSize, NULL) == 1)
						{
							xInterfacePtr<IxPrefetchItemData> pid;

							if (pide->Contains(nativeFileName, &pid) ||
								prefetchDataEnum->Contains(nativeFileName, &pid))
							{
								pide->Add(pid);
							}
							else
							{
								pid = new xPrefetchItemData;

								if (pid)
								{
									reinterpret_cast<xPrefetchItemData*>(pid.GetSafeType())->SetNativeFileName(nativeFileName);

									X_LOG_DEBUG(L"Native file name: %s", nativeFileName);

									xPtr<xChar> win32FileName;

									if (xSucceeded(r = GetWin32FileNameFromNtFileName(nativeFileName, &win32FileName)) ||
										xSucceeded(r = GetWin32FileNameFromPrefetchGuidFileName(nativeFileName, file, &win32FileName)))
									{
										X_LOG_DEBUG(L"Win32 file name: %s", win32FileName);

										if (xFileIsModule(win32FileName))
										{
											X_LOG_DEBUG(L"Win32 file name: %s is a module", win32FileName);

											xModuleDataCollector mdc;

											xInterfacePtr<IxModuleData> md;

											if (xSucceeded(r = mdc.Collect(win32FileName, &md)))
											{
												reinterpret_cast<xPrefetchItemData*>(pid.GetSafeType())->SetItem(md);
											}
											else
											{
												X_LOG_WARNING(L"Failed to get module data for win32 file name: %s", win32FileName);
											}
										}
										else
										{
											X_LOG_DEBUG(L"Win32 file name: %s is a file", win32FileName);

											xFileDataCollector fdc;

											xInterfacePtr<IxFileData> fd;

											if (xSucceeded(r = fdc.Collect(win32FileName, &fd)))
											{
												reinterpret_cast<xPrefetchItemData*>(pid.GetSafeType())->SetItem(fd);
											}
											else
											{
												X_LOG_WARNING(L"Failed to get file data for win32 file name: %s", win32FileName);
											}
										}
									}
									else
									{
										X_LOG_WARNING(L"Failed to get win32 file name for native file name: %s", nativeFileName);
									}

									pide->Add(pid);
								}
							}
						}
						else
						{
							X_LOG_WARNING(L"Failed to get file name (index: %d) for prefetch file: %s", i, fileName);
						}
					}
				}
			}

			prefetchData->SetItemData(pide);
		}
	}

	return X_S_OK;
}

xResult xPrefetchDataCollector::GetWin32FileNameFromNtFileName(const xChar* ntFileName, xChar** win32FileName)
{
	X_ASSERT(ntFileName && win32FileName);

	if (!ntFileName || !win32FileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	if (X_CORE_NTDLL_IS_AVAILABLE(NtCreateFile))
	{
		UNICODE_STRING us;

		RtlInitUnicodeString(&us, ntFileName);

		OBJECT_ATTRIBUTES oa;

		InitializeObjectAttributes(&oa, &us, OBJ_CASE_INSENSITIVE, NULL, NULL);

		IO_STATUS_BLOCK iosb;

		xFileHandle file;

		NTSTATUS status = X_CORE_NTDLL_CALL(NtCreateFile)(&file, FILE_GENERIC_READ, &oa, &iosb, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN, FILE_SEQUENTIAL_ONLY, NULL, 0);

		xResult r = X_S_OK;

		xString kernelFileName;

		if (NT_SUCCESS(status))
		{
			if (!GetFinalPathNameByHandle(file, kernelFileName.GetBuffer(X_PATH_MAX), X_PATH_MAX, VOLUME_NAME_DOS))
			{
				r = xResultFromLastError();
			}
		}
		else
		{
			r = X_E_FAIL;
		}

		return xSucceeded(r) ? xPathGetWin32FileNameFromKernelFileName(kernelFileName, win32FileName) : r;
	}
	else
	{
		X_LOG_ERROR(X_CHAR("Failed to get \"NtCreateFile \" from \"ntdll.dll\""));

		return X_E_NO_INTERFACE;
	}
}

xResult xPrefetchDataCollector::GetWin32FileNameFromPrefetchGuidFileName(const xChar* guidFileName, libscca_file_t* file, xChar** win32FileName)
{
	X_ASSERT(guidFileName && file && win32FileName);

	if (!guidFileName || !file || !win32FileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_E_FAIL;

	xString s(guidFileName);

	bool found = false;

	int	volumeCount = 0;

	if (libscca_file_get_number_of_volumes(file, &volumeCount, NULL) == 1)
	{
		for (int i = 0; i < volumeCount && !found; i++)
		{
			libscca_volume_information_t* volumeInfo = NULL;

			if (libscca_file_get_volume_information(file, i, &volumeInfo, NULL) == 1)
			{
				size_t devicePathSize = 0;

				if (libscca_volume_information_get_utf16_device_path_size(volumeInfo, &devicePathSize, NULL) == 1)
				{
					xPtr<xChar> devicePath((xChar*) xMemoryAlloc((uint32_t) (devicePathSize * sizeof(xChar))));

					if (devicePath)
					{
						if (libscca_volume_information_get_utf16_device_path(volumeInfo, (uint16_t*) (xChar*) devicePath, devicePathSize, NULL) == 1)
						{
							if (s.Find(devicePath) == 0)
							{
								uint32_t volumeSerial = 0;

								if (libscca_volume_information_get_serial_number(volumeInfo, &volumeSerial, NULL) == 1)
								{
									xPtr<xChar> driveLetter;

									if (xSucceeded(r = GetDriveLetterForVolume(volumeSerial, &driveLetter)))
									{
										s.Replace(devicePath, driveLetter);

										*win32FileName = s.Clone();

										found = true;;
									}
								}
							}
						}
					}
				}

				libscca_volume_information_free(&volumeInfo, NULL);
			}
		}
	}

	return r;
}

xResult xPrefetchDataCollector::GetDriveLetterForVolume(const uint32_t serial, xChar** driveLetter)
{
	X_ASSERT(serial && driveLetter);

	if (!serial || !driveLetter)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString dl(X_CHAR("0:\\"));

	for (xChar c = X_CHAR('C'); c < X_CHAR('Z'); c++)
	{
		dl.SetAt(0, c);

		DWORD tempSerial = 0;

		if (GetVolumeInformation(dl, NULL, 0, &tempSerial, NULL, NULL, NULL, 0))
		{
			if (tempSerial == serial)
			{
				dl.TrimRight(L'\\');

				*driveLetter = dl.Clone();

				return X_S_OK;
			}
		}
	}

	return X_E_FAIL;
}
