#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xFileData : public xPropertyEnum<IxFileData>
{
typedef xPropertyEnum<IxFileData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxFileData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Name, xChar*)
	X_PROPERTY_MAP_SET_GET(FileName, xChar*)
	X_PROPERTY_MAP_SET_GET(Size, uint64_t)
	X_PROPERTY_MAP_SET_GET_INTERFACE(TimeData, IxTimeData)
	X_PROPERTY_MAP_SET_GET_INTERFACE(HashData, IxHashData)
X_PROPERTY_MAP_END

public:
	xFileData()
	{
	}

	virtual ~xFileData()
	{
	}
};
