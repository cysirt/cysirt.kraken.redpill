#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xDnsCacheData : public xPropertyEnum<IxDnsCacheData>
{
typedef xPropertyEnum<IxDnsCacheData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDnsCacheData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Name, xChar*)
	X_PROPERTY_MAP_SET_GET(Type, uint16_t)
	X_PROPERTY_MAP_SET_GET(DataLength, uint16_t)
	X_PROPERTY_MAP_SET_GET(Flags, uint32_t)
	X_PROPERTY_MAP_SET_GET(Address, xChar*)
	X_PROPERTY_MAP_SET_GET(TimeToLive, uint32_t)
X_PROPERTY_MAP_END

public:
	xDnsCacheData()
	{
	}

	virtual ~xDnsCacheData()
	{
	}
};

