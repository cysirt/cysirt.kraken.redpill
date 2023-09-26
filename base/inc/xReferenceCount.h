#pragma once

#include <stddef.h>
#include <stdint.h>

#include "xAssert.h"



template
<
	typename t_Traits
>
class xRefCountType
{
public:
	xRefCountType()
	{
		m_type = t_Traits::Invalid();
	}

	xRefCountType(typename t_Traits::Type type)
	{
		const bool valid = type != t_Traits::Invalid();

		X_ASSERT(valid);

		if (valid)
		{
			m_type = type;

			AddRef();
		}
		else
		{
			m_type = t_Traits::Invalid();
		}
	}

	xRefCountType(xRefCountType<t_Traits>& type)
	{
		const bool valid = type.m_type != t_Traits::Invalid();

		X_ASSERT(valid);

		if (valid)
		{
			m_type = type.Detach();

			AddRef();
		}
		else
		{
			m_type = t_Traits::Invalid();
		}
	}

	~xRefCountType()
	{
		Release();
	}

	typename t_Traits::Type GetSafeType()
	{
		X_ASSERT(m_type != t_Traits::Invalid());

		return m_type;
	}

	void Attach(typename t_Traits::Type type)
	{
		const bool valid = type != t_Traits::Invalid();

		X_ASSERT(valid);

		if (valid)
		{
			Release();

			m_type = type;
		}
	}

	typename t_Traits::Type Detach()
	{
		typename t_Traits::Type type = m_type;

		m_type = t_Traits::Invalid();
		
		return type;
	}

	uint32_t AddRef()
	{
		const bool valid = m_type != t_Traits::Invalid();

		X_ASSERT(valid);

		return valid ? t_Traits::AddRef(m_type) : ~0u;
	}

	uint32_t Release()
	{
		typename t_Traits::Type type = t_Traits::Invalid();

		if (m_type != type)
		{
			uint32_t ui = t_Traits::Release(m_type);

			m_type = type;

			return ui;
		}
		else
		{
			return ~0u;
		}
	}

	bool IsValid() const
	{
		return m_type != t_Traits::Invalid();
	}

	bool operator!() const
	{
		return !IsValid();
	}

	xRefCountType<t_Traits>& operator=(const xRefCountType<t_Traits>& type)
	{
		const bool valid = type.m_type != t_Traits::Invalid();

		X_ASSERT(valid);

		if (valid && this != &type)
		{
			Release();

			m_type = type.m_type;

			AddRef();
		}

		return *this;
	}

	typename t_Traits::Type operator=(typename t_Traits::Type type)
	{
		const bool valid = type != t_Traits::Invalid();

		X_ASSERT(valid);

		if (valid)
		{
			Release();

			m_type = type;

			AddRef();
		}

		return m_type;
	}

	bool operator==(typename t_Traits::Type type) const
	{
		return m_type == type;
	}

	operator const typename t_Traits::Type() const
	{
		return m_type;
	}

	operator typename t_Traits::Type()
	{
		return m_type;
	}

	const typename t_Traits::Type operator->() const
	{
		return GetSafeType();
	}

	typename t_Traits::Type operator->()
	{
		return GetSafeType();
	}

	typename t_Traits::Type* operator&()
	{
		X_ASSERT(m_type == t_Traits::Invalid());

		return &m_type;
	}

private:
	typename t_Traits::Type m_type;
};



template
<
	typename t_Type
>
class xMemoryTraits
{
public:
	struct _RefCount
	{
		typedef t_Type* Type;

		static Type Invalid()
		{
			return NULL;
		}

		static uint32_t AddRef(Type type)
		{
			return ~0u;
		}

		static uint32_t Release(Type type)
		{
			xMemoryFree(type);

			return 0;
		}
	};
};



#define X_PTR_BASE xRefCountType<typename xMemoryTraits<t_Type>::_RefCount>

template
<
	typename t_Type
>
class xPtr : public X_PTR_BASE
{
	typedef X_PTR_BASE _Base;

public:
	xPtr()
	{
	}

	xPtr(t_Type* pType) : _Base(pType)
	{
	}

public:
	t_Type* operator=(t_Type* type)
	{
		return _Base::operator=(type);
	}
};



template
<
	typename t_Type
>
class xInterfaceTraits
{
public:
	struct _RefCount
	{
		typedef t_Type* Type;

		static Type Invalid()
		{
			return NULL;
		}

		static uint32_t AddRef(Type type)
		{
			return type->AddRef();
		}

		static uint32_t Release(Type type)
		{
			return type->Release();
		}
	};
};



#define X_INTERFACE_PTR_BASE xRefCountType<typename xInterfaceTraits<t_Type>::_RefCount>

template
<
	typename t_Type
>
class xInterfacePtr : public X_INTERFACE_PTR_BASE
{
typedef X_INTERFACE_PTR_BASE _Base;

public:
	xInterfacePtr()
	{
	}

	xInterfacePtr(t_Type* pType) : _Base(pType)
	{
	}

public:
	xInterfacePtr<t_Type>& operator=(const xInterfacePtr<t_Type>& p)
	{
		_Base::operator=(p);

		return *this;
	}
};



template
<
	typename t_Type
>
class xObjectTraits
{
public:
	struct _RefCount
	{
		typedef t_Type* Type;

		static Type Invalid()
		{
			return NULL;
		}

		static uint32_t AddRef(Type type)
		{
			return ~0u;
		}

		static uint32_t Release(Type type)
		{
			delete type;

			return 0;
		}
	};
};



#define X_OBJECT_PTR_BASE xRefCountType<typename xObjectTraits<t_Type>::_RefCount>

template
<
	typename t_Type
>
class xObjectPtr : public X_OBJECT_PTR_BASE
{
typedef X_OBJECT_PTR_BASE _Base;

public:
	xObjectPtr()
	{
	}

	xObjectPtr(t_Type* pType) : _Base(pType)
	{
	}
	
public:
	t_Type* operator=(t_Type* type)
	{
		return _Base::operator=(type);
	}
};



template
<
	typename t_Base,
	typename t_Interface = IxInterface,
	typename t_Cast = t_Base
>
class xRefCount : public t_Base
{
protected:
	xRefCount()
	{
		m_refCount = 0;
	}

	virtual ~xRefCount()
	{
	}

protected:
	t_Interface* RefUnknown()
	{
		return static_cast<t_Interface*>(static_cast<t_Cast*>(this));
	}

protected:
	uint32_t m_refCount;
};



#define X_INTERFACE_MAP_BEGIN \
public: \
	X_INTERFACE_METHOD(uint32_t, AddRef)() \
	{ \
		return ++m_refCount; \
	} \
	X_INTERFACE_METHOD(uint32_t, Release)() \
	{ \
		return (--m_refCount) ? m_refCount : (delete this, 0); \
	} \
	X_INTERFACE_METHOD(xResult, QueryInterface)(const xGuid& iid, void** p) \
	{ \
		xResult r = X_S_OK; \
		if (!p) \
		{ \
			return X_E_INVALID_ARGUMENT; \
		} \
		if (xGuidIsEqual(iid, IID_IxInterface)) \
		{ \
			*p = RefUnknown(); \
		} \

#define X_INTERFACE_MAP_ENTRY(__Interface) \
		else if (xGuidIsEqual(iid, IID_##__Interface)) \
		{ \
			*p = static_cast<__Interface*>(this); \
		} \

#define X_INTERFACE_MAP_ENTRY_IF(__Interface, __Clause) \
		else if (xGuidIsEqual(iid, IID_##__Interface) && (__Clause)) \
		{ \
			*p = static_cast<__Interface*>(this); \
		} \

#define X_INTERFACE_MAP_ENTRY_IID(__Interface, __IID) \
		else if (xGuidIsEqual(iid, __IID)) \
		{ \
			*p = static_cast<__Interface*>(this); \
		} \

#define X_INTERFACE_MAP_ENTRY_RAW(__Interface, __IID, __Cast) \
		else if (xGuidIsEqual(iid, __IID)) \
		{ \
			*p = static_cast<__Interface*>(__Cast); \
		} \

#define X_INTERFACE_MAP_ENTRY_RAW_IF(__Interface, __IID, __Cast, __Clause) \
		else if (xGuidIsEqual(iid, __IID) && (__Clause)) \
		{ \
			*p = static_cast<__Interface*>(__Cast); \
		} \

#define X_INTERFACE_MAP_ENTRY_VOID(__IID, __Cast, __RefCommand) \
		else if (xGuidIsEqual(iid, __IID)) \
		{ \
			*p = __Cast; \
			__RefCommand; \
		} \

#define X_INTERFACE_MAP_ENTRY_VOID_IF(__IID, __Cast, __Clause, __RefCommand) \
		else if (xGuidIsEqual(iid, __IID) && (__Clause)) \
		{ \
			*p = __Cast; \
			__RefCommand; \
		} \

#define X_INTERFACE_MAP_ENTRY_VOID_FUCTIONS(__IID, __Function) \
		else if (xGuidIsEqual(iid, __IID)) \
		{ \
			return __Function(p); \
		} \

#define X_INTERFACE_MAP_CHAIN(__ChainClass) \
		else if (xSucceeded(r = __ChainClass::QueryInterface(iid, p))) \
		{ \
			return r; \
		} \

#define X_INTERFACE_MAP_END \
		else \
		{ \
			*p = NULL; \
			return X_E_NO_INTERFACE; \
		} \
		AddRef(); \
		return (*p) ? r : X_E_NO_INTERFACE; \
	}


