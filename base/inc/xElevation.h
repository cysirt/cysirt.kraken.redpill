#pragma once

#include <Windows.h>
#include <shellapi.h>

#include "xChar.h"
#include "xResult.h"



class xElevation
{
private:
	xElevation()
	{
	}

public:
	~xElevation()
	{
	}

	static xElevation& Get()
	{
		static xElevation s_self;

		return s_self;
	}

	bool IsElevated()
	{
		bool isElevated = false;

		HANDLE token = NULL;

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
		{
			TOKEN_ELEVATION te;

			DWORD size;

			if (GetTokenInformation(token, TokenElevation, &te, sizeof(te), &size))
			{
				isElevated = !!te.TokenIsElevated;
			}

			CloseHandle(token);
		}

		return isElevated;
	}

	xResult EnableDebugPrivilege(const bool enable)
	{
		HANDLE token = NULL;

		if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
		{
			if (GetLastError() == ERROR_NO_TOKEN)
			{
				if (!ImpersonateSelf(SecurityImpersonation))
				{
					return xResultFromLastError();
				}

				if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &token))
				{
					return xResultFromLastError();
				}
			}
			else
			{
				return xResultFromLastError();
			}
		}

		xResult r = EnablePrivilege(token, SE_DEBUG_NAME, enable);

		CloseHandle(token);

		return r;
	}

private:
	xResult EnablePrivilege(HANDLE token, const xChar* privilege, const bool enable)
	{
		X_ASSERT(token && privilege);

		if (!token || !privilege)
		{
			return X_E_INVALID_ARGUMENT;
		}

		LUID luid;

		if (!LookupPrivilegeValue(NULL, privilege, &luid))
		{
			return xResultFromLastError();
		}

		// First pass, get current privilege setting
		TOKEN_PRIVILEGES tp = { 0 };

		tp.PrivilegeCount = 1;

		tp.Privileges[0].Luid = luid;

		tp.Privileges[0].Attributes = 0;

		TOKEN_PRIVILEGES tpPrevious = { 0 };

		DWORD cbPrevious = sizeof(TOKEN_PRIVILEGES);

		if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious))
		{
			return xResultFromLastError();
		}

		// Second pass, set privilege based on previous setting
		tpPrevious.PrivilegeCount = 1;

		tpPrevious.Privileges[0].Luid = luid;

		if (enable)
		{
			tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
		}
		else
		{
			tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);
		}

		if (!AdjustTokenPrivileges(token, FALSE, &tpPrevious, cbPrevious, NULL, NULL))
		{
			return xResultFromLastError();
		}

		return TRUE;
	}

};

static xElevation& xElevationGet() { return xElevation::Get(); }
