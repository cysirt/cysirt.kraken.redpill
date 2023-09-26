#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"

class xPrefetchData;
class xPrefetchDataEnum;

typedef intptr_t libscca_file_t;
typedef intptr_t libscca_volume_information_t;



class xPrefetchDataCollector: public xRefCount<IxPrefetchDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxPrefetchDataCollector)
X_INTERFACE_MAP_END

public:
	xPrefetchDataCollector();
	virtual ~xPrefetchDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxPrefetchDataEnum** prefetchDataEnum);

private:
	xResult GetPrefetchData(const xChar* fileName, xPrefetchDataEnum* prefetchDataEnum, IxPrefetchData** prefetchData);
	xResult GetPrefetchData(libscca_file_t* file, const xChar* fileName, xPrefetchDataEnum* prefetchDataEnum, xPrefetchData* prefetchData);
	xResult GetWin32FileNameFromNtFileName(const xChar* ntFileName, xChar** win32FileName);
	xResult GetWin32FileNameFromPrefetchGuidFileName(const xChar* guidFileName, libscca_file_t* file, xChar** win32FileName);
	xResult GetDriveLetterForVolume(const uint32_t serial, xChar** driveLetter);
};

