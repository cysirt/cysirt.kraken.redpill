#include "xTimeDataCollector.h"

#include "xConversion.h"
#include "xLog.h"

#include "xDataCache.h"
#include "xTimeData.h"



xTimeDataCollector::xTimeDataCollector()
{

}

xTimeDataCollector::~xTimeDataCollector()
{

}

xResult xTimeDataCollector::Collect(const xChar* fileName, IxTimeData** timeData)
{
	X_ASSERT(fileName && timeData);

	if (!fileName || !timeData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxTimeData, &iface);

	if (xSucceeded(r))
	{
		return iface->QueryInterface(IID_IxTimeData, (void**) timeData);
	}
	else
	{
		WIN32_FILE_ATTRIBUTE_DATA fad;

		if (!GetFileAttributesEx(fileName, GetFileExInfoStandard, &fad))
		{
			return xResultFromLastError();
		}

		xInterfacePtr<xTimeData> td(new xTimeData);

		if (!td)
		{
			return X_E_OUT_OF_MEMORY;
		}

		td->SetCreated(xConvertFileTimeToTime(fad.ftCreationTime));

		td->SetLastModified(xConvertFileTimeToTime(fad.ftLastWriteTime));

		td->SetLastAccessed(xConvertFileTimeToTime(fad.ftLastAccessTime));

		xDataCacheGet().Add(fileName, IID_IxTimeData, td);

		*timeData = td.Detach();
	}

	return X_S_OK;
}