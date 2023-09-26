#pragma once

#include "xConversion.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xMetaData : public xCorePropertyEnum<IxMetaData>
{
typedef xPropertyEnum<IxMetaData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxMetaData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(Created)
	X_PROPERTY_MAP_SET_GET(Elevated, bool)
	X_PROPERTY_MAP_SET_GET_SPECIAL(Guid, const xGuid&, xGuid*)
	X_PROPERTY_MAP_SET_GET(Version, uint32_t)
X_PROPERTY_MAP_END

public:
	xMetaData()
	{
	}

	virtual ~xMetaData()
	{
	}

	X_INTERFACE_METHOD(xResult, GetGuidString)(xChar** guidString)
	{
		X_ASSERT(guidString);

		if (!guidString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xGuid guid;

		xResult r = GetGuid(&guid);

		if (xSucceeded(r))
		{
			xGuidToString(guid, guidString);
		}

		return (*guidString) ? r : X_E_OUT_OF_MEMORY;
	}
};
