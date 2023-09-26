#pragma once

#include <cguid.h>
#include <Objbase.h>
#include <Rpc.h>



typedef GUID xGuid;

#define xGuidNull GUID_NULL;


static bool xGuidCreate(xGuid* guid)
{
	GUID temp;

	if (FAILED(CoCreateGuid((GUID*) &temp)))
	{
		return false;
	}

	temp.Data1 = 0x666dc0de;

	*guid = temp;

	return true;
}

static bool xGuidIsEqual(const xGuid& guid1, const xGuid& guid2)
{
	return !!IsEqualGUID(guid1, guid2);
}

static bool xGuidToString(const xGuid& guid, xChar** guidString)
{
	BSTR bstr = NULL;

	if (SUCCEEDED(StringFromCLSID(guid, &bstr)))
	{
		xChar* s = NULL;

		s = xCharClone(bstr);

		CoTaskMemFree(bstr);

		if (s)
		{
			*guidString = s;
		}

		return true;
	}

	return false;
}
