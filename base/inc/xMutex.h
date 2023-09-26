#pragma once

#include <Windows.h>

#include "xChar.h"
#include "xHandle.h"
#include "xResult.h"



class xMutex : public xHandle
{
public:
	xMutex()
	{
		m_exists = false;
	}

public:
	xResult Create(const bool initialOwner, const xChar* name, void* security)
	{
		Attach(CreateMutex((LPSECURITY_ATTRIBUTES) security, initialOwner, name));

		const bool valid = IsValid();

		X_ASSERT(valid);

		if (valid)
		{
			m_exists = GetLastError() == ERROR_ALREADY_EXISTS;
		}

		return valid ? X_S_OK : xResultFromLastError();
	}

	bool Exists() const
	{
		return m_exists;
	}

	xResult Release()
	{
		X_ASSERT(IsValid());

		if (!IsValid())
		{
			return X_E_UNEXPECTED;
		}

		return ReleaseMutex(GetSafeType()) ? X_S_OK : xResultFromLastError();
	}

private:
	bool m_exists;
};
