#pragma once

#include "xBaseInterface.h"
#include "xMemory.h"
#include "xReferenceCount.h"



class xMemoryStream : public xRefCount<IxStream>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStream)
X_INTERFACE_MAP_END

public:
	xMemoryStream()
	{
		m_size = 0;

		m_position = 0;
	}

	~xMemoryStream()
	{
	}

public:
	X_INTERFACE_METHOD(xResult, SetPosition)(const uint32_t offset, const xStreamOriginFlag origin)
	{
		X_ASSERT(m_buffer);

		if (!m_buffer)
		{
			return 0;
		}

		switch (origin)
		{
			case xStreamOriginFlagBegin:
			{
				X_ASSERT(m_size > offset);

				if (m_size <= offset)
				{
					return X_E_INVALID_ARGUMENT;
				}

				m_position = offset;
				
				break;
			}
			case xStreamOriginFlagCurrent:
			{
				const uint32_t position = m_position + offset;

				X_ASSERT(m_size > position);

				if (m_size <= position)
				{
					return X_E_INVALID_ARGUMENT;
				}

				m_position = position;

				break;
			}
			case xStreamOriginFlagEnd:
			{
				m_position = m_size;

				break;
			}

			default:
			{
				X_ASSERT(false);

				return X_E_INVALID_ARGUMENT;
			}
		}

		return X_S_OK;
	}

	X_INTERFACE_METHOD(uint64_t, GetSize)()
	{
		return m_size;
	}

	X_INTERFACE_METHOD(uint32_t, Write)(const uint8_t* buffer, const uint32_t size)
	{
		X_ASSERT(buffer && size);

		if (!buffer || !size)
		{
			return 0;
		}

		const uint32_t required = m_position + size;

		if (required > m_size)
		{
			xPtr<uint8_t> temp((uint8_t*) xMemoryAlloc(required));

			if (!temp)
			{
				return 0;
			}

			if (m_buffer && m_size)
			{
				xMemoryCopy(temp, m_buffer, m_size);
			}

			m_buffer.Attach(temp.Detach());
		}

		xMemoryCopy(m_buffer + m_position, buffer, size);

		m_size = required;

		m_position += size;

		return X_S_OK;
	}

	X_INTERFACE_METHOD(uint32_t, Read)(uint8_t* buffer, const uint32_t size)
	{
		X_ASSERT(buffer && size);

		if (!buffer || !size)
		{
			return 0;
		}

		if ((m_position + size) > m_size)
		{
			return 0;
		}

		xMemoryCopy(buffer, m_buffer + m_position, size);

		m_position += size;

		return size;
	}

	X_INTERFACE_METHOD(uint32_t, Read)(uint8_t** buffer, const uint32_t size)
	{
		X_ASSERT(buffer && size);

		if (!buffer || !size)
		{
			return 0;
		}

		xPtr<uint8_t> temp((uint8_t*) xMemoryAlloc(size));

		if (!temp)
		{
			return 0;
		}

		const uint32_t read = Read(temp, size);

		if (read)
		{
			*buffer = temp.Detach();
		}

		return read;
	}

	X_INTERFACE_METHOD(xResult, Reset)()
	{
		m_position = 0;

		return X_S_OK;
	}

private:
	xPtr<uint8_t> m_buffer;
	uint32_t m_size;
	uint32_t m_position;
};
