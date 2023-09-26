#pragma once

#include "xArray.h"
#include "xBaseInterface.h"
#include "xChar.h"
#include "xReferenceCount.h"
#include "xString.h"



class xStringEnum : public xRefCount<IxStringEnum>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxStringEnum)
X_INTERFACE_MAP_END

public:
	xStringEnum()
	{
	}

	~xStringEnum()
	{
	}

public:
	X_INTERFACE_METHOD(uint32_t, GetCount)()
	{
		return m_arr.GetCount();
	}

	X_INTERFACE_METHOD(xResult, Get)(const uint32_t index, xChar** str)
	{
		X_ASSERT(str);

		if (!str)
		{
			return X_E_INVALID_ARGUMENT;
		}

		const uint32_t size = m_arr.GetCount();

		X_ASSERT(index < size);
		
		if (index >= size)
		{
			return X_E_INVALID_ARGUMENT;
		}

		return (*str = m_arr[index].Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
	}

	X_INTERFACE_METHOD(xResult, Add)(const xChar* str)
	{
		X_ASSERT(str);

		if (!str)
		{
			return X_E_INVALID_ARGUMENT;
		}

		m_arr.Add(str);

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, Add)(IxStringEnum* stringEnum)
	{
		X_ASSERT(stringEnum);

		if (!stringEnum)
		{
			return X_E_INVALID_ARGUMENT;
		}

		const uint32_t count = stringEnum->GetCount();

		for (uint32_t ui = 0; ui < count; ui++)
		{
			xPtr<xChar> s;

			xResult r = stringEnum->Get(ui, &s);

			if (xFailed(r) || 
				xFailed(r = Add(s)))
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
	xArray<xString> m_arr;
};
