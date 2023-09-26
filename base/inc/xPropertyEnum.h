#pragma once

#include "xConversion.h"
#include "xProperty.h"
#include "xString.h"



// Property map
#define X_PROPERTY_MAP_BEGIN

#define X_PROPERTY_MAP_SET_SPECIAL(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(__Type type) \
	{ \
		return SetPropertyMapValue(#__Name, type); \
	}

#define X_PROPERTY_MAP_GET_SPECIAL(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Get##__Name)(__Type type) \
	{ \
		return GetPropertyMapValue(#__Name, type); \
	}

#define X_PROPERTY_MAP_SET(__Name, __Type) \
	X_PROPERTY_MAP_SET_SPECIAL(__Name, const __Type)

#define X_PROPERTY_MAP_GET(__Name, __Type) \
	X_PROPERTY_MAP_GET_SPECIAL(__Name, __Type*)

#define X_PROPERTY_MAP_SET_INTERFACE(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(__Type* type) \
	{ \
		return SetPropertyMapValue(#__Name, type); \
	}

#define X_PROPERTY_MAP_GET_INTERFACE(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Get##__Name)(__Type** type) \
	{ \
		xInterfacePtr<IxInterface> p; \
		xResult r = GetPropertyMapValue(#__Name, &p); \
		return xSucceeded(r) ? p->QueryInterface(IID_##__Type, (void**) type) : r; \
	}

#define X_PROPERTY_MAP_SET_GET(__Name, __Type) \
	X_PROPERTY_MAP_SET(__Name, __Type) \
	X_PROPERTY_MAP_GET(__Name, __Type)

#define X_PROPERTY_MAP_SET_GET_SPECIAL(__Name, __TypeSet, __TypeGet) \
	X_PROPERTY_MAP_SET_SPECIAL(__Name, __TypeSet) \
	X_PROPERTY_MAP_GET_SPECIAL(__Name, __TypeGet)

#define X_PROPERTY_MAP_SET_GET_INTERFACE(__Name, __Type) \
	X_PROPERTY_MAP_SET_INTERFACE(__Name, __Type) \
	X_PROPERTY_MAP_GET_INTERFACE(__Name, __Type)

#define X_PROPERTY_MAP_END



template
<
	typename t_Base
>
class xPropertyEnum : public xInterfaceEnum<t_Base>
{
typedef xInterfaceEnum<t_Base> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxPropertyEnum)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

public:
	xPropertyEnum()
	{
	};

	virtual ~xPropertyEnum()
	{
	};

public:
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxProperty** property)
	{
		return xInterfaceEnum<t_Base>::Get(index, IID_IxProperty, (IxInterface**) property);
	}
		
	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, IxPropertyEnum** propertyEnum)
	{
		return xInterfaceEnum<t_Base>::Get(index, IID_IxPropertyEnum, (IxInterface**) propertyEnum);
	}
	
protected:
	template <typename t_Type> xResult SetPropertyMapValue(const char* name, t_Type value)
	{
		X_ASSERT(name);

		if (!name)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xResult r = X_S_OK;

		// Check existing property
		const uint32_t count = GetCount();

		for (uint32_t ui = 0; ui < count; ui++)
		{
			xInterfacePtr<IxProperty> p;

			if (xSucceeded(r = Get(ui, &p)))
			{
				if (strcmp(p->GetName(), name) == 0)
				{
					return p->SetValue(value);
				}
			}
		}

		// Create new
		xInterfacePtr<xProperty> p(new xProperty);

		if (!p)
		{
			return X_E_OUT_OF_MEMORY;
		}

		if (xSucceeded(r = p->SetName(name)) && 
			xSucceeded(r = p->SetValue(value)))
		{
			r = Add(p);
		}

		return r;
	};

	template <typename t_Type> xResult GetPropertyMapValue(const char* name, t_Type value)
	{
		X_ASSERT(name && value);

		if (!name || !value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xResult r = X_S_OK;

		// Find property
		const uint32_t count = GetCount();

		for (uint32_t ui = 0; ui < count; ui++)
		{
			xInterfacePtr<IxProperty> p;

			if (xSucceeded(r = Get(ui, &p)))
			{
				if (strcmp(p->GetName(), name) == 0)
				{
					return p->GetValue(value);
				}
			}
		}

		return X_E_NOT_PRESENT;
	};
};
