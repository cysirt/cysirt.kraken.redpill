#pragma once

#include "xDataEnum.h"
#include "xModuleData.h"



class xProcessData : public xModuleDataBase<IxProcessData>
{
typedef xModuleDataBase<IxProcessData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxProcessData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Id, uint32_t)
	X_PROPERTY_MAP_SET_GET(Subsystem, xChar*)
	X_PROPERTY_MAP_SET_GET(ParentId, uint32_t)
	X_PROPERTY_MAP_SET_GET(Parent, xChar*)
	X_PROPERTY_MAP_SET_GET_INTERFACE(ModuleData, IxModuleDataEnum)
X_PROPERTY_MAP_END

public:
	xProcessData()
	{
	}

	virtual ~xProcessData()
	{
	}
};

