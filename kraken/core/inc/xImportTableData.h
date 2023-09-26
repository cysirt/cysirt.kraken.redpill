#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xImportTableData : public xPropertyEnum<IxImportTableData>
{
typedef xPropertyEnum<IxImportTableData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxImportTableData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(ModuleName, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(Functions, IxStringEnum)
X_PROPERTY_MAP_END

public:
	xImportTableData()
	{
	}

	virtual ~xImportTableData()
	{
	}
};
