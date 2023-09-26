#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xOperatingSystemDataCollector : public xRefCount<IxOperatingSystemDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxOperatingSystemDataCollector)
X_INTERFACE_MAP_END

public:
	xOperatingSystemDataCollector();
	virtual ~xOperatingSystemDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxOperatingSystemData** operatingSystemData);

private:
	const xChar* GetOperatingSystemSkuString(const uint32_t operatingSystemSku);
	const xChar* GetProductTypeString(const uint32_t productType);
};

