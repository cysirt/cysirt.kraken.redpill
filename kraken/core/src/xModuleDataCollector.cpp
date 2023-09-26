#include "xModuleDataCollector.h"

#include "xLog.h"

#include "xDataCache.h"
#include "xFileDataCollector.h"
#include "xFileVersionDataCollector.h"
#include "xImportTableDataCollector.h"
#include "xModuleData.h"
#include "xSignatureDataCollector.h"



#define X_DATA_COLLECTOR_MAX_PROCESS		4096
#define X_DATA_COLLECTOR_MAX_MODULE			4096



xModuleDataCollector::xModuleDataCollector()
{

}

xModuleDataCollector::~xModuleDataCollector()
{

}

xResult xModuleDataCollector::Collect(const xChar* fileName, IxModuleData** moduleData)
{
	X_ASSERT(fileName && moduleData);

	if (!fileName || !moduleData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxModuleData, &iface);

	if (xSucceeded(r))
	{
		return iface->QueryInterface(IID_IxModuleData, (void**) moduleData);
	}
	else
	{
		xInterfacePtr<xModuleData> md(new xModuleData);

		if (!md)
		{
			return X_E_OUT_OF_MEMORY;
		}

		xFileDataCollector fdc;

		xInterfacePtr<IxFileData> fd;

		if (xSucceeded(r = fdc.Collect(fileName, &fd)))
		{
			md->SetFileData(fd);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xFileVersionDataCollector fvdc;

		xInterfacePtr<IxFileVersionData> fvd;

		if (xSucceeded(r = fvdc.Collect(fileName, &fvd)))
		{
			md->SetFileVersionData(fvd);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file version information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xImportTableDataCollector itdc;

		xInterfacePtr<IxImportTableDataEnum> itde;

		if (xSucceeded(r = itdc.Collect(fileName, &itde)))
		{
			md->SetImportTableData(itde);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get import table information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xSignatureDataCollector sdc;

		xInterfacePtr<IxSignatureData> sd;

		if (xSucceeded(r = sdc.Collect(fileName, &sd)))
		{
			md->SetSignatureData(sd);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file signature information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xDataCacheGet().Add(fileName, IID_IxModuleData,  md);

		*moduleData = md.Detach();

		return r;
	}
}

