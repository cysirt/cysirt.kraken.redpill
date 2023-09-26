#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xFileVersionDataCollector : public xRefCount<IxFileVersionDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxFileVersionDataCollector)
X_INTERFACE_MAP_END

public:
	xFileVersionDataCollector();
	virtual ~xFileVersionDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxFileVersionData** fileVersionData);
};

