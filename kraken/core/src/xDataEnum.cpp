#include "xDataEnum.h"



bool xPrefetchDataEnum::Contains(const xChar* fileName, IxPrefetchItemData** prefetchItemData)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return false;
	}

	const uint32_t count = GetCount();

	for (uint32_t ui = 0; ui < count; ui++)
	{
		xInterfacePtr<IxPrefetchData> pd;

		xResult r = Get(ui, &pd);

		if (xFailed(r))
		{
			return false;
		}

		xInterfacePtr<IxPrefetchItemDataEnum> pide;

		if (xFailed(r = pd->GetItemData(&pide)))
		{
			continue;
		}

		if (pide->Contains(fileName, prefetchItemData))
		// if (reinterpret_cast<xPrefetchItemDataEnum*>(pide.GetSafeType())->Contains(fileName, prefetchItemData))
		{
			return true;
		}
	}

	return false;
}

bool xPrefetchItemDataEnum::Contains(const xChar* fileName, IxPrefetchItemData** prefetchItemData)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return false;
	}

	const uint32_t count = GetCount();

	for (uint32_t ui = 0; ui < count; ui++)
	{
		xInterfacePtr<IxPrefetchItemData> pid;

		xResult r = Get(ui, &pid);

		if (xFailed(r))
		{
			return false;
		}

		xPtr<xChar> nativeFileName;

		if (xFailed(pid->GetNativeFileName(&nativeFileName)))
		{
			continue;
		}

		if (xCharCompareNoCase(fileName, nativeFileName) == 0)
		{
			if (prefetchItemData)
			{
				//*prefetchItemData = reinterpret_cast<xPrefetchItemData*>(pid.Detach());
				*prefetchItemData = pid.Detach();
			}

			return true;
		}
	}

	return false;
}
