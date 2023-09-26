#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xImportTableDataCollector: public xRefCount<IxImportTableDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxImportTableDataCollector)
X_INTERFACE_MAP_END

public:
	xImportTableDataCollector();
	virtual ~xImportTableDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxImportTableDataEnum** importTableDataEnum);
};

