#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xModuleDataCollector: public xRefCount<IxModuleDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxModuleDataCollector)
X_INTERFACE_MAP_END

public:
	xModuleDataCollector();
	virtual ~xModuleDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxModuleData** moduleData);

private:
};

