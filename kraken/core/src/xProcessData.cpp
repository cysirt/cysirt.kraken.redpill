#include "xProcessData.h"

xProcessData::xProcessData() : m_modules(new xModuleDataEnum)
{
}

xProcessData::~xProcessData()
{
}

xResult xProcessData::GetModules(IxModuleDataEnum** moduleDataEnum)
{
	X_ASSERT(moduleDataEnum);

	if (!moduleDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	return m_modules ? ((*moduleDataEnum = m_modules)->AddRef(), X_S_OK) : X_E_OUT_OF_MEMORY;
}

/*
xResult xProcessData::Add(IxModuleData* moduleData)
{	X_ASSERT(moduleData);

	if (!moduleData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	return m_modules ? m_modules->Add(moduleData) : X_E_OUT_OF_MEMORY;
}
*/