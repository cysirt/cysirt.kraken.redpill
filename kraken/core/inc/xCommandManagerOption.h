#pragma once

#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xCommandManagerOption : public xPropertyEnum<IxCommandManagerOption>
{
typedef xPropertyEnum<IxCommandManagerOption> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxCommandManagerOption)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Flags, uint32_t)
	X_PROPERTY_MAP_SET_GET(CollectFlags, uint32_t)
	X_PROPERTY_MAP_SET_GET(LogMode, uint32_t)
	X_PROPERTY_MAP_SET_GET(LogFileName, xChar*)
	X_PROPERTY_MAP_SET_GET(OutputFileName, xChar*)
	X_PROPERTY_MAP_SET_GET(UploadUrl, xChar*)
X_PROPERTY_MAP_END

public:
	xCommandManagerOption()
	{
	}

	virtual ~xCommandManagerOption()
	{
	}
};

