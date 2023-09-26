#pragma once

#include "xDefinition.h"
#include "xFunctor.h"
#include "xHandle.h"
#include "xMemory.h"
#include "xResult.h"



typedef uint32_t(__X_STDCALL *_xThreadProc)(void*);



class xThread : public xHandle
{
private:
	typedef struct _xData
	{
		IxFunctor* functor;

		void* data;
	} xData;

public:
	xThread()
	{
		m_id = 0;
	}

	xResult Create(_xThreadProc pfn, void* data, const bool run = true, void* security = NULL)
	{
		X_ASSERT(pfn);

		if (!pfn)
		{
			return X_E_INVALID_ARGUMENT;
		}

		Attach(CreateThread((LPSECURITY_ATTRIBUTES) security, 0, (LPTHREAD_START_ROUTINE) pfn, data, run ? 0 : CREATE_SUSPENDED, (LPDWORD) &m_id));

		return IsValid() ? X_S_OK : xResultFromLastError();
	}

	xResult Create(IxFunctor* functor, void* data = NULL, const bool run = true, void* security = NULL)
	{
		X_ASSERT(functor);

		if (!functor)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xData* td = (xData*) xMemoryAlloc(sizeof(xData));

		if (!td)
		{
			return X_E_OUT_OF_MEMORY;
		}

		(td->functor = functor)->AddRef();

		td->data = data;

		xResult r = Create(_xThreadProc, td, run, security);

		if (xFailed(r))
		{
			(td->functor = functor)->Release();

			xMemoryFree(td);
		}

		return r;
	}

	xResult Suspend()
	{
		const bool valid = IsValid();

		X_ASSERT(valid);

		if (!valid)
		{
			return X_E_UNEXPECTED;
		}

		return xResultFromWin32(SuspendThread(GetSafeType()));
	}

	xResult Resume()
	{
		const bool valid = IsValid();

		X_ASSERT(valid);

		if (!valid)
		{
			return X_E_UNEXPECTED;
		}

		return xResultFromWin32(ResumeThread(GetSafeType()));
	}

	xResult SetPriority(const uint32_t priority)
	{
		return SetThreadPriority(GetSafeType(), priority) ? X_S_OK : xResultFromLastError();
	}

	xResult Terminate(const uint32_t exitCode)
	{
		const bool valid = IsValid();

		X_ASSERT(valid);

		if (!valid)
		{
			return X_E_UNEXPECTED;
		}

		return TerminateThread(GetSafeType(), exitCode) ? X_S_OK : xResultFromLastError();
	}

	uint32_t GetThreadID()
	{
		return m_id;
	}

	uint32_t GetExitCode()
	{
		const bool valid = IsValid();

		X_ASSERT(valid);
		
		if (!valid)
		{
			return 0;
		}
		
		uint32_t exitCode = 0;
		
		return GetExitCodeThread(GetSafeType(), (LPDWORD) &exitCode) ? exitCode : 0;
	}

	bool IsRunning()
	{
		return IsValid() && GetExitCode() == STILL_ACTIVE;
	}

	uint32_t WaitForCompletion(const uint32_t milliseconds)
	{
		if (IsRunning())
		{
			WaitForSingleObject(GetSafeType(), milliseconds);
		}

		return GetExitCode();
	}

private:
	static uint32_t __X_STDCALL _xThreadProc(void* data)
	{
		xThread::xData* d = static_cast<xThread::xData*>(data);

		X_ASSERT(d);

		if (!d)
		{
			return X_RESULT_CODE(X_E_INVALID_ARGUMENT);
		}

		xResult r = d->data ? d->functor->Call(d->data) : d->functor->Call();

		d->functor->Release();

		xMemoryFree(d);

		return X_RESULT_CODE(r);
	}

private:
	uint32_t m_id;
};
