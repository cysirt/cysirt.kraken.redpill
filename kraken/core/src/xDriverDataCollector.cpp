#include "xDriverDataCollector.h"

#include <Psapi.h>

#include "xLog.h"
#include "xPath.h"

#include "xDataEnum.h"
#include "xDriverData.h"
#include "xFileData.h"
#include "xFileDataCollector.h"
#include "xFileVersionDataCollector.h"
#include "xSignatureDataCollector.h"



#define X_DRIVER_DATA_COLLECTOR_MAX_DRIVER	1024



xDriverDataCollector::xDriverDataCollector()
{

}

xDriverDataCollector::~xDriverDataCollector()
{

}

xResult xDriverDataCollector::Collect(IxDriverDataEnum** driverDataEnum)
{
	X_ASSERT(driverDataEnum);

	if (!driverDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xDriverDataEnum> dde(new xDriverDataEnum);

	if (!dde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	LPVOID drivers[X_DRIVER_DATA_COLLECTOR_MAX_DRIVER] = { 0 };

	DWORD needed = 0;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &needed) && needed < sizeof(drivers))
	{
		const uint32_t count = needed / sizeof(drivers[0]);

		for (uint32_t ui = 0; ui < count; ui++)
		{
			xString fileName;

			DWORD dw = GetDeviceDriverFileName(drivers[ui], fileName.GetBuffer(X_PATH_MAX), X_PATH_MAX);

			fileName.ReleaseBuffer();

			if (dw)
			{
				xInterfacePtr<xDriverData> dd(new xDriverData);

				if (!dd)
				{
					return X_E_OUT_OF_MEMORY;
				}

				dd->SetNativeFileName(fileName);

				xPtr<xChar> fileNameWin32;

				xFileDataCollector fdc;

				xInterfacePtr<IxFileData> fd;

				if (xSucceeded(xPathGetWin32FileNameFromKernelFileName(fileName, &fileNameWin32)) &&
					xSucceeded(r = fdc.Collect(fileNameWin32, &fd)))
				{
					dd->SetFileData(fd);
				}
				else
				{
					X_LOG_WARNING(L"Failed to get file information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileNameWin32, xResultFromLastError());
				}

				xFileVersionDataCollector fvdc;

				xInterfacePtr<IxFileVersionData> fvd;

				if (xSucceeded(r = fvdc.Collect(fileNameWin32, &fvd)))
				{
					dd->SetFileVersionData(fvd);
				}
				else
				{
					X_LOG_WARNING(L"Failed to get file version information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileNameWin32, xResultFromLastError());
				}

				xSignatureDataCollector sdc;

				xInterfacePtr<IxSignatureData> sd;

				if (xSucceeded(sdc.Collect(fileNameWin32, &sd)))
				{
					dd->SetSignatureData(sd);
				}
				else
				{
					X_LOG_WARNING(L"Failed to get file signature information for \"%s\" (code: " X_RESULT_FORMAT_STRING L")", fileNameWin32, xResultFromLastError());
				}

				dde->Add(dd);
			}
		}
	}
	else
	{
		r = xResultFromLastError();

		X_LOG_WARNING(L"Failed to enumerate drivers (code: "  X_RESULT_FORMAT_STRING L")", r);

		X_LOG_DEBUG_LOCATION;
	}

	r = X_S_OK;

	*driverDataEnum = dde.Detach();

	return r;
}

