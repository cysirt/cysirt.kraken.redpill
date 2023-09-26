#pragma once

#include "xFile.h"
#include "xMemory.h"
#include "xPropertyEnum.h"
#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xLogWriterFile : public xPropertyEnum<xRefCount<IxLogWriter> >
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxLogWriter)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(FileName, xChar*)
X_PROPERTY_MAP_END

public:
	xLogWriterFile()
	{
	}

	~xLogWriterFile()
	{
	}

	X_INTERFACE_METHOD(xResult, Write)(xLogCategory category, const xChar* message)
	{
		X_ASSERT(message);

		if (!message)
		{
			return X_E_INVALID_ARGUMENT;
		}

		xPtr<xChar> fileName;

		xResult r = GetFileName(&fileName);

		if (xFailed(r))
		{
			return r;
		}

		FILE* file = xFileOpen(fileName, L"a, ccs=UNICODE");

		if (!file)
		{
			return xResultFromErrNo();
		}

		const xChar* tag = NULL;

		switch (category)
		{
			case xLogCategoryInfo:
			{
				tag = L"[INFORMATION]";
				
				break;
			}
			case xLogCategoryWarning:
			{
				tag = L"[WARNING]";

				break;
			}
			case xLogCategoryError:
			{
				tag = L"[ERROR]";

				break;
			}
			case xLogCategoryHighlightGreen:
			case xLogCategoryHighlightRed:
			case xLogCategoryHighlightBlue:
			{
				tag = L"[IMPORTANT]";

				break;
			}
			case xLogCategoryDebug:
			{
				tag = L"[DEBUG]";

				break;
			}
			default:
			{
				X_ASSERT(false);
			}
		}

		fwprintf(file, L"%s %s\r\n", tag, message);

		xFileClose(file);

		return X_S_OK;
	}
};

