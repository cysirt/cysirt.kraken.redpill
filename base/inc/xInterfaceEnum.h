#pragma once

#include "xArray.h"
#include "xBaseInterface.h"
#include "xReferenceCount.h"



template
<
	typename t_Base,
	typename t_Interface = IxInterface,
	typename t_Cast = t_Base
>
class xInterfaceEnum : public xRefCount<t_Base>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxInterfaceEnum)
X_INTERFACE_MAP_END

public:
	xInterfaceEnum()
	{
	}

	virtual ~xInterfaceEnum()
	{
	}

public:
	X_INTERFACE_METHOD(uint32_t, GetCount)()
	{
		return m_arr.GetCount();
	}

	X_INTERFACE_METHOD(xResult, Delete)(const uint32_t index)
	{
		const uint32_t size = m_arr.GetCount();

		X_ASSERT(index < size);

		if (index >= size)
		{
			return X_E_INVALID_ARGUMENT;
		}

		m_arr.RemoveAt(index);

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, DeleteAll)()
	{
		m_arr.RemoveAll();

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxInterface** iface)
	{
		const uint32_t size = m_arr.GetCount();

		X_ASSERT(index < size);

		if (index >= size)
		{
			return X_E_INVALID_ARGUMENT;
		}

		(*iface = m_arr[index])->AddRef();

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, const xGuid& iid, IxInterface** iface)
	{
		X_ASSERT(iface);

		if (!iface)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xInterfacePtr<IxInterface> temp;

		xResult r = Get(index, &temp);

		if (xFailed(r))
		{
			return r;
		}

		return temp->QueryInterface(iid, (void**) iface);
	}

	X_INTERFACE_METHOD(xResult, Add)(IxInterface* iface)
	{
		X_ASSERT(iface);

		if (!iface)
		{
			return X_E_INVALID_ARGUMENT;
		}

		m_arr.Add(iface);

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Add)(IxInterfaceEnum* ifaceEnum, const bool deep)
	{
		X_ASSERT(ifaceEnum);

		if (!ifaceEnum)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xResult r = X_S_OK;

		if (deep)
		{
			const uint32_t count = ifaceEnum->GetCount();

			for (uint32_t ui = 0; ui < count; ui++)
			{
				xInterfacePtr<IxInterface> temp;

				if (xFailed(r = ifaceEnum->Get(ui, &temp)) ||
					xFailed(r = Add(temp)))
				{
					return r;
				}
			}
		}
		else
		{
			r = Add((IxInterface*) ifaceEnum);
		}

		return r;
	}

private:
	xArray<xInterfacePtr<IxInterface> > m_arr;
};
