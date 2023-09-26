#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xPrefetchData : public xPropertyEnum<IxPrefetchData>
{
typedef xPropertyEnum<IxPrefetchData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxPrefetchData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(FileName, xChar*)
	X_PROPERTY_MAP_SET_GET(ExecutableFileName, xChar*)
	X_PROPERTY_MAP_SET_GET(Hash, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(LastRunTimes, IxTimeEnum)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ItemData, IxPrefetchItemDataEnum)
X_PROPERTY_MAP_END

public:
	xPrefetchData()
	{
	}

	virtual ~xPrefetchData()
	{
	}
};

