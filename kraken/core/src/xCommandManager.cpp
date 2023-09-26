#include "xCommandManager.h"

#include <io.h>
#include <WinInet.h>

#include "xElevation.h"
#include "xEnvironment.h"
#include "xFile.h"
#include "xInterfaceEnum.h"
#include "xLog.h"
#include "xPath.h"
#include "xString.h"

#include "xCommandManagerOption.h"
#include "xCompression.h"
#include "xDataCollector.h"
#include "xDataContainer.h"
#include "xJsonStorageProvider.h"



#define X_TIME_SPAN(start, end)	(double) ((double) (end - start) / 1000.0)



xCommandManager::xCommandManager()
{
}

xCommandManager::~xCommandManager()
{
}

xResult xCommandManager::Execute(IxCommandManagerOption* option)
{
	X_ASSERT(option);

	if (!option)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	uint32_t flags = 0;
	
	if (xFailed(r = option->GetFlags(&flags)))
	{
		return r;
	}

	xString permissions(X_CHAR("Permissions: "));

	if (xElevationGet().IsElevated())
	{
		permissions += X_CHAR("elevated");

		if (xSucceeded(xElevationGet().EnableDebugPrivilege(true)))
		{
			permissions += X_CHAR(", debug");
		}
	}
	else
	{
		permissions += X_CHAR("restricted");
	}

	X_LOG_INFO(permissions);

	X_LOG_BLANK;

	uint64_t start = GetTickCount64();

	uint64_t sectionStart = start;

	xDataCollector dc;

	xInterfacePtr<xDataContainer> container;

	if (flags & xCommandManagerOptionFlagCollect)
	{
		uint32_t collectFlags = 0;
		
		if (xFailed(r = option->GetCollectFlags(&collectFlags)))
		{
			return r;
		}

		container = new xDataContainer;

		if (!container)
		{
			return X_E_OUT_OF_MEMORY;
		}

		xInterfacePtr<IxMetaData> metaData;

		if (xSucceeded(r = dc.CollectMetaData(&metaData)))
		{
			container->SetMetaData(metaData);
		}

		if (collectFlags & xCommandManagerCollectFlagOperatingSystemData)
		{
			xInterfacePtr<IxOperatingSystemData> operatingSystemData;

			X_LOG_INFO(X_CHAR("Collecting operating system data"));

			sectionStart = GetTickCount64();

			if (xSucceeded(r = dc.CollectOperatingSystemData(&operatingSystemData)))
			{
				container->SetOperatingSystemData(operatingSystemData);

				X_LOG_INFO(X_CHAR("Finished collecting operating system data (processing time: %f seconds)"), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect operating system data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagUserAccountData)
		{
			xInterfacePtr<IxUserAccountDataEnum> userAccountDataEnum;

			X_LOG_INFO(X_CHAR("Collecting user account data"));

			sectionStart = GetTickCount64();

			if (xSucceeded(r = dc.CollectUserAccountData(&userAccountDataEnum)))
			{
				container->SetUserAccountData(userAccountDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting user account data (processing time: %f seconds)"), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect user account data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagProcessData)
		{
			xInterfacePtr<IxProcessDataEnum> processDataEnum;

			X_LOG_INFO(X_CHAR("Collecting process data"));

			sectionStart = GetTickCount64();

			if (xSucceeded(r = dc.CollectProcessData(&processDataEnum)))
			{
				container->SetProcessData(processDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting process data (%d processes, processing time: %f seconds)"), processDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect process data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagRegistryData)
		{
			xInterfacePtr<IxRegistryKeyDataEnum> registryKeyDataEnum;

			X_LOG_INFO(X_CHAR("Collecting registry data"));

			sectionStart = GetTickCount64();

			if (xSucceeded(r = dc.CollectRegistryData(&registryKeyDataEnum)))
			{
				container->SetRegistryData(registryKeyDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting registry data (%d hives, processing time: %f seconds)"), registryKeyDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect registry data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagDriverData)
		{
			xInterfacePtr<IxDriverDataEnum> driverDataEnum;

			X_LOG_INFO(X_CHAR("Collecting driver data"));

			sectionStart = GetTickCount64();

			if (xSucceeded(r = dc.CollectDriverData(&driverDataEnum)))
			{
				container->SetDriverData(driverDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting driver data (%d drivers, processing time: %f seconds)"), driverDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect driver data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagPrefetchData)
		{
			if (xElevationGet().IsElevated())
			{
				// If running elevated we acquire prefetch data (prefetch data is not accessible otherwise)
				xInterfacePtr<IxPrefetchDataEnum> prefetchDataEnum;

				X_LOG_INFO(X_CHAR("Collecting prefetch data"));

				sectionStart = GetTickCount64();

				if (xSucceeded(r = dc.CollectPrefetchData(&prefetchDataEnum)))
				{
					container->SetPrefetchData(prefetchDataEnum);

					X_LOG_INFO(X_CHAR("Finished collecting prefetch data (%d prefetch files, processing time: %f seconds)"), prefetchDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
				}
				else
				{
					X_LOG_WARNING(X_CHAR("Failed to collect prefetch data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
				}
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Elevation required to collect prefetch data - skipped"));
			}

			X_LOG_BLANK;
		}
/*
		xInterfacePtr<IxNetworkData> networkData;

		X_LOG_INFO(L"Collecting network data");

		sectionStart = GetTickCount64();

		if (xSucceeded(r = xDataCollectorGet().GetNetworkData(&networkData)))
		{
			data->Add(networkData);

			X_LOG_INFO(L"Finished collecting network data (processing time: %f seconds)", X_TIME_SPAN(sectionStart, GetTickCount64()));
		}
		else
		{
			X_LOG_WARNING(L"Failed to collect network data (code: " X_RESULT_FORMAT_STRING L")");
		}

		X_LOG_BLANK;
*/
		if (collectFlags & xCommandManagerCollectFlagConnectionData)
		{
			X_LOG_INFO(X_CHAR("Collecting network connection data"));

			sectionStart = GetTickCount64();

			xInterfacePtr<IxNetworkConnectionDataEnum> networkConnectionDataEnum;

			if (xSucceeded(r = dc.CollectNetworkConnectionData(&networkConnectionDataEnum)))
			{
				container->SetNetworkConnectionData(networkConnectionDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting connection data (%d network connections, processing time: %f seconds)"), networkConnectionDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect connection data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}
		
			X_LOG_BLANK;
		}

		if (collectFlags & xCommandManagerCollectFlagDnsCacheData)
		{
			X_LOG_INFO(X_CHAR("Collecting DNS cache data"));

			sectionStart = GetTickCount64();

			xInterfacePtr<IxDnsCacheDataEnum> dnsCacheDataEnum;

			if (xSucceeded(r = dc.CollectDnsCacheData(&dnsCacheDataEnum, !!(flags & xCommandManagerOptionFlagNoNetwork))))
			{
				container->SetDnsCacheData(dnsCacheDataEnum);

				X_LOG_INFO(X_CHAR("Finished collecting DNS cache data (%d DNS cache entries, processing time: %f seconds)"), dnsCacheDataEnum->GetCount(), X_TIME_SPAN(sectionStart, GetTickCount64()));
			}
			else
			{
				X_LOG_WARNING(X_CHAR("Failed to collect DNS cache data (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), r);
			}

			X_LOG_BLANK;
		}
	}

	xInterfacePtr<IxStorageProvider> storageProvider;

	if (flags & xCommandManagerOptionFlagOutput)
	{
		xPtr<xChar> outputFileName;

		if (xFailed(r = option->GetOutputFileName(&outputFileName)))
		{
			return r;
		}

		xPtr<xChar> outputPath;

		if (xFailed(r = xPathRemoveLast(outputFileName, &outputPath)))
		{
			return r;
		}

		xPtr<xChar> ofnj;

		if (xFailed(r = GetOutputFileName(outputPath, X_CHAR("json"), &ofnj)))
		{
			return r;
		}

		xPtr<xChar> outputFileNameJson;

		if (xFailed(r = xPathAppend(outputPath, ofnj, &outputFileNameJson)))
		{
			return r;
		}

		storageProvider = new xJsonStorageProvider;

		if (!storageProvider)
		{
			return X_E_OUT_OF_MEMORY;
		}

		storageProvider->SetFileName(outputFileNameJson);

		if (container)
		{
			X_LOG_INFO(X_CHAR("Writing JSON output to file: %s"), outputFileNameJson);

			sectionStart = GetTickCount64();

			if (xFailed(r = storageProvider->Save(container)))
			{
				X_LOG_ERROR(X_CHAR("Failed to write JSON output file %s (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), outputFileNameJson, r);
			}
			else
			{
				X_LOG_INFO(X_CHAR("Finished writing JSON output file (processing time: %f seconds)"), X_TIME_SPAN(start, GetTickCount64()));
			}

			X_LOG_BLANK;
		}

		X_LOG_INFO(X_CHAR("Writing zip compressed output to file: %s"), outputFileName);

		sectionStart = GetTickCount64();

		xCompression zip;

		if (xFailed(r = zip.Add(outputFileNameJson)) ||
			xFailed(r = zip.Compress(outputFileName)))
		{
			X_LOG_ERROR(X_CHAR("Failed to write zip compressed output file %s (code: ") X_RESULT_FORMAT_STRING X_CHAR(")"), outputFileName, r);
		}
		else
		{
			X_LOG_INFO(X_CHAR("Finished writing zip compressed output file (processing time: %f seconds)"), X_TIME_SPAN(sectionStart, GetTickCount64()));

			xFileDelete(outputFileNameJson);
		}

		X_LOG_BLANK;
	}

	X_LOG_INFO(X_CHAR("Finished (total processing time: %f seconds)"), X_TIME_SPAN(start, GetTickCount64()));

	return r;
}

xResult xCommandManager::GetOutputFileName(const xChar* path, const xChar* extension, xChar** fileName)
{
	X_ASSERT(path && extension && fileName);

	if (!path || !extension || !fileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xPtr<xChar> hostName;

	xResult r = xEnvironmentGet().GetHostName(&hostName);

	if (xFailed(r))
	{
		return r;
	}

	SYSTEMTIME st;

	GetSystemTime(&st);

	xString fn;

	fn.Format(X_CHAR("%u-%02u-%02u_%02u-%02u-%02u_%s.%s"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, hostName, extension);

	return (*fileName = fn.Clone()) ? X_S_OK : X_E_OUT_OF_MEMORY;
}

xResult xCommandManager::UploadFile(const xChar* fileName, const xChar* url)
{
	X_ASSERT(fileName && url);

	if (!fileName || !url)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	HINTERNET session = InternetOpen(X_CHAR("CyberArmyKnife"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (session)
	{
		HINTERNET connection = InternetConnect(session, url, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

		if (connection)
		{
			HINTERNET request = HttpOpenRequest(connection, X_CHAR("POST"), fileName, HTTP_VERSION, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION && INTERNET_FLAG_NO_CACHE_WRITE, 0);

			if (request)
			{
				FILE* file = xFileOpen(fileName, X_CHAR("rb"));

				INTERNET_BUFFERS ib = { 0 };

				ib.dwStructSize = sizeof(INTERNET_BUFFERS);

				ib.dwBufferTotal = (DWORD) xFileGetSize(file);

				if (HttpSendRequestEx(request, NULL, 0, NULL, 0))
				{
					uint32_t read = 0;

					DWORD written = 0;

					uint8_t buffer[4096];

					do
					{
						read = (uint32_t) xFileRead(buffer, sizeof(buffer), file);

						if (!read)
						{
							break;
						}

						if (!InternetWriteFile(request, buffer, read, &written))
						{
							break;
						}

					} while (read == written);
				}

				InternetCloseHandle(request);

				xFileClose(file);
			}
			else
			{
				r = xResultFromLastError();
			}

			InternetCloseHandle(connection);
		}
		else
		{
			r = xResultFromLastError();
		}

		InternetCloseHandle(session);
	}
	else
	{
		r = xResultFromLastError();
	}

	return r;
}
