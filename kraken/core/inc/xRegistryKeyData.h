#pragma once

#include "xConversion.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xRegistryKeyData : public xCorePropertyEnum<IxRegistryKeyData>
{
typedef xCorePropertyEnum<IxRegistryKeyData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxRegistryKeyData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(RootKey, xChar*)
	X_PROPERTY_MAP_SET_GET(Path, xChar*)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(LastModified)
	X_PROPERTY_MAP_SET_GET_INTERFACE(KeyData, IxRegistryKeyDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ValueData, IxRegistryValueDataEnum)
X_PROPERTY_MAP_END

public:
	xRegistryKeyData()
	{
	}

	virtual ~xRegistryKeyData()
	{
	}
};
