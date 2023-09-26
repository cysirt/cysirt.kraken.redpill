#pragma once

#include <wchar.h>

#include "xAssert.h"
#include "xDefinition.h"
#include "xResult.h"
#include "xString.h"



#define X_PATH_SEPARATOR_UNC	X_CHAR('/')
#define X_PATH_SEPARATOR		X_CHAR('\\')

#define X_PATH_MAX				_MAX_PATH
#define X_DRIVE_MAX				_MAX_DRIVE
#define X_DIR_MAX				_MAX_DIR
#define X_FNAME_MAX				_MAX_FNAME
#define X_EXT_MAX				_MAX_EXT

#define X_PATH_SELF				X_CHAR(".")
#define X_PATH_PARENT			X_CHAR("..")


#define X_PATH_NATIVE_FILE_NAME_1			X_CHAR("\\\\?\\")
#define X_PATH_NATIVE_FILE_NAME_2			X_CHAR("\\??\\")
#define X_PATH_NATIVE_FILE_NAME_3			X_CHAR("\\")

#define X_PATH_NATIVE_FILE_NAME_GLOBAL_ROOT	X_CHAR("GlobalRoot\\")
#define X_PATH_NATIVE_FILE_NAME_SYSTEM_ROOT	X_CHAR("SystemRoot")
#define X_PATH_NATIVE_FILE_NAME_WINDOWS		X_CHAR("Windows")



static const xChar* s_kernelFileName[] =
{
	X_PATH_NATIVE_FILE_NAME_1,
	X_PATH_NATIVE_FILE_NAME_2,
	X_PATH_NATIVE_FILE_NAME_3,
	X_PATH_NATIVE_FILE_NAME_GLOBAL_ROOT
};



#define xPathSplitA _splitpath
#define xPathSplitW _wsplitpath

#ifdef UNICODE
	#define xPathSplit xPathSplitW
#else // UNICODE
	#define xPathSplit xPathSplitA
#endif 



static bool xPathIsUnc(const xChar* path)
{
	X_ASSERT(path);

	if (!path)
	{
		return false;
	}

	return !!wcschr(path, X_PATH_SEPARATOR_UNC);
}



static xResult xPathAppend(const xChar* s, const xChar* append, xChar** result, const bool* unc = NULL)
{
	X_ASSERT(s && append && result);

	if (!s || !append || !result)
	{
		return X_E_INVALID_ARGUMENT;
	}

	const bool isUnc = unc ? *unc : xPathIsUnc(s);

	xString temp(s);

	if (xCharLength(append))
	{
		if (temp.ReverseFind(isUnc ? X_PATH_SEPARATOR_UNC : X_PATH_SEPARATOR) != (temp.GetLength() - 1))
		{
			temp += isUnc ? X_PATH_SEPARATOR_UNC : X_PATH_SEPARATOR;
		}

		temp += append;
	}

	return (*result = temp.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathRemoveLast(const xChar* s, xChar** result, const bool* unc = NULL)
{
	X_ASSERT(s && result);
	if (!s || !result)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString r;

	xString temp(s);
	
	const int32_t pos = temp.ReverseFind((unc ? *unc : xPathIsUnc(s)) ? X_PATH_SEPARATOR_UNC : X_PATH_SEPARATOR);

	if (pos != -1)
	{
		r = temp.Left(pos);
	}

	return (*result = r.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathGetLast(const xChar* s, xChar** result, const bool* unc = NULL)
{
	X_ASSERT(s && result);
	
	if (!s || !result)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString r;

	xString temp(s);

	const int32_t pos = temp.ReverseFind((unc ? *unc : xPathIsUnc(s)) ? X_PATH_SEPARATOR_UNC : X_PATH_SEPARATOR);

	if (pos != -1)
	{
		r = temp.Right(temp.GetLength() - pos - 1);
	}
	else
	{
		r = s;
	}

	return (*result = r.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathRemoveExtension(const xChar* s, xChar** result)
{
	X_ASSERT(s && result);

	if (!s || !result)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString temp(s);

	const int32_t pos = temp.ReverseFind(L'.');

	if (pos != -1)
	{
		temp.SetAt(pos, X_CHAR('\0'));
	}

	return (*result = temp.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathGetCurrentDirectory(xChar** result)
{
	X_ASSERT(result);

	if (!result)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString s;

	GetCurrentDirectory(X_PATH_MAX, s.GetBuffer(X_PATH_MAX));

	return (*result = s.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathGetTempFileName(const xChar* prefix, xChar** tempFileName)
{
	xString temp;

	xString path;

	if (!GetTempPath(X_PATH_MAX, path.GetBuffer(X_PATH_MAX)))
	{
		return xResultFromLastError();
	}

	if (!GetTempFileName(path, prefix ? prefix : X_CHAR_EMPTY, 0, temp.GetBuffer(X_PATH_MAX)))
	{
		return xResultFromLastError();
	}
	
	return (*tempFileName = temp.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}



static xResult xPathGetWin32FileNameFromKernelFileName(const xChar* kernelFileName, xChar** win32FileName)
{
	X_ASSERT(kernelFileName && win32FileName);

	if (!kernelFileName || !win32FileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString temp(kernelFileName);

	const uint32_t count = sizeof(s_kernelFileName) / sizeof(xChar*);

	for (uint32_t ui = 0; ui < count; ui++)
	{
		const uint32_t length = (uint32_t) wcslen(s_kernelFileName[ui]);

		if (temp.Left(length).CompareNoCase(s_kernelFileName[ui]) == 0)
		{
			temp = temp.Right(temp.GetLength() - length);
		}
	}

	const xChar* replace = NULL;

	if (temp.Left((uint32_t) xCharLength(X_PATH_NATIVE_FILE_NAME_SYSTEM_ROOT)).CompareNoCase(X_PATH_NATIVE_FILE_NAME_SYSTEM_ROOT) == 0)
	{
		replace = X_PATH_NATIVE_FILE_NAME_SYSTEM_ROOT;
	}
	else if (temp.Left((uint32_t) xCharLength(X_PATH_NATIVE_FILE_NAME_WINDOWS)).CompareNoCase(X_PATH_NATIVE_FILE_NAME_WINDOWS) == 0)
	{
		replace = X_PATH_NATIVE_FILE_NAME_WINDOWS;
	}

	if (replace)
	{
		xString dir;

		if (GetWindowsDirectory(dir.GetBuffer(X_PATH_MAX), X_PATH_MAX))
		{
			const uint32_t length = (uint32_t) xCharLength(replace);

			temp = temp.Right(temp.GetLength() - length);

			temp.Insert(0, dir);
		}
	}

	return (*win32FileName = temp.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}
