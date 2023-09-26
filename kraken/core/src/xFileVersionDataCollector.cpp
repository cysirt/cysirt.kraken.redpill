#include "xFileVersionDataCollector.h"

#include "xLog.h"
#include "xMemory.h"
#include "xString.h"

#include "xDataCache.h"
#include "xFileVersionData.h"



xFileVersionDataCollector::xFileVersionDataCollector()
{

}

xFileVersionDataCollector::~xFileVersionDataCollector()
{

}

xResult xFileVersionDataCollector::Collect(const xChar* fileName, IxFileVersionData** fileVersionData)
{
	X_ASSERT(fileName && fileVersionData);

	if (!fileName || !fileVersionData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxFileVersionData, &iface);

	xInterfacePtr<xFileVersionData> fvd;

	if (xSucceeded(r))
	{
		r = iface->QueryInterface(IID_IxFileVersionData, (void**) &fvd);
	}
	else
	{
		DWORD dw = 0;

		const DWORD size = GetFileVersionInfoSize(fileName, &dw);

		if (!size)
		{
			return xResultFromLastError();
		}

		xPtr<uint8_t> fvi((uint8_t*) xMemoryAlloc(size));

		if (!fvi)
		{
			return X_E_OUT_OF_MEMORY;
		}

		if (!GetFileVersionInfo(fileName, dw, size, fvi))
		{
			return xResultFromLastError();
		}

		typedef struct _xLangCodePage
		{
			WORD language;
			WORD codePage;
		} xLangCodePage;

		xLangCodePage* lcp = NULL;

		uint32_t sizeInfo = 0;

		if (!VerQueryValue(fvi, X_CHAR("\\VarFileInfo\\Translation"), (void**) &lcp, &sizeInfo))
		{
			return xResultFromLastError();
		}

		const xChar* info[] =
		{
			X_CHAR("Comments"),
			X_CHAR("InternalName"),
			X_CHAR("ProductName"),
			X_CHAR("CompanyName"),
			X_CHAR("LegalCopyright"),
			X_CHAR("ProductVersion"),
			X_CHAR("FileDescription"),
			X_CHAR("LegalTrademarks"),
			X_CHAR("PrivateBuild"),
			X_CHAR("FileVersion"),
			X_CHAR("OriginalFilename"),
			X_CHAR("SpecialBuild")
		};

		fvd = new xFileVersionData;

		if (!fvd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		for (uint32_t ui = 0; ui < (sizeInfo / sizeof(xLangCodePage)); ui++)
		{
			for (uint32_t uj = 0; uj < (sizeof(info) / sizeof(xChar*)); uj++)
			{
				xString query;

				query.Format(X_CHAR("\\StringFileInfo\\%04x%04x\\%s"), lcp[ui].language, lcp[ui].codePage, info[uj]);

				uint32_t sizeValue = 0;

				xChar* value = NULL;

				if (!VerQueryValue(fvi, query, (void**) &value, &sizeValue))
				{
					continue;
				}

				switch (uj)
				{
					case 0x00:
					{
						fvd->SetComments(value);

						break;
					}
					case 0x01:
					{
						fvd->SetInternalName(value);

						break;
					}
					case 0x02:
					{
						fvd->SetProductName(value);

						break;
					}
					case 0x03:
					{
						fvd->SetCompanyName(value);

						break;
					}
					case 0x04:
					{
						fvd->SetLegalCopyright(value);

						break;
					}
					case 0x05:
					{
						fvd->SetProductVersion(value);

						break;
					}
					case 0x06:
					{
						fvd->SetFileDescription(value);

						break;
					}
					case 0x07:
					{
						fvd->SetLegalTrademarks(value);

						break;
					}
					case 0x08:
					{
						fvd->SetPrivateBuild(value);

						break;
					}
					case 0x09:
					{
						fvd->SetFileVersion(value);

						break;
					}
					case 0x0a:
					{
						fvd->SetOriginalFileName(value);

						break;
					}
					case 0x0b:
					{
						fvd->SetSpecialBuild(value);

						break;
					}
					default:
					{
						X_ASSERT(false);

						break;
					}
				}
			}
		}

		r = xDataCacheGet().Add(fileName, IID_IxFileVersionData, fvd);
	}

	if (xSucceeded(r))
	{
		const uint32_t count = fvd->GetCount();

		if (count)
		{
			*fileVersionData = fvd.Detach();
		}
		else
		{
			X_LOG_WARNING(X_CHAR("File version information for \"%s\" present but empty"), fileName);
		}

		return count ? X_S_OK : X_E_NO_DATA;
	}

	return r;
}
