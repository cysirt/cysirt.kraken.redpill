#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xFileDataCollector: public xRefCount<IxFileDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxFileDataCollector)
X_INTERFACE_MAP_END

public:
	xFileDataCollector();
	virtual ~xFileDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxFileData** fileData);

private:
	xResult GetFileSizeData(const xChar* fileName, uint64_t* size);

};

