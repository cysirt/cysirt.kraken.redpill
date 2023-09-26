#pragma once

#include "xInterfaceEnum.h"
#include "xCoreInterface.h"


/*
class xNetworkAdapterDataEnum : public xInterfaceEnum<IxNetworkAdapterDataEnum>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxNetworkAdapterDataEnum)
X_INTERFACE_MAP_END

public:
	xNetworkAdapterDataEnum()
	{
	}

	~xNetworkAdapterDataEnum()
	{
	}

	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxNetworkConnectionData** ipAddressData)
	{
		return xInterfaceEnum<IxNetworkAdapterDataEnum>::Get(index, IID_IxNetworkAdapterData, (IxInterface**) ipAddressData);
	}
};
*/