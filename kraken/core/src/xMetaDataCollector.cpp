#include "xMetaDataCollector.h"

#include <time.h>

#include "xElevation.h"
#include "xGuid.h"
#include "xMetaData.h"



#define X_META_DATA_COLLECTOR_VERSION_1			1

#define X_META_DATA_COLLECTOR_VERSION_CURRENT	X_META_DATA_COLLECTOR_VERSION_1



xMetaDataCollector::xMetaDataCollector()
{

}

xMetaDataCollector::~xMetaDataCollector()
{

}

xResult xMetaDataCollector::Collect(IxMetaData** metaData)
{
	X_ASSERT(metaData);

	if (!metaData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xMetaData> md(new xMetaData);

	if (!md)
	{
		return X_E_OUT_OF_MEMORY;
	}

	md->SetCreated(time(NULL));

	md->SetElevated(xElevationGet().IsElevated());

	xGuid guid;

	if (xGuidCreate(&guid))
	{
		md->SetGuid(guid);
	}

	md->SetVersion(X_META_DATA_COLLECTOR_VERSION_1);

	*metaData = md.Detach();

	return X_S_OK;
}
