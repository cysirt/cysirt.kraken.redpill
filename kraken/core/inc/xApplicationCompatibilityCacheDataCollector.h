#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xApplicationCompatibilityCacheDataCollector : public xRefCount<IxApplicationCompatibilityCacheDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxApplicationCompatibilityCacheDataCollector)
X_INTERFACE_MAP_END

public:
	xApplicationCompatibilityCacheDataCollector();
	virtual ~xApplicationCompatibilityCacheDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);

private:
	xResult Collect(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	xResult CollectWindowsVistaWindowsServer2003WindowsServer2008(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	xResult CollectWindows7(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	xResult CollectWindows8WindowsServer2012(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	xResult CollectWindows81WindowsServer2012R2(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
	xResult CollectWindows10(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData);
};

