#pragma once

#include "xPath.h"
#include "xStringEnum.h"



typedef  xResult (__X_STDCALL* pfnFileFindCallback)(const xChar* fileName, void* context);



class xFileFind
{
public:
	xFileFind()
	{
	}

	~xFileFind()
	{
	}

public:
	xResult Find(const xChar* pattern, const bool recursive, IxStringEnum** stringEnum)
	{
		X_ASSERT(pattern);

		if (!pattern)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xInterfacePtr<IxStringEnum> se(new xStringEnum);

		if (!se)
		{
			return X_E_OUT_OF_MEMORY;
		}

		xChar drive[X_DRIVE_MAX] = {X_CHAR('\0')};

		xChar dir[X_DIR_MAX] = {X_CHAR('\0')};
		
		xChar fname[X_FNAME_MAX] = {X_CHAR('\0')};

		xChar ext[X_EXT_MAX] = {X_CHAR('\0')};

		xPathSplit(pattern, drive, dir, fname, ext);

		xString baseFileName;

		baseFileName.Format(X_CHAR("%s%s"), drive, dir);

		WIN32_FIND_DATA w32fd;

		ZeroMemory(&w32fd, sizeof(w32fd));

		HANDLE findFile = FindFirstFile(pattern, &w32fd);

		if (findFile  != INVALID_HANDLE_VALUE)
		{
			do
			{	
				if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// Skip dots
					if (xCharCompareNoCase(w32fd.cFileName, X_PATH_SELF) == 0 || xCharCompareNoCase(w32fd.cFileName, X_PATH_PARENT) == 0)
					{
						continue;
					}

					// Recursion
					if (recursive)
					{
						xString s;

						s.Format(X_CHAR("%s%s\\%s%s"), baseFileName, w32fd.cFileName, fname, ext);
						
						xInterfacePtr<IxStringEnum> temp;

						if (xSucceeded(Find(s, recursive, &temp)))
						{
							se->Add(temp);
						}
					}
				}
				else
				{
					xString s;

					s.Format(X_CHAR("%s%s"), baseFileName, w32fd.cFileName);

					se->Add(s);
				}
			}
			while (FindNextFile(findFile, &w32fd));

			FindClose(findFile);
		}
		else
		{
			return xResultFromLastError();
		}

		*stringEnum = se.Detach();

		return X_S_OK;
	}

	xResult Find(const xChar* pattern, const bool recursive, pfnFileFindCallback callback, void* context)
	{
		X_ASSERT(pattern && callback);

		if (!pattern || !callback)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xChar drive[X_DRIVE_MAX] = {X_CHAR('\0')};

		xChar dir[X_DIR_MAX] = {X_CHAR('\0')};

		xChar fname[X_FNAME_MAX] = {X_CHAR('\0')};

		xChar ext[X_EXT_MAX] = {X_CHAR('\0')};

		xPathSplit(pattern, drive, dir, fname, ext);

		xString baseFileName;

		baseFileName.Format(X_CHAR("%s%s"), drive, dir);

		WIN32_FIND_DATA w32fd;

		ZeroMemory(&w32fd, sizeof(w32fd));

		HANDLE findFile = FindFirstFile(pattern, &w32fd);

		if (findFile  != INVALID_HANDLE_VALUE)
		{
			do
			{	
				if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					// Skip dots
					if (xCharCompareNoCase(w32fd.cFileName, X_PATH_SELF) == 0 || xCharCompareNoCase(w32fd.cFileName, X_PATH_PARENT) == 0)
					{
						continue;
					}

					// Recursion
					if (recursive)
					{
						xString s;

						s.Format(X_CHAR("%s%s\\%s%s"), baseFileName, w32fd.cFileName, fname, ext);

						Find(s, recursive, callback, context);
					}
				}
				else
				{
					xString s;

					s.Format(X_CHAR("%s%s"), baseFileName, w32fd.cFileName);

					callback(s, context);
				}
			}
			while (FindNextFile(findFile, &w32fd));

			FindClose(findFile);
		}
		else
		{
			return xResultFromLastError();
		}

		return X_S_OK;
	}
};