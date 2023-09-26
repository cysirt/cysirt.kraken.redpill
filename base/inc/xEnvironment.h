#pragma once

#include <Windows.h>

#include "xAssert.h"
#include "xChar.h"
#include "xResult.h"
#include "xString.h"



class xEnvironment
{
public:
	xEnvironment()
	{
	}

	~xEnvironment()
	{
	}

	static xEnvironment& Get()
	{
		static xEnvironment s_self;

		return s_self;
	}

	xResult GetHostName(xChar** hostName)
	{
		X_ASSERT(hostName);

		if (!hostName)
		{
			return X_E_INVALID_ARGUMENT;
		}

		DWORD size = 0;

		if (GetComputerNameEx(ComputerNameDnsFullyQualified, NULL, &size) == 0 && GetLastError() == ERROR_MORE_DATA)
		{
			xString s;

			if (GetComputerNameEx(ComputerNameDnsFullyQualified, s.GetBuffer(size), &size))
			{
				s.ReleaseBuffer();

				*hostName = s.Clone();

				return X_S_OK;
			}
			else
			{
				return xResultFromLastError();
			}
		}
		else
		{
			return X_E_FAIL;
		}
	}

};

static xEnvironment& xEnvironmentGet() { return xEnvironment::Get(); }
