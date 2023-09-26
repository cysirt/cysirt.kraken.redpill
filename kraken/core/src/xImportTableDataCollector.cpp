#include "xImportTableDataCollector.h"

#include "xConversion.h"
#include "xLog.h"
#include "xStringEnum.h"

#include "xDataEnum.h"
#include "xImportTableData.h"



xImportTableDataCollector::xImportTableDataCollector()
{

}

xImportTableDataCollector::~xImportTableDataCollector()
{

}

xResult xImportTableDataCollector::Collect(const xChar* fileName, IxImportTableDataEnum** importTableDataEnum)
{
	X_ASSERT(fileName && importTableDataEnum);

	if (!fileName || !importTableDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xImportTableDataEnum> itde(new xImportTableDataEnum);

	if (!itde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xResult r = X_S_OK;

	HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (file != INVALID_HANDLE_VALUE)
	{
		HANDLE fileMap = CreateFileMapping(file, 0, PAGE_READONLY, 0, 0, 0);

		if (fileMap)
		{
			void* fileMemory = MapViewOfFile(fileMap, FILE_MAP_READ, 0, 0, 0);

			if (fileMemory)
			{
				PIMAGE_DOS_HEADER idh = (PIMAGE_DOS_HEADER) fileMemory;

				PIMAGE_NT_HEADERS inth = (PIMAGE_NT_HEADERS) ((uintptr_t) fileMemory + idh->e_lfanew);

				DWORD sectionCount = inth->FileHeader.NumberOfSections;

				// Import table is at position "1" in the array
				DWORD va = inth->OptionalHeader.DataDirectory[1].VirtualAddress;

				if (va)
				{
					PIMAGE_SECTION_HEADER ish = (PIMAGE_SECTION_HEADER) ((uintptr_t) inth + sizeof(IMAGE_NT_HEADERS));

					DWORD section = 0;

					for(; section < sectionCount && ish->VirtualAddress <= va; ish++, section++);

					ish--;

					uintptr_t rawOffset = (uintptr_t) fileMemory + ish->PointerToRawData;

					PIMAGE_IMPORT_DESCRIPTOR iid = (PIMAGE_IMPORT_DESCRIPTOR) (rawOffset + (va-ish->VirtualAddress));

					for(; iid->Name != 0; iid++)
					{
						xInterfacePtr<xImportTableData> importTableData(new xImportTableData);

						if (!importTableData)
						{
							r = X_E_OUT_OF_MEMORY;

							break;
						}

						importTableData->SetModuleName(xPtr<xChar>(xConvertUtf8ToUtf16((const char*) (rawOffset + (iid->Name-ish->VirtualAddress)))));

						PIMAGE_THUNK_DATA itd = (PIMAGE_THUNK_DATA) (rawOffset + ((iid->OriginalFirstThunk ? iid->OriginalFirstThunk : iid->FirstThunk) - ish->VirtualAddress));

						xInterfacePtr<IxStringEnum> functions(new xStringEnum);

						for (; itd->u1.AddressOfData != 0; itd++)
						{
							if (!(itd->u1.AddressOfData & IMAGE_ORDINAL_FLAG))
							{
								functions->Add(xPtr<xChar>(xConvertUtf8ToUtf16((const char*) (rawOffset + (itd->u1.AddressOfData - ish->VirtualAddress + 2)))));
							}
							
							if (itd->u1.Ordinal & IMAGE_ORDINAL_FLAG)
							{
								xString s;

								s.Format(X_CHAR("0x%08x"), itd->u1.Ordinal);

								functions->Add(s);
							}
						}

						importTableData->SetFunctions(functions);

						itde->Add(importTableData);
					}
				}
				else
				{
					X_LOG_WARNING(X_CHAR("Import table of \"%s\" is empty"), fileName);

					r = X_E_NO_DATA;
				}

				UnmapViewOfFile(fileMemory);
			}
			else
			{
				r = xResultFromLastError();
			}

			CloseHandle(fileMap);
		}
		else
		{
			r = xResultFromLastError();
		}

		CloseHandle(file);
	}
	else
	{
		r = xResultFromLastError();
	}

	if (xSucceeded(r))
	{
		*importTableDataEnum = itde.Detach();
	}

	return r;
}