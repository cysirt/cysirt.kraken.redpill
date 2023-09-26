#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xHashData : public xPropertyEnum<IxHashData>
{
typedef xPropertyEnum<IxHashData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxHashData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Md5, xChar*)
	X_PROPERTY_MAP_SET_GET(Sha1, xChar*)
	X_PROPERTY_MAP_SET_GET(Sha256, xChar*)
X_PROPERTY_MAP_END

public:
	xHashData()
	{
	}

	virtual ~xHashData()
	{
	}
};
