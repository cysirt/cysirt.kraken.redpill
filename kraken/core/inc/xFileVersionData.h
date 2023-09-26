#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xFileVersionData : public xPropertyEnum<IxFileVersionData>
{
typedef xPropertyEnum<IxFileVersionData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxFileVersionData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Comments, xChar*)
	X_PROPERTY_MAP_SET_GET(InternalName, xChar*)
	X_PROPERTY_MAP_SET_GET(ProductName, xChar*)
	X_PROPERTY_MAP_SET_GET(CompanyName, xChar*)
	X_PROPERTY_MAP_SET_GET(LegalCopyright, xChar*)
	X_PROPERTY_MAP_SET_GET(ProductVersion, xChar*)
	X_PROPERTY_MAP_SET_GET(FileDescription, xChar*)
	X_PROPERTY_MAP_SET_GET(LegalTrademarks, xChar*)
	X_PROPERTY_MAP_SET_GET(PrivateBuild, xChar*)
	X_PROPERTY_MAP_SET_GET(FileVersion, xChar*)
	X_PROPERTY_MAP_SET_GET(OriginalFileName, xChar*)
	X_PROPERTY_MAP_SET_GET(SpecialBuild, xChar*)
X_PROPERTY_MAP_END

public:
	xFileVersionData()
	{
	}

	virtual ~xFileVersionData()
	{
	}
};
