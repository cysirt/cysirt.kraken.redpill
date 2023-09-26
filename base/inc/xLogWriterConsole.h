#pragma once

#include "xMemory.h"
#include "xReferenceCount.h"

#include "xBaseInterface.h"



#define X_LOG_WRITER_CONSOLE_FORMAT L"%s\n"



class xLogWriterConsole : public xRefCount<IxLogWriter>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxLogWriter)
X_INTERFACE_MAP_END

public:
	xLogWriterConsole()
	{
	}

	~xLogWriterConsole()
	{
	}

	X_INTERFACE_METHOD(xResult, Write)(xLogCategory category, const xChar* message)
	{
		X_ASSERT(message);

		if (!message)
		{
			return X_E_INVALID_ARGUMENT;
		}

		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!h)
		{
			return xResultFromLastError();
		}

		CONSOLE_SCREEN_BUFFER_INFO csbi;

		xMemoryZero(&csbi, sizeof(csbi));

		GetConsoleScreenBufferInfo(h, &csbi);

		uint16_t attributes = csbi.wAttributes;

		switch (category)
		{
			case xLogCategoryInfo:
			{
				attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

				break;
			}
			case xLogCategoryWarning:
			{
				attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

				break;
			}
			case xLogCategoryError:
			{
				attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;

				break;
			}
			case xLogCategoryDebug:
			{
				attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;

				break;
			}
			case xLogCategoryHighlightGreen:
			{
				attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_GREEN;

				break;
			}
			case xLogCategoryHighlightRed:
			{
				attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY;

				break;
			}
			case xLogCategoryHighlightBlue:
			{
				attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY;

				break;
			}
		}

		if (attributes)
		{
			SetConsoleTextAttribute(h, attributes);
		}

		xString s;

		s.Format(X_LOG_WRITER_CONSOLE_FORMAT, message);

		wprintf(s);

		SetConsoleTextAttribute(h, csbi.wAttributes);

		return X_S_OK;
	}

private:
	xResult SetConsoleAttributes(xLogCategory category)
	{
		return X_E_INTERNAL;
	}
};

