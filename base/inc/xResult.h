#pragma once

#include <errno.h>
#include <stdint.h>
#include <Windows.h>
#include <WinSock2.h>

#include "xChar.h"
#include "xDefinition.h"



typedef int64_t	xResult;



#define X_RESULT_CODE(__Result)		((__Result) & 0xFFFFFFFF)
#define X_RESULT_FACILITY(__Result)	(((__Result) >> 32) & 0x1FFFFFFF)
#define X_RESULT_SEVERITY(__Result)	(((__Result) >> 63) & 0x1)



#define X_RESULT_SEVERITY_SUCCESS	0
#define X_RESULT_SEVERITY_ERROR		1

#define X_RESULT_FACILITY_BASE		0x00
#define X_RESULT_FACILITY_ERRNO		0x02

#define X_RESULT_FACILITY_WIN32		0x03
#define X_RESULT_FACILITY_HRESULT	0x04



#define X_RESULT_CREATE(__Severity, __Facility, __Code)		((xResult) (((xResult) (__Severity) << 63) | ((xResult) (__Facility) << 32) | ((xResult) (__Code))))
#define X_RESULT_CREATE_SUCCESS(__Code)						(X_RESULT_CREATE(X_RESULT_SEVERITY_SUCCESS, X_RESULT_FACILITY_BASE, __Code))
#define X_RESULT_CREATE_ERROR(__Code)						(X_RESULT_CREATE(X_RESULT_SEVERITY_ERROR, X_RESULT_FACILITY_BASE, __Code))

#define X_RESULT_CREATE_ERROR_ERRNO(__Code)					(X_RESULT_CREATE(X_RESULT_SEVERITY_ERROR, X_RESULT_FACILITY_ERRNO, __Code))

#define X_RESULT_CREATE_SUCCESS_WIN32(__Code)				(X_RESULT_CREATE(X_RESULT_SEVERITY_SUCCESS, X_RESULT_FACILITY_WIN32, __Code))
#define X_RESULT_CREATE_ERROR_WIN32(__Code)					(X_RESULT_CREATE(X_RESULT_SEVERITY_ERROR, X_RESULT_FACILITY_WIN32, __Code))

#define X_RESULT_CREATE_SUCCESS_HRESULT(__Code)				(X_RESULT_CREATE(X_RESULT_SEVERITY_SUCCESS, X_RESULT_FACILITY_HRESULT, __Code))
#define X_RESULT_CREATE_ERROR_HRESULT(__Code)				(X_RESULT_CREATE(X_RESULT_SEVERITY_ERROR, X_RESULT_FACILITY_HRESULT, __Code))



#define X_S_OK							X_RESULT_CREATE_SUCCESS(0x0000)
#define X_S_TRUE						X_RESULT_CREATE_SUCCESS(0x0001)
#define X_S_FALSE						X_RESULT_CREATE_SUCCESS(0x0002)



#define X_E_FAIL						X_RESULT_CREATE_ERROR(0x0001)
#define X_E_INTERNAL					X_RESULT_CREATE_ERROR(0x0002)
#define X_E_INVALID_ARGUMENT			X_RESULT_CREATE_ERROR(0x0003)
#define X_E_INVALID_DATA				X_RESULT_CREATE_ERROR(0x0004)
#define X_E_INVALID_TYPE				X_RESULT_CREATE_ERROR(0x0005)
#define X_E_NO_DATA						X_RESULT_CREATE_ERROR(0x0006)
#define X_E_NO_INTERFACE				X_RESULT_CREATE_ERROR(0x0007)
#define X_E_NOT_IMPLEMENTED				X_RESULT_CREATE_ERROR(0x0008)
#define X_E_NOT_PRESENT					X_RESULT_CREATE_ERROR(0x0009)
#define X_E_OUT_OF_MEMORY				X_RESULT_CREATE_ERROR(0x000a)
#define X_E_TIMEOUT						X_RESULT_CREATE_ERROR(0x000b)
#define X_E_UNEXPECTED					X_RESULT_CREATE_ERROR(0x000c)



#define X_RESULT_FORMAT_STRING			X_CHAR_FORMAT_HEX_16_W



static __X_INLINE bool xSucceeded(const xResult r)
{
	return X_RESULT_SEVERITY(r) == 0;
}



static __X_INLINE bool xFailed(const xResult r)
{
	return !xSucceeded(r);
}



static xResult xResultFromErrNo()
{
	return X_RESULT_CREATE_ERROR_ERRNO(errno);
}

static xResult xResultFromWin32(const uint32_t code)
{
	return code == ERROR_SUCCESS ? X_RESULT_CREATE_SUCCESS_WIN32(code) : X_RESULT_CREATE_ERROR_WIN32(code);
}

static xResult xResultFromLastError()
{
	return xResultFromWin32(GetLastError());
}

static xResult xResultFromWsaLastError()
{
	return xResultFromWin32(WSAGetLastError());
}

static xResult xResultFromHresult(const HRESULT code)
{
	return SUCCEEDED(code) ? X_RESULT_CREATE_SUCCESS_HRESULT(code) : X_RESULT_CREATE_ERROR_HRESULT(code);
}
