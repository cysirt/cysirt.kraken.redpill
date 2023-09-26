#pragma once

#include "xBaseInterface.h"
#include "xReferenceCount.h"



template
<
	typename t_Type
>
class xFunctor : public xRefCount<IxFunctor>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxFunctor)
X_INTERFACE_MAP_END

public:
	xFunctor()
	{
		m_type = NULL;

		m_pfn = NULL;

		m_pfnData = NULL;
	}

	explicit xFunctor(t_Type* type, xResult(t_Type::*pfn)())
	{
		X_ASSERT(type && pfn);

		if (!type || !pfn)
		{
			return;
		}

		Set(type, pfn);
	}

	explicit xFunctor(t_Type* type, xResult(t_Type::*pfn)(void*))
	{
		X_ASSERT(type && pfn);

		if (!type || !pfn)
		{
			return;
		}

		Set(type, pfn);
	}

	~xFunctor()
	{
		Detach(m_type);
	}

	xResult Set(t_Type* type, xResult(t_Type::*pfn)())
	{
		X_ASSERT(type && pfn);

		if (!type || !pfn)
		{
			return X_E_INVALID_ARGUMENT;
		}

		if (m_type)
		{
			Detach(m_type);
		}

		Attach(type);

		m_pfn = pfn;

		return X_S_OK;
	}

	xResult Set(t_Type* type, xResult(t_Type::*pfn)(void*))
	{
		X_ASSERT(type && pfn);

		if (!type || !pfn)
		{
			return X_E_INVALID_ARGUMENT;
		}

		if (m_type)
		{
			Detach(m_type);
		}

		Attach(type);

		m_pfnData = pfn;

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Call)()
	{
		X_ASSERT(m_type && m_pfn);

		if (!m_type || !m_pfn)
		{
			return X_E_UNEXPECTED;
		}

		return (m_type->*m_pfn)();
	}

	X_INTERFACE_METHOD(xResult, Call)(void* data)
	{
		X_ASSERT(m_type && m_pfn);

		if (!m_type || !m_pfn)
		{
			return X_E_UNEXPECTED;
		}

		return (m_type->*m_pfnData)(data);
	}

private:
	void Attach(t_Type* type)
	{
		m_type = type;
	}

	void Attach(IxInterface* iface)
	{
		X_ASSERT(iface);

		if (iface)
		{
			(m_type = iface)->AddRef();
		}
	}

	void Detach(t_Type* type)
	{
		m_type = NULL;
	}

	void Detach(IxInterface* iface)
	{
		X_ASSERT(iface);

		if (iface)
		{
			iface->Release();
		}
	}

private:
	t_Type* m_type;
	xResult(t_Type::*m_pfn)();
	xResult(t_Type::*m_pfnData)(void*);
};
