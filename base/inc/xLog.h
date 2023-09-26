#pragma once

#include <stdint.h>
#include <stdio.h>

#include "xBaseInterface.h"
#include "xCriticalSection.h"
#include "xInterfaceEnum.h"
#include "xResult.h"
#include "xString.h"



#define X_LOG						xLogGet().Log
#define X_LOG_INFO					xLogGet().LogInfo
#define X_LOG_WARNING				xLogGet().LogWarning
#define X_LOG_ERROR					xLogGet().LogError
#define X_LOG_HIGHLIGHT_GREEN		xLogGet().LogHighlightGreen
#define X_LOG_HIGHLIGHT_RED			xLogGet().LogHighlightRed
#define X_LOG_HIGHLIGHT_BLUE		xLogGet().LogHighlightBlue
#define X_LOG_RESULT				xLogGet().LogResult
#define X_LOG_BLANK					xLogGet().LogInfo(L"")
#define X_LOG_DEBUG					xLogGet().LogDebug
#define X_LOG_DEBUG_FUNCTION		X_LOG_DEBUG(__FUNCTION__)
#define X_LOG_DEBUG_LOCATION		X_LOG_DEBUG(L"File: %s, Line: %d", __FILE__, __LINE__)



class xLog : public xCriticalSection
{
private:
	xLog()
	{
		xCriticalSectionLock lock;

		m_flags = xLogCategoryInfo | xLogCategoryWarning | xLogCategoryError | xLogCategoryHighlightGreen | xLogCategoryHighlightRed | xLogCategoryHighlightBlue;
	}

public:
	~xLog()
	{
	}

	static xLog& Get()
	{
		static xLog s_self;

		return s_self;
	}

	xResult AddLogWriter(IxLogWriter* logWriter)
	{
		X_ASSERT(logWriter);

		if (!logWriter)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xCriticalSectionLock lock;

		xResult r = m_logWriters.Add(logWriter);

		return r;
	}

	void __X_CDECL Log(const xLogCategory category, const xChar* format, va_list args)
	{
		xCriticalSectionLock lock;

		if (!(m_flags & category))
		{
			return;
		}

		xString s;

		s.FormatV(format, args);

		LogInterface(category, s);
	}

	void __X_CDECL Log(const xLogCategory category, const xChar* format, ...)
	{
		xCriticalSectionLock lock;

		if (!(m_flags & category))
		{
			return;
		}

		va_list args;

		va_start(args, format);

		Log(category, format, args);

		va_end(args);
	}

	void __X_CDECL LogInfo(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryInfo, format, args);

		va_end(args);
	}

	void __X_CDECL LogWarning(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryWarning, format, args);

		va_end(args);
	}

	void __X_CDECL LogError(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryError, format, args);

		va_end(args);
	}

	void __X_CDECL LogDebug(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryDebug, format, args);

		va_end(args);
	}

	void __X_CDECL LogHighlightGreen(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryHighlightGreen, format, args);

		va_end(args);
	}

	void __X_CDECL LogHighlightRed(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryHighlightRed, format, args);

		va_end(args);
	}

	void __X_CDECL LogHighlightBlue(const xChar* format, ...)
	{
		va_list args;

		va_start(args, format);

		Log(xLogCategoryHighlightBlue, format, args);

		va_end(args);
	}

	void LogInterface(const xLogCategory category, const xChar* message)
	{
		xCriticalSectionLock lock;

		X_ASSERT(message);

		if (!message)
		{
			return;
		}

		xResult r = X_S_OK;

		const uint32_t count = m_logWriters.GetCount();

		for (uint32_t ui = 0; ui < count; ui++)
		{
			xInterfacePtr<IxLogWriter> log;

			if (xSucceeded(r = m_logWriters.Get(ui, (IxInterface**) &log)))
			{
				log->Write(category, message);
			}
		}
	}

private:
	uint32_t m_flags;
	xInterfaceEnum<IxInterfaceEnum> m_logWriters;
};

static xLog& xLogGet() { return xLog::Get(); }
