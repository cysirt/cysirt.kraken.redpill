#pragma once

#include "xModuleData.h"



class xDriverData : public xModuleDataBase<IxDriverData>
{
typedef xModuleDataBase<IxDriverData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDriverData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(NativeFileName, xChar*)
X_PROPERTY_MAP_END

public:
	xDriverData()
	{
	}

	virtual ~xDriverData()
	{
	}
};

