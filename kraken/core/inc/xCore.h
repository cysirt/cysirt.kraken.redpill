#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xCore : public xRefCount<IxCore>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxCore)
X_INTERFACE_MAP_END

public:
	xCore();
	virtual ~xCore();

	X_INTERFACE_METHOD(xResult, Init)();
	X_INTERFACE_METHOD(xResult, Shutdown)();

	X_INTERFACE_METHOD(xResult, AddLogWriter)(IxLogWriter* logWriter);

	X_INTERFACE_METHOD(xResult, CreateCommandManagerOption)(IxCommandManagerOption** commandManagerOption);
	X_INTERFACE_METHOD(xResult, CreateCommandManager)(IxCommandManager** commandManager);

	X_INTERFACE_METHOD(xResult, CreateDataCollector)(IxDataCollector** dataCollector);
};

