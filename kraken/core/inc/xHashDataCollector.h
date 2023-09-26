#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xHashDataCollector: public xRefCount<IxHashDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxHashDataCollector)
X_INTERFACE_MAP_END

public:
	xHashDataCollector();
	virtual ~xHashDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxHashData** hashData);
};

