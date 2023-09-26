#pragma once

#include "xInterfaceEnum.h"
#include "xString.h"



class xDataCache
{
private:
	xDataCache();

public:
	~xDataCache();

	static xDataCache& Get();

	xResult Add(const xChar* key, const xGuid& iid, IxInterface* iface);
	xResult Contains(const xChar* key, const xGuid& iid, IxInterface** iface);

private:
	typedef struct _xDataCacheItem
	{
		xGuid iid;
		xString key;
		xInterfacePtr<IxInterface> iface;
	} xDataCacheItem;

	xArray<xObjectPtr<xDataCacheItem> > m_cache;
};

static xDataCache& xDataCacheGet() { return xDataCache::Get(); }
