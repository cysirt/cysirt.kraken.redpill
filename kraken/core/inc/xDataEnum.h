#pragma once

#include "xPropertyEnum.h"

#include "xCoreInterface.h"

// HACK
#include "xPrefetchItemData.h"



#define X_DECLARE_DATA_ENUM_BEGIN(__Class) \
	class __Class##Enum : public xPropertyEnum<I##__Class##Enum> \
	{ \
	typedef xPropertyEnum<I##__Class##Enum> _Base; \
	X_INTERFACE_MAP_BEGIN \
		X_INTERFACE_MAP_ENTRY(I##__Class##Enum) \
		X_INTERFACE_MAP_CHAIN(_Base) \
	X_INTERFACE_MAP_END \
	public: \
		__Class##Enum() \
		{ \
		} \
		virtual ~__Class##Enum() \
		{ \
		} \
		X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, I##__Class** data) \
		{ \
			return xInterfaceEnum<I##__Class##Enum>::Get(index, IID_I##__Class, (IxInterface**) data); \
		}

#define X_DECLARE_DATA_ENUM_END \
	};



X_DECLARE_DATA_ENUM_BEGIN(xNetworkConnectionData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xDnsCacheData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xDriverData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xImportTableData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xModuleData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xPrefetchData)
	X_INTERFACE_METHOD(bool, Contains)(const xChar* fileName, IxPrefetchItemData** prefetchItemData);
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xPrefetchItemData)
	X_INTERFACE_METHOD(bool, Contains)(const xChar* fileName, IxPrefetchItemData** prefetchItemData);
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xProcessData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xRegistryKeyData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xRegistryValueData)
X_DECLARE_DATA_ENUM_END

X_DECLARE_DATA_ENUM_BEGIN(xUserAccountData)
X_DECLARE_DATA_ENUM_END
