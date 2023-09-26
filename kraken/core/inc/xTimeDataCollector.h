#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xTimeDataCollector: public xRefCount<IxTimeDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxTimeDataCollector)
X_INTERFACE_MAP_END

public:
	xTimeDataCollector();
	virtual ~xTimeDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxTimeData** timeData);
};

