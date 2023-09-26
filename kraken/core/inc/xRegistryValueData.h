#pragma once

#include "xConversion.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xRegistryValueData : public xCorePropertyEnum<IxRegistryValueData>
{
typedef xCorePropertyEnum<IxRegistryValueData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxRegistryValueData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(RootKey, xChar*)
	X_PROPERTY_MAP_SET_GET(Path, xChar*)
	X_PROPERTY_MAP_SET_GET(Name, xChar*)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(Type, uint32_t)
	X_PROPERTY_MAP_SET_GET(BinaryValue, xChar*)
	X_PROPERTY_MAP_SET_GET(DwordValue, uint32_t)
	X_PROPERTY_MAP_SET_GET(BigEndianDwordValue, uint32_t)
	X_PROPERTY_MAP_SET_GET(EnvironmentStringValue, xChar*)
	X_PROPERTY_MAP_SET_GET(LinkValue, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(MultiStringValue, IxStringEnum)
	X_PROPERTY_MAP_SET_GET(QwordValue, uint64_t)
	X_PROPERTY_MAP_SET_GET(StringValue, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(FileData, IxFileData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ModuleData, IxModuleData)
X_PROPERTY_MAP_END

public:
	xRegistryValueData()
	{
	}

	virtual ~xRegistryValueData()
	{
	}

	X_INTERFACE_METHOD(xResult, GetTypeString)(xChar** typeString)
	{
		X_ASSERT(typeString);

		if (!typeString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t type = 0;

		xResult r = GetType(&type);

		if (xFailed(r))
		{
			return r;
		}

		switch (type)
		{
			case REG_BINARY:
			{
				*typeString = xCharClone(X_CHAR("REG_BINARY"));

				break;
			}
			case REG_DWORD:
			{
				*typeString = xCharClone(X_CHAR("REG_DWORD"));

				break;
			}
			case REG_DWORD_BIG_ENDIAN:
			{
				*typeString = xCharClone(X_CHAR("REG_DWORD_BIG_ENDIAN"));

				break;
			}
			case REG_EXPAND_SZ:
			{
				*typeString = xCharClone(X_CHAR("REG_EXPAND_SZ"));

				break;
			}
			case REG_LINK:
			{
				*typeString = xCharClone(X_CHAR("REG_LINK"));

				break;
			}
			case REG_MULTI_SZ:
			{
				*typeString = xCharClone(X_CHAR("REG_MULTI_SZ"));

				break;
			}
			case REG_NONE:
			{
				*typeString = xCharClone(X_CHAR("REG_NONE"));

				break;
			}
			case REG_QWORD:
			{
				*typeString = xCharClone(X_CHAR("REG_QWORD"));

				break;
			}
			case REG_SZ:
			{
				*typeString = xCharClone(X_CHAR("REG_SZ"));

				break;
			}
			case REG_RESOURCE_LIST:
			{
				*typeString = xCharClone(X_CHAR("REG_RESOURCE_LIST"));

				break;
			}
			case REG_FULL_RESOURCE_DESCRIPTOR:
			{
				*typeString = xCharClone(X_CHAR("REG_FULL_RESOURCE_DESCRIPTOR"));

				break;
			}
			case REG_RESOURCE_REQUIREMENTS_LIST:
			{
				*typeString = xCharClone(X_CHAR("REG_RESOURCE_REQUIREMENTS_LIST"));

				break;
			}
			default:
			{
				*typeString = xCharClone(X_CHAR("UNKNOWN"));

				break;
			}
		}

		return (*typeString) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}
};
