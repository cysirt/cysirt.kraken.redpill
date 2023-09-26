#pragma once

#include "xCorePropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xTimeData : public xCorePropertyEnum<IxTimeData>
{
typedef xCorePropertyEnum<IxTimeData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxTimeData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(Created)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(LastModified)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(LastAccessed)
X_PROPERTY_MAP_END

public:
	xTimeData()
	{
	}

	virtual ~xTimeData()
	{
	}
};
