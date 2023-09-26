#pragma once

#include <atlbase.h>
#include <ole2.h>
#include <time.h>
#include <wbemidl.h>
#include <Windows.h>

#include "xAssert.h"
#include "xChar.h"
#include "xMemory.h"
#include "xReferenceCount.h"
#include "xResult.h"
#include "xString.h"
#include "xStringEnum.h"



static char* xConvertUtf16ToUtf8(const xChar* str)
{
	X_ASSERT(str);

	if (!str)
	{
		return NULL;
	}

	const int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL) + sizeof(char);

	xPtr<char> temp((char*) xMemoryAlloc(size * sizeof(char)));

	if (!temp)
	{
		return NULL;
	}

	return WideCharToMultiByte(CP_UTF8, 0, str, -1, temp, size, NULL, NULL) ? temp.Detach() : NULL;
}

static xChar* xConvertUtf8ToUtf16(const char* str)
{
	X_ASSERT(str);

	if (!str)
	{
		return NULL;
	}

	const int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);

	xPtr<xChar> temp((xChar*) xMemoryAlloc(size * sizeof(xChar)));

	if (!temp)
	{
		return NULL;
	}

	return MultiByteToWideChar(CP_UTF8, 0, str, -1, temp, size) ? temp.Detach() : NULL;
}

static time_t xConvertFileTimeToTime(const FILETIME& fileTime)
{
	ULARGE_INTEGER uli;

	uli.LowPart = fileTime.dwLowDateTime;

	uli.HighPart = fileTime.dwHighDateTime;

	return uli.QuadPart / 10000000ull - 11644473600ull;
}

static time_t xConvertSystemTimeToTime(const SYSTEMTIME& st)
{
	struct tm tm;
	
	tm.tm_sec = st.wSecond;

	tm.tm_min = st.wMinute;

	tm.tm_hour = st.wHour;

	tm.tm_mday = st.wDay;

	tm.tm_mon = st.wMonth - 1;

	tm.tm_year = st.wYear - 1900;

	tm.tm_isdst = -1;
	
	return mktime(&tm);
}

static xResult xConvertTimeToIso8601(const time_t time, xChar** iso8601)
{
	X_ASSERT(iso8601);

	if (!iso8601)
	{
		return X_E_INVALID_ARGUMENT;
	}

	const uint32_t size = (uint32_t) xCharLength(X_CHAR("0000-00-00T00:00:00Z")) + 1;

	xPtr<xChar> s((xChar*) xMemoryAlloc(size * sizeof(xChar)));

	if (!s)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if (!xCharFormatTime(s, size, X_CHAR("%Y-%m-%dT%H:%M:%SZ"), gmtime(&time)))
	{

	}

	*iso8601 = s.Detach();

	return X_S_OK;
}

static time_t xConvertCimDateTimeToTime(const BSTR cimDateTime)
{
	X_ASSERT(cimDateTime);

	if (!cimDateTime)
	{
		return 0;
	}

	CComPtr<ISWbemDateTime> wbem;
	
	HRESULT hr = CoCreateInstance(CLSID_SWbemDateTime, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wbem));

	if (SUCCEEDED(hr))
	{
		wbem->put_Value(cimDateTime);

		DATE date = 0;

		if (SUCCEEDED(hr = wbem->GetVarDate(VARIANT_TRUE, &date)))
		{
			SYSTEMTIME st;

			if (VariantTimeToSystemTime(date, &st))
			{
				return xConvertSystemTimeToTime(st);
			}
		}
	}

	return 0;
}

static xChar* xConvertBinaryToHex(const uint8_t* buffer, const uint32_t size)
{
	X_ASSERT(buffer && size);

	if (!buffer || !size)
	{
		return false;
	}

	xString s;

	xChar* temp = s.GetBuffer(size * sizeof(xChar));

	if (!temp)
	{
		return NULL;
	}

	for (uint32_t ui = 0; ui < size; ui++)
	{
		wsprintf(temp + ui * 2, X_CHAR("%02x"), buffer[ui]);
	}

	s.ReleaseBuffer();

	return s.Clone();
}

static xResult xConvertMultiStringToStringEnum(const xChar* multiString, IxStringEnum** stringEnum)
{
	X_ASSERT(multiString && stringEnum);

	if (!multiString || !stringEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}


	xInterfacePtr<xStringEnum> se(new xStringEnum);

	if (!se)
	{
		return X_E_OUT_OF_MEMORY;
	}

	const xChar* iterator = NULL;

	for (iterator = multiString; *iterator; iterator += xCharLength(iterator) + 1)
	{
		se->Add(iterator);
	}

	if (se->GetCount())
	{
		*stringEnum = se.Detach();
	}

	return (*stringEnum) ? X_S_OK : X_E_NO_DATA;
}