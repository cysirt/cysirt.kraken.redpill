#include "xDataCollector.h"

#include "xApplicationCompatibilityCacheDataCollector.h"
#include "xDnsCacheDataCollector.h"
#include "xDriverDataCollector.h"
#include "xFileVersionDataCollector.h"
#include "xMetaDataCollector.h"
#include "xNetworkConnectionDataCollector.h"
#include "xOperatingSystemDataCollector.h"
#include "xPrefetchDataCollector.h"
#include "xProcessDataCollector.h"
#include "xRegistryDataCollector.h"
#include "xUserAccountDataCollector.h"



xDataCollector::xDataCollector()
{
	//xApplicationCompatibilityCacheDataCollector accdc;

	//accdc.Collect((IxApplicationCompatibilityCacheData**) 0xFF);
}

xDataCollector::~xDataCollector()
{
}

xResult xDataCollector::CollectApplicationCompatibilityCacheData(IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	X_ASSERT(applicationCompatibilityCacheData);

	if (!applicationCompatibilityCacheData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xApplicationCompatibilityCacheDataCollector accdc;

	return accdc.Collect(applicationCompatibilityCacheData);
}

xResult xDataCollector::CollectDnsCacheData(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline)
{
	X_ASSERT(dnsCacheDataEnum);

	if (!dnsCacheDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xDnsCacheDataCollector dcdc;

	return dcdc.Collect(dnsCacheDataEnum, offline);
}

xResult xDataCollector::CollectDriverData(IxDriverDataEnum** driverDataEnum)
{
	X_ASSERT(driverDataEnum);

	if (!driverDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xDriverDataCollector ddc;

	return ddc.Collect(driverDataEnum);
}

xResult xDataCollector::CollectFileVersionData(const xChar* fileName, IxFileVersionData** fileVersionData)
{
	X_ASSERT(fileName && fileVersionData);

	if (!fileName || !fileVersionData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xFileVersionDataCollector fvdc;

	return fvdc.Collect(fileName, fileVersionData);
}

xResult xDataCollector::CollectMetaData(IxMetaData** metaData)
{
	X_ASSERT(metaData);

	if (!metaData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xMetaDataCollector mdc;

	return mdc.Collect(metaData);
}

xResult xDataCollector::CollectNetworkConnectionData(IxNetworkConnectionDataEnum** networkConnectionDataEnum)
{
	X_ASSERT(networkConnectionDataEnum);

	if (!networkConnectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xNetworkConnectionDataCollector ncdc;

	return ncdc.Collect(networkConnectionDataEnum);
}

xResult xDataCollector::CollectPrefetchData(IxPrefetchDataEnum** prefetchDataEnum)
{
	X_ASSERT(prefetchDataEnum);

	if (!prefetchDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xPrefetchDataCollector pdc;

	return pdc.Collect(prefetchDataEnum);
}

xResult xDataCollector::CollectProcessData(IxProcessDataEnum** processDataEnum)
{
	X_ASSERT(processDataEnum);

	if (!processDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xProcessDataCollector pdc;

	return pdc.Collect(processDataEnum);
}

xResult xDataCollector::CollectRegistryData(IxRegistryKeyDataEnum** registryKeyDataEnum)
{
	X_ASSERT(registryKeyDataEnum);

	if (!registryKeyDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xRegistryDataCollector rdc;

	return rdc.Collect(registryKeyDataEnum);
}

xResult xDataCollector::CollectOperatingSystemData(IxOperatingSystemData** operatingSystemData)
{
	X_ASSERT(operatingSystemData);

	if (!operatingSystemData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xOperatingSystemDataCollector osdc;

	return osdc.Collect(operatingSystemData);
}

xResult xDataCollector::CollectUserAccountData(IxUserAccountDataEnum** userAccountDataEnum)
{
	X_ASSERT(userAccountDataEnum);

	if (!userAccountDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xUserAccountDataCollector uadc;

	return uadc.Collect(userAccountDataEnum);
}
