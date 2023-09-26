#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xDataContainer : public xPropertyEnum<IxDataContainer>
{
typedef xPropertyEnum<IxDataContainer> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDataContainer)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET_INTERFACE(DnsCacheData, IxDnsCacheDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(DriverData, IxDriverDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(MetaData, IxMetaData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(NetworkConnectionData, IxNetworkConnectionDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(PrefetchData, IxPrefetchDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ProcessData, IxProcessDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(RegistryData, IxRegistryKeyDataEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(OperatingSystemData, IxOperatingSystemData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(UserAccountData, IxUserAccountDataEnum)
X_PROPERTY_MAP_END

public:
	xDataContainer()
	{
	}

	virtual ~xDataContainer()
	{
	}
};
