#pragma once

#include <Windows.h>

#include <wincrypt.h>
#include <LM.h>
#include <Mscat.h>
#include <Winternl.h>

#include "xDynamicModule.h"



// Undocumented windows API
typedef struct _xDnsCacheEntry
{
	struct _xDnsCacheEntry* next;
	PWSTR name;
	uint16_t type;
	uint16_t dataLength;
	uint32_t flags;
} xDnsCacheEntry;



class xDynamicCoreApi
{
private:
	xDynamicCoreApi()
	{
	}

public:
	static xDynamicCoreApi& GetDynamicCoreApi()
	{
		static xDynamicCoreApi s_self;

		return s_self;
	}

public:
	// Dnsapi
	X_DYNAMIC_MODULE_BEGIN(Dnsapi, L"Dnsapi.dll")
		X_DYNAMIC_MODULE_FUNCTION_1(NTSTATUS, DnsGetCacheDataTable, xDnsCacheEntry*)
	X_DYNAMIC_MODULE_END(Dnsapi)
	
	// Ntdll
	X_DYNAMIC_MODULE_BEGIN(Ntdll, L"ntdll.dll")
		X_DYNAMIC_MODULE_FUNCTION_11(NTSTATUS, NtCreateFile, PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, ULONG, ULONG, ULONG, ULONG, PVOID, ULONG)
		X_DYNAMIC_MODULE_FUNCTION_5(NTSTATUS, NtQueryInformationProcess, HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG)
	X_DYNAMIC_MODULE_END(Ntdll)

	// Wintrust
	X_DYNAMIC_MODULE_BEGIN(Wintrust, L"Wintrust.dll")
		X_DYNAMIC_MODULE_FUNCTION_5(NTSTATUS, CryptCATAdminAcquireContext2, HCATADMIN*, const GUID*, PCWSTR, PCCERT_STRONG_SIGN_PARA, DWORD)
		X_DYNAMIC_MODULE_FUNCTION_5(NTSTATUS, CryptCATAdminCalcHashFromFileHandle2, HCATADMIN, HANDLE, DWORD*, BYTE*, DWORD)
	X_DYNAMIC_MODULE_END(Wintrust)
};

static xDynamicCoreApi& xGetDynamicCoreApi() {return xDynamicCoreApi::GetDynamicCoreApi();}



#define X_CORE_DNSAPI_CALL(__Name)				xGetDynamicCoreApi().GetDnsapi().##__Name
#define X_CORE_DNSAPI_IS_AVAILABLE(__Name)		xGetDynamicCoreApi().GetDnsapi().GetFunction(#__Name)

#define X_CORE_NTDLL_CALL(__Name)				xGetDynamicCoreApi().GetNtdll().##__Name
#define X_CORE_NTDLL_IS_AVAILABLE(__Name)		xGetDynamicCoreApi().GetNtdll().GetFunction(#__Name)

#define X_CORE_WINTRUST_CALL(__Name)			xGetDynamicCoreApi().GetWintrust().##__Name
#define X_CORE_WINTRUST_IS_AVAILABLE(__Name)	xGetDynamicCoreApi().GetWintrust().GetFunction(#__Name)
