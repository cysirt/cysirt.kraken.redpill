#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xDnsCacheDataCollector : public xRefCount<IxDnsCacheDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxDnsCacheDataCollector)
X_INTERFACE_MAP_END

public:
	xDnsCacheDataCollector();
	virtual ~xDnsCacheDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool resolve);

private:
	xResult GetDnsCacheDataDnsCache(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool resolve);
	xResult GetDnsCacheDataHosts(IxDnsCacheDataEnum** dnsCacheDataEnum);
};

