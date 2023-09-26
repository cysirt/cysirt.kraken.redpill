#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xNetworkConnectionData : public xPropertyEnum<IxNetworkConnectionData>
{
typedef xPropertyEnum<IxNetworkConnectionData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxNetworkConnectionData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Type, uint32_t)
	X_PROPERTY_MAP_SET_GET(TypeString, xChar*)
	X_PROPERTY_MAP_SET_GET(State, xChar*)
	X_PROPERTY_MAP_SET_GET(LocalAddress, xChar*)
	X_PROPERTY_MAP_SET_GET(LocalHost, xChar*)
	X_PROPERTY_MAP_SET_GET(LocalPort, uint16_t)
	X_PROPERTY_MAP_SET_GET(RemoteAddress, xChar*)
	X_PROPERTY_MAP_SET_GET(RemoteHost, xChar*)
	X_PROPERTY_MAP_SET_GET(RemotePort, uint16_t)
	X_PROPERTY_MAP_SET_GET_INTERFACE(OwningProcess, IxProcessData)
X_PROPERTY_MAP_END

public:
	xNetworkConnectionData()
	{
	}

	virtual ~xNetworkConnectionData()
	{
	}
};
