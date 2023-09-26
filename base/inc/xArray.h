#pragma once

#include <new>
#include <stdint.h>
#include <stdlib.h>

#include "xAssert.h"
#include "xMemory.h"



template
<
	typename t_Type,
	typename t_Argument = const t_Type&
>
class xArray
{
public:
	xArray()
	{
		m_data = NULL;

		m_size = m_sizeMax = m_grow = 0;
	}

	xArray(const xArray& arr)
	{
		Copy(arr);
	}

	~xArray()
	{
		if (m_data)
		{
			for (uint32_t ui = 0; ui < m_size; ui++)
			{
				(m_data + ui)->~t_Type();
			}

			xMemoryFree(m_data);
		}
	}

public:
	uint32_t GetCount() const
	{
		return m_size;
	}

	void SetSize(const uint32_t size, const int32_t grow = -1)
	{
		if (grow >= 0)
		{
			m_grow = grow;
		}

		if (size == 0)
		{
			if (m_data)
			{
				for (uint32_t ui = 0; ui < m_size; ui++)
				{
					(m_data + ui)->~t_Type();
				}

				xMemoryFree(m_data);

				m_data = NULL;
			}

			m_size = m_sizeMax = 0;
		}
		else if (!m_data)
		{
			const uint32_t sizeAlloc = size >= m_grow ? size : m_grow;

			m_data = (t_Type*) xMemoryAlloc(sizeAlloc * sizeof(t_Type));
			
			for (uint32_t ui = 0; ui < size; ui++)
			{
#pragma push_macro("new")
#undef new
				::new((void*) (m_data + ui)) t_Type;
#pragma pop_macro("new")
			}

			m_size = size;

			m_sizeMax = sizeAlloc;
		}
		else if (size <= m_sizeMax)
		{
			if (size > m_size)
			{
				xMemoryZero(m_data + m_size, (size - m_size) * sizeof(t_Type));

				for (uint32_t ui = 0; ui < (size - m_size); ui++)
				{
#pragma push_macro("new")
#undef new
					::new((void*) (m_data + m_size + ui)) t_Type;
#pragma pop_macro("new")
				}
			}
			else if (m_size > size)
			{
				for (uint32_t ui = 0; ui < (m_size - size); ui++)
				{
					(m_data + size + ui)->~t_Type();
				}
			}

			m_size = size;
		}
		else
		{
			uint32_t grow = m_grow;

			if (!grow)
			{
				grow = m_size / 8;

				grow = (grow < 4) ? 4 : ((grow > 1024) ? 1024 : grow);
			}

			const uint32_t sizeMaxNew = (size < m_sizeMax + grow) ? (m_sizeMax + grow) : size;

			X_ASSERT(sizeMaxNew >= m_sizeMax);

			if (sizeMaxNew < m_sizeMax)
			{
				return;
			}

			t_Type* data = (t_Type*) xMemoryAlloc(sizeMaxNew * sizeof(t_Type));

			xMemoryCopy(data, m_data, m_size * sizeof(t_Type));

			X_ASSERT(size > m_size);
			
			xMemoryZero(data + m_size, (size - m_size) * sizeof(t_Type));

			for (uint32_t ui = 0; ui < (size - m_size); ui++)
			{
#pragma push_macro("new")
#undef new
				::new((void*) (data + m_size + ui)) t_Type;
#pragma pop_macro("new")
			}

			xMemoryFree(m_data);

			m_data = data;
		
			m_size = size;

			m_sizeMax = sizeMaxNew;
		}
	}

	bool IsEmpty() const
	{
		return m_size == 0;
	}

	const t_Type& GetAt(const uint32_t index) const
	{
		X_ASSERT(index >= 0 && index < m_size);

		return m_data[index];
	}

	t_Type& GetAt(const uint32_t index)
	{
		X_ASSERT(index >= 0 && index < m_size);

		return m_data[index];
	}

	const t_Type& operator[](const uint32_t index) const
	{
		X_ASSERT(index >= 0 && index < m_size);

		return GetAt(index);
	}

	t_Type& operator[](const uint32_t index)
	{
		X_ASSERT(index >= 0 && index < m_size);

		return GetAt(index);
	}

	const t_Type* GetData() const
	{
		return (const t_Type*) m_data;
	}

	t_Type* GetData()
	{
		return (t_Type*) m_data;
	}

	uint32_t Add(t_Argument obj)
	{
		const uint32_t index = m_size;

		SetAtGrow(index, obj);
		
		return index;
	}

	void InsertAt(const uint32_t index, t_Argument obj, const uint32_t count = 1)
	{
		X_ASSERT(index >= 0 && count > 0);
		if (index < 0 || count <= 0)
			return;

		if (index >= m_size)
		{
			SetSize(index + count, -1);
		}
		else
		{
			const uint32_t sizeOld = m_size;

			SetSize(m_size + count, -1);

			for (uint32_t ui = 0; ui < count; ui++)
			{
				(m_data + sizeOld + ui)->~t_Type();
			}

			xMemoryMove(m_data + index + count, m_data + index, (sizeOld - index) * sizeof(t_Type));

			xMemoryZero(m_data + index, count * sizeof(t_Type));

			for (uint32_t ui = 0; ui < count; ui++)
			{
#pragma push_macro("new")
#undef new
				::new((void*) (m_data + m_size + ui)) t_Type;
#pragma pop_macro("new")
			}
		}

		X_ASSERT(index + count <= m_size);

		for (uint32_t ui = 0; ui < count; ui++)
		{
			m_data[index + ui] = obj;
		}
	}

	void InsertAt(const uint32_t index, xArray* arr)
	{
		X_ASSERT(arr);

		if (!arr)
		{
			return;
		}

		if (arr->GetSize())
		{
			InsertAt(index, arr->GetAt(0), arr->GetSize());

			for (uint32_t ui = 0; ui < arr->GetSize(); ui++)
			{
				SetAt(index + ui, arr->GetAt(ui));
			}
		}
	}

	void SetAt(const uint32_t index, t_Argument obj)
	{
		X_ASSERT(index >= 0 && index < m_size);

		m_data[index] = obj; 
	}

	void SetAtGrow(const uint32_t index, t_Argument obj)
	{
		X_ASSERT(index >= 0);

		if (index < 0)
		{
			return;
		}

		if (index >= m_size)
		{
			SetSize(index + 1, -1);
		}

		m_data[index] = obj;
	}

	uint32_t Append(const xArray& arr)
	{
		X_ASSERT(this != &arr);

		if (this == &arr)
		{
			return -1;
		}

		const uint32_t sizeOld = m_size;

		SetSize(m_size + arr.m_size);

		CopyElements(m_data + sizeOld, arr.m_data, arr.m_size);

		return sizeOld;
	}

	void Copy(const xArray& arr)
	{
		X_ASSERT(this != &arr);

		if (this == &arr)
		{
			return;
		}

		SetSize(arr.m_size);

		CopyElements(m_data, arr.m_data, arr.m_size);
	}

	void RemoveAt(const uint32_t index, const uint32_t count = 1)
	{
		X_ASSERT((index + count) <= m_size);

		if ((index + count) > m_size)
		{
			return;
		}

		const uint32_t move = m_size - (index + count);

		for (uint32_t ui = 0; ui < count; ui++)
		{
			(m_data + index + ui)->~t_Type();
		}

		if (move)
		{
			xMemoryMove(m_data + index, m_data + index + count, move * sizeof(t_Type));
		}

		m_size -= count;
	}

	void RemoveAll()
	{
		SetSize(0, -1);
	}

	void Compact()
	{
		if (m_size != m_sizeMax)
		{
			t_Type* data = NULL;

			if (m_size)
			{
				data = (t_Type*) xMemoryAlloc(m_size * sizeof(t_Type));

				xMemoryCopy(data, m_data, m_size * sizeof(t_Type));
			}

			xMemoryFree(m_data);

			m_data = data;

			m_sizeMax = m_size;
		}
	}

private:
	void CopyElements(t_Type* target, const t_Type* source, const uint32_t count)
	{
		X_ASSERT(count == 0 || target);

		X_ASSERT(count == 0 || source);

		uint32_t ui = count;

		while (ui--)
		{
			*target++ = *source++;
		}
	}

private:
	t_Type* m_data;
	uint32_t m_size;
	uint32_t m_sizeMax;
	uint32_t m_grow;
};

