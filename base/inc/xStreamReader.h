#pragma once

#include "xBaseInterface.h"
#include "xAssert.h"
#include "xMemory.h"
#include "xReferenceCount.h"



class xStreamReader : public xRefCount<IxStreamReader>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStreamReader)
X_INTERFACE_MAP_END

public:
	xStreamReader()
	{
		m_stream;
	}
	
	virtual ~xStreamReader()
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

	X_INTERFACE_METHOD(xResult, Read)(int32_t* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(int64_t* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(uint64_t* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(uint32_t* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(uint8_t* value, const uint32_t size)
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

		return m_stream->Read(value, size) == size ? X_S_OK : X_E_FAIL;
	}

	X_INTERFACE_METHOD(xResult, Read)(uint8_t** value, const uint32_t size)
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

		xPtr<uint8_t> buffer;

		if (m_stream->Read(&buffer, size) == size)
		{
			*value = buffer.Detach();
		}

		return size;
	}

	X_INTERFACE_METHOD(xResult, Read)(uint16_t* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(bool* value)
	{
		return ReadType(value);
	}

	X_INTERFACE_METHOD(xResult, Read)(xChar** value, const uint32_t count)
	{
		X_ASSERT(value && count);

		if (!value || !count)
		{
			return X_E_INVALID_ARGUMENT;
		}

		X_ASSERT(m_stream);
		
		if (!m_stream)
		{
			return X_E_INTERNAL;
		}

		xPtr<xChar> temp;

		const uint32_t size = (count + 1) * sizeof(xChar);

		temp.Attach((xChar*) xMemoryAlloc(size));

		if (!temp)
		{
			return X_E_OUT_OF_MEMORY;
		}

		xResult r = m_stream->Read((uint8_t*) (xChar*) temp, count * sizeof(xChar)) == (count * sizeof(xChar)) ? X_S_OK : X_E_FAIL;
		
		if (xFailed(r))
		{
			return r;
		}

		*value = temp.Detach();

		return r;
	}

private:
	template <typename t_Type> xResult ReadType(t_Type value)
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

		return m_stream->Read((uint8_t*) value, sizeof(*value)) == sizeof(*value) ? X_S_OK : X_E_FAIL;
	}

	inline void Clean()
	{
		m_stream.Release();
	}

private:
	xInterfacePtr<IxStream> m_stream;
};
