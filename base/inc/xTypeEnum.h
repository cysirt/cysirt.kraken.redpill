#pragma once

#include "xArray.h"
#include "xBaseInterface.h"
#include "xReferenceCount.h"



template
<
	typename t_Type,
	typename t_InterfaceEnum
>
class xTypeEnum : public xRefCount<t_InterfaceEnum>
{
protected:
	xTypeEnum()
	{
	}

public:
	virtual ~xTypeEnum()
	{
	}

public:
	X_INTERFACE_METHOD(uint32_t, GetCount)()
	{
		return m_arr.GetCount();
	}

	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, t_Type* type)
	{
		X_ASSERT(type);

		if (!type)
		{
			return X_E_INVALID_ARGUMENT;
		}

		const uint32_t size = m_arr.GetCount();

		X_ASSERT(index < size);
		
		if (index >= size)
		{
			return X_E_INVALID_ARGUMENT;
		}

		return (*type = m_arr[index]) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}

	X_INTERFACE_METHOD(xResult, Add)(const t_Type type)
	{
		m_arr.Add(type);

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Add)(t_InterfaceEnum* ifaceEnum)
	{
		X_ASSERT(ifaceEnum);

		if (!ifaceEnum)
		{
			return X_E_INVALID_ARGUMENT;
		}

		const uint32_t count = ifaceEnum->GetCount();

		for (uint32_t ui = 0; ui < count; ui++)
		{
			t_Type type = 0;

			xResult r = ifaceEnum->Get(ui, &type);

			if (xFailed(r) || 
				xFailed(r = Add(type)))
			{
				return r;
			}
		}

		return X_S_OK;
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

private:
	xArray<t_Type> m_arr;
};



#define X_TYPE_ENUM_DECLARE(__Name, __Type) \
	class __Name : public xTypeEnum<__Type, I##__Name> \
	{ \
	X_INTERFACE_MAP_BEGIN \
		X_INTERFACE_MAP_ENTRY(I##__Name) \
	X_INTERFACE_MAP_END \
	};



X_TYPE_ENUM_DECLARE(xBoolEnum, bool)

X_TYPE_ENUM_DECLARE(xInt8Enum, int8_t)

X_TYPE_ENUM_DECLARE(xInt16Enum, int16_t)

X_TYPE_ENUM_DECLARE(xInt32Enum, int32_t)

X_TYPE_ENUM_DECLARE(xInt64Enum, int64_t)

X_TYPE_ENUM_DECLARE(xUint8Enum, uint8_t)

X_TYPE_ENUM_DECLARE(xUint16Enum, uint16_t)

X_TYPE_ENUM_DECLARE(xUint32Enum, uint32_t)

X_TYPE_ENUM_DECLARE(xUint64Enum, uint64_t)

X_TYPE_ENUM_DECLARE(xTimeEnum, time_t)
