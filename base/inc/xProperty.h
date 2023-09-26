#pragma once

#include "xAssert.h"
#include "xBaseInterface.h"
#include "xInterfaceEnum.h"
#include "xMemory.h"



class xProperty : public xRefCount<IxProperty>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxProperty)
X_INTERFACE_MAP_END

public:
	xProperty()
	{
		m_propertyType = xPropertyTypeInvalid;

		m_propertyIgnore = false;

		xMemoryZero(&m_propertyData, sizeof(PropertyData));
	};
	
	virtual ~xProperty()
	{
	};

	X_INTERFACE_METHOD(xResult, SetName)(const char* name)
	{
		X_ASSERT(name);

		if (!name)
		{
			return X_E_INVALID_ARGUMENT;
		}

		const size_t size = (strlen(name) + 1) * sizeof(char);

		char* n = (char*) xMemoryClone(name, size);

		if (!n)
		{
			return X_E_OUT_OF_MEMORY;
		}

		m_propertyName = n;

		return X_S_OK;
	};

	X_INTERFACE_METHOD(const char*, GetName)()
	{
		X_ASSERT(m_propertyName);

		return m_propertyName;
	};

	X_INTERFACE_METHOD(xResult, SetType)(const xPropertyType type)
	{
		X_ASSERT(m_propertyType == xPropertyTypeInvalid || m_propertyType == type);

		if (m_propertyType != xPropertyTypeInvalid && m_propertyType != type)
		{
			return X_E_INTERNAL;
		}

		m_propertyType = type;

		return X_S_OK;
	};

	X_INTERFACE_METHOD(xPropertyType, GetType)()
	{
		return m_propertyType;
	};

	X_INTERFACE_METHOD(void, SetIgnore)(const bool ignore)
	{
		m_propertyIgnore = ignore;
	};

	X_INTERFACE_METHOD(bool, GetIgnore)()
	{
		return m_propertyIgnore;
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const bool value)
	{
		return SetPropertyValue(xPropertyTypeInt8, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(bool* value)
	{
		return GetPropertyValue(xPropertyTypeInt8, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const int8_t value)
	{
		return SetPropertyValue(xPropertyTypeInt8, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(int8_t* value)
	{
		return GetPropertyValue(xPropertyTypeInt8, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const int16_t value)
	{
		return SetPropertyValue(xPropertyTypeInt16, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(int16_t* value)
	{
		return GetPropertyValue(xPropertyTypeInt16, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const int32_t value)
	{
		return SetPropertyValue(xPropertyTypeInt32, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(int32_t* value)
	{
		return GetPropertyValue(xPropertyTypeInt32, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const int64_t value)
	{
		return SetPropertyValue(xPropertyTypeInt64, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(int64_t* value)
	{
		return GetPropertyValue(xPropertyTypeInt64, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const uint8_t value)
	{
		return SetPropertyValue(xPropertyTypeUint8, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(uint8_t* value)
	{
		return GetPropertyValue(xPropertyTypeUint8, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const uint16_t value)
	{
		return SetPropertyValue(xPropertyTypeUint16, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(uint16_t* value)
	{
		return GetPropertyValue(xPropertyTypeUint16, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const uint32_t value)
	{
		return SetPropertyValue(xPropertyTypeUint32, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(uint32_t* value)
	{
		return GetPropertyValue(xPropertyTypeUint32, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const uint64_t value)
	{
		return SetPropertyValue(xPropertyTypeUint64, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(uint64_t* value)
	{
		return GetPropertyValue(xPropertyTypeUint64, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const xChar* value)
	{
		return SetPropertyValue(xPropertyTypeString, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(xChar** value)
	{
		return GetPropertyValue(xPropertyTypeString, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(IxInterface* value)
	{
		return SetPropertyValue(xPropertyTypeInterface, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(IxInterface** value)
	{
		return GetPropertyValue(xPropertyTypeInterface, value);
	};

	X_INTERFACE_METHOD(xResult, SetValue)(const xGuid& value)
	{
		return SetPropertyValue(xPropertyTypeGuid, value);
	};

	X_INTERFACE_METHOD(xResult, GetValue)(xGuid* value)
	{
		return GetPropertyValue(xPropertyTypeGuid, value);
	};

private:
	template <typename t_Type> xResult SetPropertyValue(const xPropertyType type, t_Type value)
	{
		X_ASSERT(m_propertyType == xPropertyTypeInvalid || m_propertyType == type);

		if (m_propertyType != xPropertyTypeInvalid && m_propertyType != type)
		{
			return X_E_INVALID_TYPE;
		}

		SetType(type);

		return SetValueData(value);
	}
	
	xResult SetValueData(bool value)
	{
		m_propertyData.boolValue = value;

		return X_S_OK;
	}

	xResult SetValueData(int8_t value)
	{
		m_propertyData.int8Value = value;

		return X_S_OK;
	}

	xResult SetValueData(int16_t value)
	{
		m_propertyData.int16Value = value;

		return X_S_OK;
	}

	xResult SetValueData(int32_t value)
	{
		m_propertyData.int32Value = value;

		return X_S_OK;
	}

	xResult SetValueData(int64_t value)
	{
		m_propertyData.int64Value = value;

		return X_S_OK;
	}

	xResult SetValueData(uint8_t value)
	{
		m_propertyData.uint8Value = value;

		return X_S_OK;
	}

	xResult SetValueData(uint16_t value)
	{
		m_propertyData.uint16Value = value;

		return X_S_OK;
	}

	xResult SetValueData(uint32_t value)
	{
		m_propertyData.uint32Value = value;

		return X_S_OK;
	}

	xResult SetValueData(uint64_t value)
	{
		m_propertyData.uint64Value = value;

		return X_S_OK;
	}

	xResult SetValueData(const xChar* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		m_propertyData.stringValue = xCharClone(value);

		return X_S_OK;
	}

	xResult SetValueData(IxInterface* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		(m_propertyData.interfaceValue = value)->AddRef();

		return X_S_OK;
	}

	xResult SetValueData(const xGuid& value)
	{
		m_propertyData.guidValue = value;

		return X_S_OK;
	}

	template <typename t_Type> xResult GetPropertyValue(const xPropertyType type, t_Type value)
	{
		const xPropertyType propertyType = GetType();

		X_ASSERT(propertyType == type);

		if (propertyType != type)
		{
			return X_E_INVALID_TYPE;
		}

		return GetValueData(value);
	}

	xResult GetValueData(bool* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.boolValue;

		return X_S_OK;
	}

	xResult GetValueData(int8_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.int8Value;

		return X_S_OK;
	}

	xResult GetValueData(int16_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.int16Value;

		return X_S_OK;
	}

	xResult GetValueData(int32_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.int32Value;

		return X_S_OK;
	}

	xResult GetValueData(int64_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.int64Value;

		return X_S_OK;
	}

	xResult GetValueData(uint8_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.uint8Value;

		return X_S_OK;
	}

	xResult GetValueData(uint16_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.uint16Value;

		return X_S_OK;
	}

	xResult GetValueData(uint32_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.uint32Value;

		return X_S_OK;
	}

	xResult GetValueData(uint64_t* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.uint64Value;

		return X_S_OK;
	}

	xResult GetValueData(xChar** value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		return (*value = xCharClone(m_propertyData.stringValue)) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}

	xResult GetValueData(IxInterface** value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		X_ASSERT(m_propertyData.interfaceValue);

		if (!m_propertyData.interfaceValue)
		{
			return X_E_INVALID_ARGUMENT;
		}

		(*value = m_propertyData.interfaceValue)->AddRef();

		return X_S_OK;
	}

	xResult GetValueData(xGuid* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		*value = m_propertyData.guidValue;

		return X_S_OK;
	}

private:
	xPtr<char> m_propertyName;
	xPropertyType m_propertyType;
	bool m_propertyIgnore;
	union PropertyData
	{
		PropertyData()
		{
		};
		
		~PropertyData()
		{
		};

		bool boolValue;
		int8_t int8Value;
		int16_t int16Value;
		int32_t int32Value;
		int64_t int64Value;
		uint8_t uint8Value;
		uint16_t uint16Value;
		uint32_t uint32Value;
		uint64_t uint64Value;
		xPtr<xChar> stringValue;
		xInterfacePtr<IxInterface> interfaceValue;
		xGuid guidValue;
	} m_propertyData;
};
