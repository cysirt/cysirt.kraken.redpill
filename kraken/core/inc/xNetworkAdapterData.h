#pragma once

#include "xCoreInterface.h"
#include "xProperty.h"
#include "xReferenceCount.h"


/*
class xNetworkAdapterData : public xProperty<xRefCount<IxNetworkAdapterData> >
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxNetworkAdapterData)
X_INTERFACE_MAP_END

#define X_PROPERTY_IP_ADDRESS_DATA_TYPE				X_PROPERTY_CREATE_TYPE(0x0001)
#define X_PROPERTY_IP_ADDRESS_DATA_TYPE_STRING		X_PROPERTY_CREATE_STRING(0x0002)
#define X_PROPERTY_IP_ADDRESS_DATA_IP				X_PROPERTY_CREATE_STRING(0x0003)

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET_TYPE(SetType, GetType, uint32_t, 0, X_PROPERTY_IP_ADDRESS_DATA_TYPE)
	X_PROPERTY_MAP_SET_GET_STRING(SetType, GetType, X_PROPERTY_IP_ADDRESS_DATA_TYPE_STRING)
	X_PROPERTY_MAP_SET_GET_STRING(SetIp, GetIp, X_PROPERTY_IP_ADDRESS_DATA_IP)
X_PROPERTY_MAP_END

public:
	xNetworkAdapterData()
	{
	}

	~xNetworkAdapterData()
	{
	}
};
*/