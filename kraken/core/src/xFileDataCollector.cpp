#include "xFileDataCollector.h"

#include "xLog.h"
#include "xPath.h"

#include "xDataCache.h"
#include "xFileData.h"
#include "xHashDataCollector.h"
#include "xTimeDataCollector.h"
#include "xTimeData.h"



xFileDataCollector::xFileDataCollector()
{

}

xFileDataCollector::~xFileDataCollector()
{

}

xResult xFileDataCollector::Collect(const xChar* fileName, IxFileData** fileData)
{
	X_ASSERT(fileName && fileData);

	if (!fileName || !fileData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxFileData, &iface);

	if (xSucceeded(r))
	{
		return iface->QueryInterface(IID_IxFileData, (void**) fileData);
	}
	else
	{
		xInterfacePtr<xFileData> fd(new xFileData);

		if (!fd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		xString fn(fileName);

		const int32_t i = fn.ReverseFind(X_PATH_SEPARATOR);

		fd->SetName((i != -1) ? fn.Right(fn.GetLength() - i - 1) : fn);

		fd->SetFileName(fileName);

		uint64_t size = 0;

		if (xSucceeded(r = GetFileSizeData(fileName, &size)))
		{
			fd->SetSize(size);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file size information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xTimeDataCollector tdc;

		xInterfacePtr<IxTimeData> td;

		if (xSucceeded(r = tdc.Collect(fileName, &td)))
		{
			fd->SetTimeData(td);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file time information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xHashDataCollector hdc;

		xInterfacePtr<IxHashData> hd;

		if (xSucceeded(r = hdc.Collect(fileName, &hd)))
		{
			fd->SetHashData(hd);
		}
		else
		{
			X_LOG_WARNING(L"Failed to get file hash information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileName, xResultFromLastError());
		}

		xDataCacheGet().Add(fileName, IID_IxFileData,  fd);
		
		*fileData = fd.Detach();
	}

	return r;
}

xResult xFileDataCollector::GetFileSizeData(const xChar* fileName, uint64_t* size)
{
	X_ASSERT(fileName && size);

	if (!fileName || !size)
	{
		return X_E_INVALID_ARGUMENT;
	}

	WIN32_FILE_ATTRIBUTE_DATA fad;

	if (!GetFileAttributesEx(fileName, GetFileExInfoStandard, &fad))
	{
		return xResultFromLastError();
	}

	*size = (static_cast<uint64_t>(fad.nFileSizeHigh) << (8 * sizeof(fad.nFileSizeLow))) | fad.nFileSizeLow;

	return X_S_OK;
}
