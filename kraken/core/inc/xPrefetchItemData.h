#pragma once

#include "xProperty.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xPrefetchItemData : public xPropertyEnum<IxPrefetchItemData>
{
typedef xPropertyEnum<IxPrefetchItemData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxPrefetchItemData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(NativeFileName, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(Item, IxInterface)
X_PROPERTY_MAP_END

public:
	xPrefetchItemData()
	{
	}

	virtual ~xPrefetchItemData()
	{
	}
};

