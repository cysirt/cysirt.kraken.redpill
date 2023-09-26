#pragma once

#include <Windows.h>

#include "xChar.h"
#include "xHandle.h"
#include "xResult.h"



class xEvent  : public xHandle
{
public:
	xEvent ()
	{
		m_exists = false;
	}

public:
	xResult Create(const xChar* name, const bool manualReset, const bool initialState, void* security)
	{
		Attach(CreateEvent((LPSECURITY_ATTRIBUTES) security, manualReset, initialState, name));

		const bool valid = IsValid();

		X_ASSERT(valid);

		if (valid)
		{
			m_exists = ::GetLastError() == ERROR_ALREADY_EXISTS;
		}

		return valid ? X_S_OK : xResultFromLastError();
	}

	bool Exist() const
	{
		return m_exists;
	}

	xResult Set()
	{
		X_ASSERT(IsValid());

		if (!IsValid())
		{
			return X_E_UNEXPECTED;
		}

		return SetEvent(GetSafeType()) ? X_S_OK : xResultFromLastError();
	}

	xResult Reset()
	{
		X_ASSERT(IsValid());

		if (!IsValid())
		{
			return X_E_UNEXPECTED;
		}

		return ResetEvent(GetSafeType()) ? X_S_OK : xResultFromLastError();
	}

	xResult Pulse()
	{
		X_ASSERT(IsValid());

		if (!IsValid())
		{
			return X_E_UNEXPECTED;
		}

		return PulseEvent(GetSafeType()) ? X_S_OK : xResultFromLastError();
	}

	xResult WaitForSignal(const uint32_t milliseconds)
	{
		const bool valid = IsValid();

		X_ASSERT(valid);

		if (!valid)
		{
			return X_E_UNEXPECTED;
		}

		const uint32_t wait = (uint32_t) WaitForSingleObject(GetSafeType(), milliseconds);

		switch (wait)
		{
			case WAIT_OBJECT_0:
			{
				return X_S_OK;
			}
			case WAIT_TIMEOUT:
			{
				return X_E_TIMEOUT;
			}

			default:
			{
				return xResultFromLastError();
			}
		}
	}

private:
	bool m_exists;
};
