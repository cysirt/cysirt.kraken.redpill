#include "xDataCache.h"

#include "xCriticalSection.h"

#include "xCoreInterface.h"



xDataCache::xDataCache()
{
}

xDataCache::~xDataCache()
{
}

xDataCache& xDataCache::Get()
{
	static xDataCache s_self;

	return s_self;
}

xResult xDataCache::Add(const xChar* key, const xGuid& iid, IxInterface* iface)
{
	X_ASSERT(key && iface);

	if (!key || !iface)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xObjectPtr<xDataCacheItem> dci(new xDataCacheItem);

	if (!dci)
	{
		return X_E_OUT_OF_MEMORY;
	}

	dci->iid = iid;

	dci->key = key;

	dci->iface = iface;

	m_cache.Add(dci);

	dci.Detach();

	return X_S_OK;
}

xResult xDataCache::Contains(const xChar* key, const xGuid& iid, IxInterface** iface)
{
	X_ASSERT(key && iface);

	if (!key || !iface)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xCriticalSectionLock lock;

	const uint32_t count = m_cache.GetCount();

	for (uint32_t ui = 0; ui < count; ui++)
	{
		xDataCacheItem* dci = m_cache[ui];

		if (xGuidIsEqual(dci->iid, iid) && dci->key.CompareNoCase(key) == 0)
		{
			(*iface = dci->iface)->AddRef();

			return X_S_OK;
		}
	}

	return X_E_NOT_PRESENT;
}
