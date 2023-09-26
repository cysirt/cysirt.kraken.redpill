#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xOperatingSystemData : public xCorePropertyEnum<IxOperatingSystemData>
{
typedef xCorePropertyEnum<IxOperatingSystemData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxOperatingSystemData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(FullyQualifiedDomainName, xChar*)
	X_PROPERTY_MAP_SET_GET(BootDevice, xChar*)
	X_PROPERTY_MAP_SET_GET(BuildNumber, xChar*)
	X_PROPERTY_MAP_SET_GET(BuildType, xChar*)
	X_PROPERTY_MAP_SET_GET(Caption, xChar*)
	X_PROPERTY_MAP_SET_GET(CodeSet, xChar*)
	X_PROPERTY_MAP_SET_GET(CountryCode, xChar*)
	X_PROPERTY_MAP_SET_GET(CsdVersion, xChar*)
	X_PROPERTY_MAP_SET_GET(CurrentTimeZone, int16_t)
	X_PROPERTY_MAP_SET_GET(DataExecutionPrevention32BitApplications, bool)
	X_PROPERTY_MAP_SET_GET(DataExecutionPreventionAvailable, bool)
	X_PROPERTY_MAP_SET_GET(DataExecutionPreventionDrivers, bool)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(DataExecutionPreventionSupportPolicy, uint8_t)
	X_PROPERTY_MAP_SET_GET(Distributed, bool)
	X_PROPERTY_MAP_SET_GET(EncryptionLevel, uint32_t)
	X_PROPERTY_MAP_SET_GET(InstallDate, time_t)
	X_PROPERTY_MAP_SET_GET(InstallDateString, xChar*)
	X_PROPERTY_MAP_SET_GET(LastBootUpTime, time_t)
	X_PROPERTY_MAP_SET_GET(LastBootUpTimeString, xChar*)
	X_PROPERTY_MAP_SET_GET(LocalDateTime, time_t)
	X_PROPERTY_MAP_SET_GET(LocalDateTimeString, xChar*)
	X_PROPERTY_MAP_SET_GET(Locale, xChar*)
	X_PROPERTY_MAP_SET_GET(Name, xChar*)
	X_PROPERTY_MAP_SET_GET(NumberOfUsers, uint32_t)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(OperatingSystemSku, uint32_t)
	X_PROPERTY_MAP_SET_GET(Organization, xChar*)
	X_PROPERTY_MAP_SET_GET(OsArchitecture, xChar*)
	X_PROPERTY_MAP_SET_GET(PortableOperatingSystem, bool)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(ProductType, uint32_t)
	X_PROPERTY_MAP_SET_GET(Primary, bool)
	X_PROPERTY_MAP_SET_GET(RegisteredUser, xChar*)
	X_PROPERTY_MAP_SET_GET(SerialNumber, xChar*)
	X_PROPERTY_MAP_SET_GET(ServicePackMajorVersion, uint16_t)
	X_PROPERTY_MAP_SET_GET(ServicePackMinorVersion, uint16_t)
	X_PROPERTY_MAP_SET_GET(SuiteMask, uint32_t)
	X_PROPERTY_MAP_SET_GET(SystemDevice, xChar*)
	X_PROPERTY_MAP_SET_GET(SystemDrive, xChar*)
	X_PROPERTY_MAP_SET_GET(SystemDirectory, xChar*)
	X_PROPERTY_MAP_SET_GET(Version, xChar*)
	X_PROPERTY_MAP_SET_GET(WindowsDirectory, xChar*)
X_PROPERTY_MAP_END

public:
	xOperatingSystemData()
	{
	}

	virtual ~xOperatingSystemData()
	{
	}

	X_INTERFACE_METHOD(xResult, GetDataExecutionPreventionSupportPolicyString)(xChar** dataExecutionPreventionSupportPolicyString)
	{
		X_ASSERT(dataExecutionPreventionSupportPolicyString);

		if (!dataExecutionPreventionSupportPolicyString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint8_t dataExecutionPreventionSupportPolicy = 0;

		xResult r = GetDataExecutionPreventionSupportPolicy(&dataExecutionPreventionSupportPolicy);

		if (xFailed(r))
		{
			return r;
		}

		switch (dataExecutionPreventionSupportPolicy)
		{
			case 0:
			{
				*dataExecutionPreventionSupportPolicyString = xCharClone(X_CHAR("AlwaysOff"));

				break;
			}
			case 1:
			{
				*dataExecutionPreventionSupportPolicyString = xCharClone(X_CHAR("AlwaysOn"));

				break;
			}
			case 2:
			{
				*dataExecutionPreventionSupportPolicyString = xCharClone(X_CHAR("OptIn"));

				break;
			}
			case 3:
			{
				*dataExecutionPreventionSupportPolicyString = xCharClone(X_CHAR("OptOut"));

				break;
			}
			default:
			{
				*dataExecutionPreventionSupportPolicyString = xCharClone(X_CHAR("Unknown"));

				break;
			}
		}

		return (*dataExecutionPreventionSupportPolicyString) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}

	X_INTERFACE_METHOD(xResult, GetOperatingSystemSkuString)(xChar** operatingSystemSkuString)
	{
		X_ASSERT(operatingSystemSkuString);

		if (!operatingSystemSkuString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t operatingSystemSku = 0;

		xResult r = GetOperatingSystemSku(&operatingSystemSku);

		if (xFailed(r))
		{
			return r;
		}
		
		const xChar* sku[] =
		{
			X_CHAR("PRODUCT_UNDEFINED"),
			X_CHAR("PRODUCT_ULTIMATE"),
			X_CHAR("PRODUCT_HOME_BASIC"),
			X_CHAR("PRODUCT_HOME_PREMIUM"),
			X_CHAR("PRODUCT_ENTERPRISE"),
			X_CHAR("PRODUCT_HOME_BASIC_N"),
			X_CHAR("PRODUCT_BUSINESS"),
			X_CHAR("PRODUCT_STANDARD_SERVER"),
			X_CHAR("PRODUCT_DATACENTER_SERVER"),
			X_CHAR("PRODUCT_SMALLBUSINESS_SERVER"),
			X_CHAR("PRODUCT_ENTERPRISE_SERVER"),
			X_CHAR("PRODUCT_STARTER"),
			X_CHAR("PRODUCT_DATACENTER_SERVER_CORE"),
			X_CHAR("PRODUCT_STANDARD_SERVER_CORE"),
			X_CHAR("PRODUCT_ENTERPRISE_SERVER_CORE"),
			X_CHAR("PRODUCT_ENTERPRISE_SERVER_IA64"),
			X_CHAR("PRODUCT_BUSINESS_N"),
			X_CHAR("PRODUCT_WEB_SERVER"),
			X_CHAR("PRODUCT_CLUSTER_SERVER"),
			X_CHAR("PRODUCT_HOME_SERVER"),
			X_CHAR("PRODUCT_STORAGE_EXPRESS_SERVER"),
			X_CHAR("PRODUCT_STORAGE_STANDARD_SERVER"),
			X_CHAR("PRODUCT_STORAGE_WORKGROUP_SERVER"),
			X_CHAR("PRODUCT_STORAGE_ENTERPRISE_SERVER"),
			X_CHAR("PRODUCT_SERVER_FOR_SMALLBUSINESS"),
			X_CHAR("PRODUCT_SMALLBUSINESS_SERVER_PREMIUM"),
			X_CHAR("PRODUCT_HOME_PREMIUM_N"),
			X_CHAR("PRODUCT_ENTERPRISE_N"),
			X_CHAR("PRODUCT_ULTIMATE_N"),
			X_CHAR("PRODUCT_WEB_SERVER_CORE"),
			X_CHAR("PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT"),
			X_CHAR("PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY"),
			X_CHAR("PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING"),
			X_CHAR("PRODUCT_SERVER_FOUNDATION"),
			X_CHAR("PRODUCT_HOME_PREMIUM_SERVER"),
			X_CHAR("PRODUCT_SERVER_FOR_SMALLBUSINESS_V"),
			X_CHAR("PRODUCT_STANDARD_SERVER_V"),
			X_CHAR("PRODUCT_DATACENTER_SERVER_V"),
			X_CHAR("PRODUCT_ENTERPRISE_SERVER_V"),
			X_CHAR("PRODUCT_DATACENTER_SERVER_CORE_V"),
			X_CHAR("PRODUCT_STANDARD_SERVER_CORE_V"),
			X_CHAR("PRODUCT_ENTERPRISE_SERVER_CORE_V"),
			X_CHAR("PRODUCT_HYPERV"),
			X_CHAR("PRODUCT_STORAGE_EXPRESS_SERVER_CORE"),
			X_CHAR("PRODUCT_STORAGE_STANDARD_SERVER_CORE"),
			X_CHAR("PRODUCT_STORAGE_WORKGROUP_SERVER_CORE"),
			X_CHAR("PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE"),
			X_CHAR("PRODUCT_STARTER_N"),
			X_CHAR("PRODUCT_PROFESSIONAL"),
			X_CHAR("PRODUCT_PROFESSIONAL_N"),
			X_CHAR("PRODUCT_SB_SOLUTION_SERVER"),
			X_CHAR("PRODUCT_SERVER_FOR_SB_SOLUTIONS"),
			X_CHAR("PRODUCT_STANDARD_SERVER_SOLUTIONS"),
			X_CHAR("PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE"),
			X_CHAR("PRODUCT_SB_SOLUTION_SERVER_EM"),
			X_CHAR("PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM"),
			X_CHAR("PRODUCT_SOLUTION_EMBEDDEDSERVER"),
			X_CHAR("PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE"),
			X_CHAR("PRODUCT_PROFESSIONAL_EMBEDDED"),
			X_CHAR("PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT"),
			X_CHAR("PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL"),
			X_CHAR("PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC"),
			X_CHAR("PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC"),
			X_CHAR("PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE"),
			X_CHAR("PRODUCT_CLUSTER_SERVER_V"),
			X_CHAR("PRODUCT_EMBEDDED"),
			X_CHAR("PRODUCT_STARTER_E"),
			X_CHAR("PRODUCT_HOME_BASIC_E"),
			X_CHAR("PRODUCT_HOME_PREMIUM_E"),
			X_CHAR("PRODUCT_PROFESSIONAL_E"),
			X_CHAR("PRODUCT_ENTERPRISE_E"),
			X_CHAR("PRODUCT_ULTIMATE_E"),
			X_CHAR("PRODUCT_ENTERPRISE_EVALUATION"),
			X_CHAR("PRODUCT_MULTIPOINT_STANDARD_SERVER"),
			X_CHAR("PRODUCT_MULTIPOINT_PREMIUM_SERVER"),
			X_CHAR("PRODUCT_STANDARD_EVALUATION_SERVER"),
			X_CHAR("PRODUCT_DATACENTER_EVALUATION_SERVER"),
			X_CHAR("PRODUCT_ENTERPRISE_N_EVALUATION"),
			X_CHAR("PRODUCT_EMBEDDED_AUTOMOTIVE"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY_A"),
			X_CHAR("PRODUCT_THINPC"),
			X_CHAR("PRODUCT_EMBEDDED_A"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY"),
			X_CHAR("PRODUCT_EMBEDDED_E"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY_E"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY_A_E"),
			X_CHAR("PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER"),
			X_CHAR("PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER"),
			X_CHAR("PRODUCT_CORE_ARM"),
			X_CHAR("PRODUCT_CORE_N"),
			X_CHAR("PRODUCT_CORE_COUNTRYSPECIFIC"),
			X_CHAR("PRODUCT_CORE_SINGLELANGUAGE"),
			X_CHAR("PRODUCT_CORE"),
			X_CHAR("PRODUCT_PROFESSIONAL_WMC"),
			X_CHAR("PRODUCT_MOBILE_CORE"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY_EVAL"),
			X_CHAR("PRODUCT_EMBEDDED_INDUSTRY_E_EVAL"),
			X_CHAR("PRODUCT_EMBEDDED_EVAL"),
			X_CHAR("PRODUCT_EMBEDDED_E_EVAL"),
			X_CHAR("PRODUCT_CORE_SERVER"),
			X_CHAR("PRODUCT_CLOUD_STORAGE_SERVER"),
			X_CHAR("PRODUCT_CORE_CONNECTED"),
			X_CHAR("PRODUCT_PROFESSIONAL_STUDENT"),
			X_CHAR("PRODUCT_CORE_CONNECTED_N"),
			X_CHAR("PRODUCT_PROFESSIONAL_STUDENT_N"),
			X_CHAR("PRODUCT_CORE_CONNECTED_SINGLELANGUAGE"),
			X_CHAR("PRODUCT_CORE_CONNECTED_COUNTRYSPECIFIC")
		};

		if (operatingSystemSku == PRODUCT_UNLICENSED)
		{
			*operatingSystemSkuString = xCharClone(X_CHAR("PRODUCT_UNLICENSED"));
		}
		else if (operatingSystemSku >= (sizeof(sku) / sizeof(xChar*)))
		{
			*operatingSystemSkuString = xCharClone(sku[0]);
		}
		else
		{
			*operatingSystemSkuString = xCharClone(sku[operatingSystemSku]);
		}

		return (*operatingSystemSkuString) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}

	X_INTERFACE_METHOD(xResult, GetProductTypeString)(xChar** productTypeString)
	{
		X_ASSERT(productTypeString);

		if (!productTypeString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t productType = 0;

		xResult r = GetProductType(&productType);

		if (xFailed(r))
		{
			return r;
		}
		
		switch (productType)
		{
			case 1:
			{
				*productTypeString = xCharClone(X_CHAR("Workstation"));

				break;
			}
			case 2:
			{
				*productTypeString = xCharClone(X_CHAR("DomainController"));

				break;
			}
			case 3:
			{
				*productTypeString = xCharClone(X_CHAR("Server"));

				break;
			}
			default:
			{
				*productTypeString = xCharClone(X_CHAR("Unknown"));

				break;
			}
		}

		return (*productTypeString) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}
};
