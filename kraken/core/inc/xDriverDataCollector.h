#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xDriverDataCollector: public xRefCount<IxDriverDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDriverDataCollector)
X_INTERFACE_MAP_END

public:
	xDriverDataCollector();
	virtual ~xDriverDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxDriverDataEnum** driverDataEnum);
};

