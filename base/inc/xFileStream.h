#pragma once

#include "xBaseInterface.h"
#include "xFile.h"
#include "xMemory.h"
#include "xReferenceCount.h"



class xFileStream : public xRefCount<IxStream>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStream)
X_INTERFACE_MAP_END

public:
	xFileStream()
	{
		m_file = NULL;
	}

	~xFileStream()
	{
		Clean();
	}

public:
	xResult Open(const xChar* fileName, const bool read = true)
	{
		X_ASSERT(fileName);

		if (!fileName)
		{
			return X_E_INVALID_ARGUMENT;
		}

		Clean();

		m_file = xFileOpen(fileName, read ? L"rb" : L"wb");

		return m_file ? X_S_OK : xResultFromErrNo();
	}

	X_INTERFACE_METHOD(xResult, SetPosition)(const uint32_t offset, const xStreamOriginFlag origin)
	{
		X_ASSERT(m_file);

		if (!m_file)
		{
			return 0;
		}

		int pos = 0;

		switch (origin)
		{
			case xStreamOriginFlagBegin:
			{
				pos = SEEK_SET;
				
				break;
			}
			case xStreamOriginFlagCurrent:
			{
				pos = SEEK_CUR;

				break;
			}
			case xStreamOriginFlagEnd:
			{
				pos = SEEK_END;

				break;
			}

			default:
			{
				X_ASSERT(false);

				return X_E_INVALID_ARGUMENT;
			}
		}

		return fseek(m_file, offset, pos) == 0 ? X_S_OK : xResultFromErrNo();
	}

	X_INTERFACE_METHOD(uint64_t, GetSize)()
	{
		X_ASSERT(m_file);

		if (!m_file)
		{
			return 0;
		}

		return xFileGetSize(m_file);
	}

	X_INTERFACE_METHOD(uint32_t, Write)(const uint8_t* buffer, const uint32_t size)
	{
		X_ASSERT(buffer && size);

		if (!buffer || !size)
		{
			return 0;
		}

		return (uint32_t) xFileWrite(buffer, size, m_file);
	}

	X_INTERFACE_METHOD(uint32_t, Read)(uint8_t* buffer, const uint32_t size)
	{
		X_ASSERT(buffer && size);

		if (!buffer || !size)
		{
			return 0;
		}

		return (uint32_t) xFileRead(buffer, size, m_file);
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
		return X_E_FAIL;
	}

private:
	void Clean()
	{
		if (m_file)
		{
			xFileClose(m_file);

			m_file = NULL;
		}
	}

private:
	FILE* m_file;
};
