#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xCommandManager : public xRefCount<IxCommandManager>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxCommandManager)
X_INTERFACE_MAP_END

public:
	xCommandManager();
	virtual ~xCommandManager();

	X_INTERFACE_METHOD(xResult, Execute)(IxCommandManagerOption* option);

private:
	xResult GetOutputFileName(const xChar* path, const xChar* extension, xChar** fileName);
	xResult UploadFile(const xChar* fileName, const xChar* url);
};

