#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xMetaDataCollector : public xRefCount<IxMetaDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxMetaDataCollector)
X_INTERFACE_MAP_END

public:
	xMetaDataCollector();
	virtual ~xMetaDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxMetaData** metaData);
};

