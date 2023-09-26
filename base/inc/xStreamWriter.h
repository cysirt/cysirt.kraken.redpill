#pragma once

#include "xBaseInterface.h"
#include "xAssert.h"
#include "xMemory.h"
#include "xReferenceCount.h"



class xStreamWriter : public xRefCount<IxStreamWriter>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStreamWriter)
X_INTERFACE_MAP_END

public:
	xStreamWriter()
	{
		m_stream;
	}
	
	virtual ~xStreamWriter()
	{
		Clean();
	}

public:
	X_INTERFACE_METHOD(xResult, Set)(IxStream* stream)
	{
		X_ASSERT(stream);
	
		if (!stream)
		{
			return X_E_INVALID_ARGUMENT;
		}

		Clean();

		m_stream = stream;

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Write)(int32_t value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(int64_t value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(uint64_t value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(uint32_t value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(const uint8_t* value, const uint32_t size)
	{
		X_ASSERT(value && size);

		if (!value || !size)
		{
			return X_E_INVALID_ARGUMENT;
		}

		X_ASSERT(m_stream);

		if (!m_stream)
		{
			return X_E_INTERNAL;
		}

		return m_stream->Write(value, size) == size ? X_S_OK : X_E_FAIL;
	}

	X_INTERFACE_METHOD(xResult, Write)(uint16_t value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(bool value)
	{
		return WriteType(value);
	}

	X_INTERFACE_METHOD(xResult, Write)(const xChar* value)
	{
		X_ASSERT(value);

		if (!value)
		{
			return X_E_INVALID_ARGUMENT;
		}

		X_ASSERT(m_stream);
		
		if (!m_stream)
		{
			return X_E_INTERNAL;
		}

		const uint32_t size = (uint32_t) (wcslen(value) + 1) * sizeof(xChar);

		return m_stream->Write((uint8_t*) (xChar*) value, size) == size ? X_S_OK : X_E_FAIL;
	}

private:
	template <typename t_Type> xResult WriteType(t_Type value)
	{
		X_ASSERT(m_stream);
		
		if (!m_stream)
		{
			return X_E_INTERNAL;
		}

		return m_stream->Write((uint8_t*) &value, sizeof(value)) == sizeof(value) ? X_S_OK : X_E_FAIL;
	}

	inline void Clean()
	{
		m_stream.Release();
	}

private:
	xInterfacePtr<IxStream> m_stream;
};
