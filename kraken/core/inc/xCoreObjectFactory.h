#pragma once

#include "xCoreInterface.h"



class xCoreObjectFactory
{
private:
	xCoreObjectFactory();

public:
	~xCoreObjectFactory();

	xResult GetCore(IxCore** core);

	static xCoreObjectFactory& Get();
};

static xCoreObjectFactory& xCoreObjectFactoryGet() { return xCoreObjectFactory::Get(); }
