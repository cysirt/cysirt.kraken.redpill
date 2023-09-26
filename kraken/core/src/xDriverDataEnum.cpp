#include "xDriverDataEnum.h"



xDriverDataEnum::xDriverDataEnum()
{
}


xDriverDataEnum::~xDriverDataEnum()
{
}

xResult xDriverDataEnum::Get(const uint32_t index, IxDriverData** driverData)
{
	return xInterfaceEnum<IxDriverDataEnum>::Get(index, IID_IxDriverData, (IxInterface**) driverData);
}
