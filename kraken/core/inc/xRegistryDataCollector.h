#pragma once

#include <Windows.h>

#include "xReferenceCount.h"

#include "xCoreInterface.h"

class xRegistryValueData;



class xRegistryDataCollector : public xRefCount<IxRegistryDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxRegistryDataCollector)
X_INTERFACE_MAP_END

public:
	xRegistryDataCollector();
	virtual ~xRegistryDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxRegistryKeyDataEnum** IxRegistryKeyDataEnum);

private:
	xResult Collect(HKEY rootKey, const xChar* path, IxRegistryKeyData** registryKeyData);
	xResult Collect(HKEY key, const xChar* path, const xChar* name, IxRegistryValueData** registryValueData);
	xResult Collect(xRegistryValueData* registryValueData, const xChar* fileName);
	xResult GetRootKeyString(HKEY rootKey, xChar** rootKeyString);

	//xResult Collect(HKEY rootHive, const xChar* path, void** registryData);
	//xResult Collect(HKEY key, uint32_t count);
};

