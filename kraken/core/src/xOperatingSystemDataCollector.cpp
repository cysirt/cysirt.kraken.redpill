#include "xOperatingSystemDataCollector.h"

#include <atlbase.h>
#include <comdef.h>
#include <Wbemidl.h>

#include "xConversion.h"
#include "xString.h"

#include "xOperatingSystemData.h"



xOperatingSystemDataCollector::xOperatingSystemDataCollector()
{

}

xOperatingSystemDataCollector::~xOperatingSystemDataCollector()
{

}

xResult xOperatingSystemDataCollector::Collect(IxOperatingSystemData** operatingSystemData)
{
	X_ASSERT(operatingSystemData);

	if (!operatingSystemData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xOperatingSystemData> osd(new xOperatingSystemData);

	if (!osd)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xResult r = X_S_OK;

	DWORD size = 0;

	if (GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, NULL, &size) == 0)
	{
		xString fqdn;

		if (GetComputerNameEx(ComputerNamePhysicalDnsFullyQualified, fqdn.GetBuffer(size), &size))
		{
			fqdn.ReleaseBuffer();

			osd->SetFullyQualifiedDomainName(fqdn);
		}
	}

	CComPtr<IWbemLocator> wbl;

	HRESULT hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (void**) &wbl);

	if (FAILED(hr))
	{
		return xResultFromHresult(hr);
	}

	CComPtr<IWbemServices> wbs;

	if (FAILED(hr = wbl->ConnectServer(bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbs)) ||
		FAILED(hr = CoSetProxyBlanket(wbs, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
	{
		return xResultFromHresult(hr);
	}

	CComPtr<IEnumWbemClassObject> ewbco;

	if (FAILED(hr = wbs->ExecQuery(bstr_t(L"WQL"), bstr_t(L"SELECT * FROM Win32_OperatingSystem"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &ewbco)))
	{
		return xResultFromHresult(hr);
	}

	CComPtr<IWbemClassObject> wbco;

	ULONG result = 0;

	if (SUCCEEDED(hr = ewbco->Next(WBEM_INFINITE, 1, &wbco, &result))  && result)
	{
		variant_t prop;

		if (SUCCEEDED(hr = wbco->Get(L"BootDevice", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetBootDevice(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"BuildNumber", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetBuildNumber(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"BuildType", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetBuildType(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Caption", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetCaption(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"CodeSet", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetCodeSet(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"CountryCode", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetCountryCode(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"CSDVersion", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetCsdVersion(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"CurrentTimeZone", 0, &prop, 0, 0)))
		{
			osd->SetCurrentTimeZone(prop.intVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"DataExecutionPrevention_32BitApplications", 0, &prop, 0, 0)))
		{
			osd->SetDataExecutionPrevention32BitApplications(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"DataExecutionPrevention_Available", 0, &prop, 0, 0)))
		{
			osd->SetDataExecutionPreventionAvailable(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"DataExecutionPrevention_Drivers", 0, &prop, 0, 0)))
		{
			osd->SetDataExecutionPreventionDrivers(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"DataExecutionPrevention_SupportPolicy", 0, &prop, 0, 0)))
		{
			osd->SetDataExecutionPreventionSupportPolicy(prop.bVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Distributed", 0, &prop, 0, 0)))
		{
			osd->SetDistributed(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"EncryptionLevel", 0, &prop, 0, 0)))
		{
			osd->SetEncryptionLevel(prop.uintVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"InstallDate", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetInstallDate(xConvertCimDateTimeToTime(prop.bstrVal));
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"LastBootUpTime", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetLastBootUpTime(xConvertCimDateTimeToTime(prop.bstrVal));
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"LocalDateTime", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetLocalDateTime(xConvertCimDateTimeToTime(prop.bstrVal));
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Name", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetName(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"NumberOfUsers", 0, &prop, 0, 0)))
		{
			osd->SetNumberOfUsers(prop.uintVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"OperatingSystemSKU", 0, &prop, 0, 0)))
		{
			osd->SetOperatingSystemSku(prop.uintVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Organization", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetOrganization(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"OSArchitecture", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetOsArchitecture(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"PortableOperatingSystem", 0, &prop, 0, 0)))
		{
			osd->SetPortableOperatingSystem(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Primary", 0, &prop, 0, 0)))
		{
			osd->SetPrimary(prop.boolVal == VARIANT_TRUE);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"ProductType", 0, &prop, 0, 0)))
		{
			osd->SetProductType(prop.uintVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"RegisteredUser", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetRegisteredUser(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"SerialNumber", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetSerialNumber(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"ServicePackMajorVersion", 0, &prop, 0, 0)))
		{
			osd->SetServicePackMajorVersion(prop.uiVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"ServicePackMinorVersion", 0, &prop, 0, 0)))
		{
			osd->SetServicePackMinorVersion(prop.uiVal);
		}

		prop.Clear();

		// TODO
		if (SUCCEEDED(hr = wbco->Get(L"SuiteMask", 0, &prop, 0, 0)))
		{
			osd->SetSuiteMask(prop.uintVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"SystemDevice", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetSystemDevice(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"SystemDirectory", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetSystemDirectory(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"SystemDrive", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetSystemDrive(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"Version", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetVersion(prop.bstrVal);
		}

		prop.Clear();

		if (SUCCEEDED(hr = wbco->Get(L"WindowsDirectory", 0, &prop, 0, 0)) && prop.bstrVal && SysStringLen(prop.bstrVal))
		{
			osd->SetWindowsDirectory(prop.bstrVal);
		}

		prop.Clear();
	}

	*operatingSystemData = osd.Detach();

	return X_S_OK;
}

