#pragma once

#include <stdio.h>
#include <sys/stat.h>

#include "xAssert.h"
#include "xChar.h"



static FILE* xFileOpen(const xChar* fileName, const xChar* mode)
{
	X_ASSERT(fileName && mode);

	if (!fileName || !mode)
	{
		return NULL;
	}

	return _wfopen(fileName, mode);
}

static uint64_t xFileGetSize(FILE* file)
{
	X_ASSERT(file);

	if (!file)
	{
		return 0;
	}

	int f = _fileno(file);

	struct _stat64 stat;

	return _fstat64(f, &stat) == 0 ? (uint64_t) stat.st_size : 0;
}

static size_t xFileWrite(const void* buffer, const size_t size, FILE* file)
{
	X_ASSERT(buffer && size && file);

	if (!buffer || !size || !file)
	{
		return 0;
	}

	return fwrite(buffer, size, 1, file);
}

static size_t xFileRead(void* buffer, const size_t size, FILE* file)
{
	X_ASSERT(buffer && size && file);

	if (!buffer || !size || !file)
	{
		return 0;
	}

	return fread(buffer, 1, size, file);
}

static int xFileClose(FILE* file)
{
	X_ASSERT(file);

	if (!file)
	{
		return 0;
	}

	return fclose(file);
}

static int xFileDelete(const xChar* fileName)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return 0;
	}

	return _wremove(fileName);
}

static bool xFileIsModule(const xChar* fileName)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return false;
	}

	bool result = false;

	HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (file != INVALID_HANDLE_VALUE)
	{
		HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);

		if (fileMapping)
		{
			void* fileBase = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);

			if (fileBase)
			{
				PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) fileBase;

				if (dosHeader->e_magic == 0x5a4d)
				{
					PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS) ((uintptr_t) dosHeader + (uintptr_t) dosHeader->e_lfanew);

					if ((ntHeader->FileHeader.Characteristics & IMAGE_FILE_DLL) ||
						(ntHeader->FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE))
					{
						result = true;
					}
				}

				UnmapViewOfFile(fileBase);
			}

			CloseHandle(fileMapping);
		}

		CloseHandle(file);
	}

	return result;
}
