#include "xCore.h"

#include "xCommandManager.h"
#include "xCommandManagerOption.h"
#include "xDataCollector.h"
#include "xElevation.h"
#include "xLog.h"



xCore::xCore()
{
}

xCore::~xCore()
{
}

xResult xCore::Init()
{
	return X_S_OK;
}

xResult xCore::Shutdown()
{
	return X_S_OK;
}

xResult xCore::AddLogWriter(IxLogWriter* log)
{
	return xLogGet().AddLogWriter(log);
}

xResult xCore::CreateCommandManagerOption(IxCommandManagerOption** commandManagerOption)
{
	X_ASSERT(commandManagerOption);

	if (!commandManagerOption)
	{
		return X_E_INVALID_ARGUMENT;
	}

	return (*commandManagerOption = new xCommandManagerOption) ? X_S_OK : X_E_OUT_OF_MEMORY;
}

xResult xCore::CreateCommandManager(IxCommandManager** commandManager)
{
	X_ASSERT(commandManager);

	if (!commandManager)
	{
		return X_E_INVALID_ARGUMENT;
	}

	return (*commandManager = new xCommandManager) ? X_S_OK : X_E_OUT_OF_MEMORY;
}


xResult xCore::CreateDataCollector(IxDataCollector** dataCollector)
{
	X_ASSERT(dataCollector);

	if (!dataCollector)
	{
		return X_E_INVALID_ARGUMENT;
	}

	return (*dataCollector = new xDataCollector) ? X_S_OK : X_E_OUT_OF_MEMORY;
}
