#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xDataCollector : public xRefCount<IxDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDataCollector)
X_INTERFACE_MAP_END

public:
	xDataCollector();
	virtual ~xDataCollector();

	X_INTERFACE_METHOD(xResult, CollectApplicationCompatibilityCacheData)(IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	X_INTERFACE_METHOD(xResult, CollectDnsCacheData)(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline);
	X_INTERFACE_METHOD(xResult, CollectDriverData)(IxDriverDataEnum** driverDataEnum);
	X_INTERFACE_METHOD(xResult, CollectFileVersionData)(const xChar* fileName, IxFileVersionData** fileVersionData);
	X_INTERFACE_METHOD(xResult, CollectMetaData)(IxMetaData** metaData);
	X_INTERFACE_METHOD(xResult, CollectNetworkConnectionData)(IxNetworkConnectionDataEnum** networkConnectionDataEnum);
	X_INTERFACE_METHOD(xResult, CollectOperatingSystemData)(IxOperatingSystemData** operatingSystemData);
	X_INTERFACE_METHOD(xResult, CollectPrefetchData)(IxPrefetchDataEnum** prefetchDataEnum);
	X_INTERFACE_METHOD(xResult, CollectProcessData)(IxProcessDataEnum** processDataEnum);
	X_INTERFACE_METHOD(xResult, CollectRegistryData)(IxRegistryKeyDataEnum** registryKeyDataEnum);
	X_INTERFACE_METHOD(xResult, CollectUserAccountData)(IxUserAccountDataEnum** userAccountDataEnum);
};

