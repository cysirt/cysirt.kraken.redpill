#pragma once

#include "xConversion.h"
#include "xPropertyEnum.h"
#include "xString.h"



// String method appendix
#define X_CORE_PROPERTY_MAP_STRING_APPENDIX			String
#define X_CORE_PROPERTY_MAP_STRING_ENUM_APPENDIX	StringEnum

// Core property map
#define X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_ENUM(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(__Type type) \
	{ \
		xResult r = SetPropertyMapValue(#__Name, type); \
		if (xSucceeded(r)) \
		{ \
			xInterfacePtr<IxStringEnum> stringEnum; \
			if (xSucceeded(r = X_CONCAT_EXPAND_3(Get, __Name, X_CORE_PROPERTY_MAP_STRING_ENUM_APPENDIX)(&stringEnum))) \
			{ \
				 r = SetPropertyMapValue(#__Name X_CHAR_STRINGIZE_EXPAND_A(X_CORE_PROPERTY_MAP_STRING_ENUM_APPENDIX), stringEnum); \
			} \
		} \
		return r; \
	}

#define X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_HEX(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(__Type type) \
	{ \
		return SetPropertyMapValueStringCompanionHex(#__Name, type); \
	}

#define X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_RAW(__Name, __Type) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(__Type type) \
	{ \
		xResult r = SetPropertyMapValue(#__Name, type); \
		xPtr<xChar> s; \
		if (xSucceeded(r = X_CONCAT_EXPAND_3(Get, __Name, X_CORE_PROPERTY_MAP_STRING_APPENDIX)(&s))) \
		{ \
			r = SetPropertyMapValue(#__Name X_CHAR_STRINGIZE_EXPAND_A(X_CORE_PROPERTY_MAP_STRING_APPENDIX), s); \
		} \
		return r; \
	}

#define X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_TIME(__Name) \
public: \
	X_INTERFACE_METHOD(xResult, Set##__Name)(time_t type) \
	{ \
		return SetPropertyMapValueStringCompanionTime(#__Name, type); \
	}

#define X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_ENUM(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_ENUM(__Name, __Type)

#define X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_HEX(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_HEX(__Name, __Type)

#define X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_RAW(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_RAW(__Name, __Type)

#define X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_TIME(__Name) \
	X_CORE_PROPERTY_MAP_SET_SPECIAL_COMPANION_STRING_TIME(__Name)

#define X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_ENUM(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_ENUM(__Name, __Type) \
	X_PROPERTY_MAP_GET(__Name, __Type) \
	X_PROPERTY_MAP_GET(X_CONCAT_EXPAND_2(__Name, X_CORE_PROPERTY_MAP_STRING_APPENDIX), xChar*) \

#define X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_HEX(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_HEX(__Name, __Type) \
	X_PROPERTY_MAP_GET(__Name, __Type) \
	X_PROPERTY_MAP_GET(X_CONCAT_EXPAND_2(__Name, X_CORE_PROPERTY_MAP_STRING_APPENDIX), xChar*)

#define X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(__Name, __Type) \
	X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_RAW(__Name, __Type) \
	X_PROPERTY_MAP_GET(__Name, __Type) \

#define X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(__Name) \
	X_CORE_PROPERTY_MAP_SET_COMPANION_STRING_TIME(__Name) \
	X_PROPERTY_MAP_GET(__Name, time_t) \
	X_PROPERTY_MAP_GET(X_CONCAT_EXPAND_2(__Name, X_CORE_PROPERTY_MAP_STRING_APPENDIX), xChar*)



template
<
	typename t_Base
>
class xCorePropertyEnum : public xPropertyEnum<t_Base>
{
typedef xInterfaceEnum<t_Base> _Base;

public:
	xCorePropertyEnum()
	{
	};

	virtual ~xCorePropertyEnum()
	{
	};
	
protected:
	template <typename t_Type> xResult SetPropertyMapValueStringCompanionHex(const char* name, t_Type value)
	{
		X_ASSERT(name);

		if (!name)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xPtr<xChar> valueString;

		xResult r = SetPropertyMapValue(name, value);

		if (xFailed(r) ||
			xFailed(r = GetPropertyMapValueStringFormatHex(value, &valueString)))
		{
			return r;
		}

		xPtr<char> nameString(xCharAppendA(name, X_CHAR_STRINGIZE_EXPAND_A(X_CORE_PROPERTY_MAP_STRING_APPENDIX)));

		if (!nameString)
		{
			return X_E_OUT_OF_MEMORY;
		}

		return SetPropertyMapValue(nameString, valueString);
	}

	xResult SetPropertyMapValueStringCompanionTime(const char* name, const time_t value)
	{
		X_ASSERT(name);

		if (!name)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xPtr<xChar> valueString;

		xResult r = SetPropertyMapValue(name, value);

		if (xFailed(r) ||
			xFailed(r = GetPropertyMapValueStringFormatTime(value, &valueString)))
		{
			return r;
		}

		xPtr<char> nameString(xCharAppendA(name, X_CHAR_STRINGIZE_EXPAND_A(X_CORE_PROPERTY_MAP_STRING_APPENDIX)));

		if (!nameString)
		{
			return X_E_OUT_OF_MEMORY;
		}

		return SetPropertyMapValue(nameString, valueString);
	}

	xResult GetPropertyMapValueStringFormatHex(const bool value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_2, stringValue);
	}
		
	xResult GetPropertyMapValueStringFormatHex(const int8_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_2, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const int16_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_4, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const int32_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_8, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const int64_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_16, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const uint8_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_2, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const uint16_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_4, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const uint32_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_8, stringValue);
	}

	xResult GetPropertyMapValueStringFormatHex(const uint64_t value, xChar** stringValue)
	{
		return GetPropertyMapValueStringFormatHex(value, X_CHAR_FORMAT_HEX_16, stringValue);
	}

	template <typename t_Type> xResult GetPropertyMapValueStringFormatHex(const t_Type value, const xChar* format, xChar** stringValue)
	{
		X_ASSERT(format && stringValue);

		if (!format || !stringValue)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xString s;

		s.Format(format, value);

		return (*stringValue = s.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
	};

	xResult GetPropertyMapValueStringFormatTime(const time_t value, xChar** stringValue)
	{
		X_ASSERT(stringValue);

		if (!stringValue)
		{
			return X_E_INVALID_ARGUMENT;
		}

		return xConvertTimeToIso8601(value, stringValue);
	};
};
