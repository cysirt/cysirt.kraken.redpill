#include "xCoreObjectFactory.h"

#include "xAssert.h"

#include "xCore.h"



xCoreObjectFactory::xCoreObjectFactory()
{
}

xCoreObjectFactory::~xCoreObjectFactory()
{
}

xResult xCoreObjectFactory::GetCore(IxCore** core)
{
	X_ASSERT(core);

	if (!core)
	{
		return X_E_INVALID_ARGUMENT;
	}

	IxCore* temp = new xCore;

	return temp ? (*core = temp), X_S_OK : X_E_OUT_OF_MEMORY;
}

xCoreObjectFactory& xCoreObjectFactory::Get()
{
	static xCoreObjectFactory s_self;

	return s_self;
}
