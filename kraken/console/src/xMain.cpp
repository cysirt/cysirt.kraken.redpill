#include <SDKDDKVer.h>

#include <tchar.h>
#include <Windows.h>
#include <Objbase.h>

#include "SimpleOpt.h"

#include "xLog.h"
#include "xLogWriterConsole.h"
#include "xPath.h"
#include "xReferenceCount.h"
#include "xResult.h"

#include "xCoreObjectFactory.h"



#ifdef _WIN64
	#define X_PRODUCT_NAME L"cysirt-kraken64"
#else
	#define X_PRODUCT_NAME L"cysirt-kraken32"
#endif



CSimpleOpt::SOption g_arguments[] =
{
	{ xCommandManagerOptionFlagCollectFull,		L"--collect-full", SO_NONE },
	{ xCommandManagerOptionFlagCollectQuick,	L"--collect-quick", SO_NONE },
	{ xCommandManagerOptionFlagCollect,			L"--collect", SO_MULTI },
	{ xCommandManagerOptionFlagOutput,			L"--output-filename", SO_REQ_SEP },
	{ xCommandManagerOptionFlagUpload,			L"--upload", SO_REQ_SEP },
	{ xCommandManagerOptionFlagNoNetwork,		L"--no-network", SO_NONE },
	{ xCommandManagerOptionFlagLogMode,			L"--log-mode", SO_REQ_SEP },
	{ xCommandManagerOptionFlagLogFile,			L"--log-file", SO_REQ_SEP },
	{ xCommandManagerOptionFlagHelp,			L"-h", SO_NONE },
	{ xCommandManagerOptionFlagHelp,			L"--help", SO_NONE },
	{ xCommandManagerOptionFlagHelp,			L"-?", SO_NONE },
	{ xCommandManagerOptionFlagAbout,			L"--about", SO_NONE },
	SO_END_OF_OPTIONS
};

#define X_COMMAND_MANAGER_COLLECT_FLAG_CONNECTION_DATA			L"connection-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_DNS_CACHE_DATA			L"dns-cache-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_PREFETCH_DATA			L"prefetch-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_PROCESS_DATA				L"process-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_REGISTRY_DATA			L"registry-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_OPERATING_SYSTEM_DATA	L"operating-system-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_USER_ACCOUNT_DATA		L"user-account-data"
#define X_COMMAND_MANAGER_COLLECT_FLAG_DRIVER_DATA				L"driver-data"



bool IsWow64();

void ShowAbout(IxCore* core);
void ShowOptions();
void ShowLogo();
void ShowVersion(IxCore* core);
void ShowHeader(IxCore* core);
void ShowLegal();

xResult Execute(int argc, wchar_t* argv[]);



int wmain(int argc, wchar_t* argv[])
{
	// Setup console logging
	xInterfacePtr<IxLogWriter> lw(new xLogWriterConsole);

	if (lw)
	{
		xLogGet().AddLogWriter(lw);
	}

	// Ensure appropriate bit size
	if (IsWow64())
	{
		X_LOG_HIGHLIGHT_RED(L"Your running " X_PRODUCT_NAME L" on 64-bit Windows. Please use the 64-bit version.");

		return 0;
	}

	// Setup environment
	xResult r = xResultFromHresult(CoInitializeEx(0, COINIT_MULTITHREADED));

	if (xSucceeded(r))
	{
		// Setup security
		if (xSucceeded(r = xResultFromHresult(CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL))))
		{
			Execute(argc, argv);
		}
		else
		{
			X_LOG_ERROR(L"Security initialization failed (code: " X_RESULT_FORMAT_STRING L")", r);
		}

		CoUninitialize();
	}
	else
	{
		X_LOG_ERROR(L"COM initialization failed (code: " X_RESULT_FORMAT_STRING L")", r);
	}

#ifdef _DEBUG
	getchar();
#endif // _DEBUG

	return X_RESULT_CODE(r);
}

bool IsWow64()
{
#ifdef _WIN32
	BOOL wow64 = FALSE;

	if (IsWow64Process(GetCurrentProcess(), &wow64) && wow64)
	{
		return true;
	}
#endif

	return false;
}

void ShowAbout(IxCore* core)
{
	ShowHeader(core);
}

void ShowOptions()
{
	X_LOG_INFO(L"Usage:");
	X_LOG_BLANK;
	X_LOG_INFO(L"--help -h -?");
	X_LOG_INFO(L"  Show this message");
	X_LOG_BLANK;
	X_LOG_INFO(L"--collect-full (collect full data set)");
	X_LOG_INFO(L"  Contains:");
	X_LOG_INFO(L"    operating-system-data");
	X_LOG_INFO(L"    user-account-data");
	X_LOG_INFO(L"    process-data");
	X_LOG_INFO(L"    driver-data");
	X_LOG_INFO(L"    connection-data");
	X_LOG_INFO(L"    prefetch-data");
	X_LOG_INFO(L"    dns-cache-data");
	X_LOG_BLANK;
	X_LOG_INFO(L"--collect-quick (collect limited data set)");
	X_LOG_INFO(L"  Contains:");
	X_LOG_INFO(L"    operating-system-data");
	X_LOG_INFO(L"    user-account-data");
	X_LOG_INFO(L"    process-data");
	X_LOG_INFO(L"    driver-data");
	X_LOG_INFO(L"    connection-data");
	X_LOG_INFO(L"    dns-cache-data");
	X_LOG_BLANK;
	X_LOG_INFO(L"--collect n option-1 option-2 ... option-n (collect specified data set)");
	X_LOG_INFO(L"  Options:");
	X_LOG_INFO(L"    operating-system-data");
	X_LOG_INFO(L"    user-account-data");
	X_LOG_INFO(L"    process-data");
	X_LOG_INFO(L"    driver-data");
	X_LOG_INFO(L"    connection-data");
	X_LOG_INFO(L"    prefetch-data");
	X_LOG_INFO(L"    dns-cache-data");
	X_LOG_INFO(L"    sample: --collect 3 operating-system-data user-account-data process-data");
	X_LOG_BLANK;
	X_LOG_INFO(L"--output-filename \"path\\filename\"");
	X_LOG_INFO(L"  Store data in the specified output file");
	X_LOG_BLANK;
	X_LOG_INFO(L"--about");
	X_LOG_INFO(L"  Show information about this application");
	X_LOG_BLANK;
}

void ShowLogo()
{

	X_LOG_HIGHLIGHT_GREEN(L"=====================================================================");
	X_LOG_HIGHLIGHT_GREEN(L"       _________ _____.___. _________.________________________       ");
	X_LOG_HIGHLIGHT_GREEN(L"       \\_   ___ \\\\__  |   |/   _____/|   \\______   \\__    ___/       ");
	X_LOG_HIGHLIGHT_GREEN(L"       /    \\  \\/ /   |   |\\_____  \\ |   ||       _/ |    |          ");
	X_LOG_HIGHLIGHT_GREEN(L"       \\     \\____\\____   |/        \\|   ||    |   \\ |    |          ");
	X_LOG_HIGHLIGHT_GREEN(L"        \\______  // ______/_______  /|___||____|_  / |____|          ");
	X_LOG_HIGHLIGHT_GREEN(L"               \\/ \\/              \\/             \\/                  ");
	X_LOG_HIGHLIGHT_GREEN(L"---------------------------------------------------------------------");
	X_LOG_HIGHLIGHT_GREEN(L"                                KRAKEN                               ");
	X_LOG_HIGHLIGHT_GREEN(L"=====================================================================");
}

void ShowVersion(IxCore* core)
{
	X_ASSERT(core);

	if (!core)
	{
		return;
	}

	xString fileName;

	GetModuleFileName(NULL, fileName.GetBuffer(X_PATH_MAX), X_PATH_MAX);

	fileName.ReleaseBuffer();

	xInterfacePtr<IxDataCollector> dc;

	xResult r = core->CreateDataCollector(&dc);

	xInterfacePtr<IxFileVersionData> fvd;

	if (xSucceeded(r) &&
		xSucceeded(r = dc->CollectFileVersionData(fileName, &fvd)))
	{
		xPtr<xChar> fileDescription;

		if (xSucceeded(r = fvd->GetFileDescription(&fileDescription)))
		{
			DWORD binaryType = 0;

			const xChar* architecture = NULL;

			if (GetBinaryType(fileName, &binaryType))
			{
				switch (binaryType)
				{
					case SCS_32BIT_BINARY:
					{
						architecture = X_CHAR("32-bit");

						break;
					}
					case SCS_64BIT_BINARY:
					{
						architecture = X_CHAR("64-bit");

						break;
					}
					default:
					{
						X_ASSERT(false);

						architecture = X_CHAR_EMPTY;

						break;
					}
				}
			}

			X_LOG_INFO(X_CHAR("%s (%s)"), fileDescription, architecture);
		}

		xPtr<xChar> fileVersion;

		if (xSucceeded(r = fvd->GetFileVersion(&fileVersion)))
		{
			X_LOG_INFO(X_CHAR("Version: %s"), fileVersion);
		}

		xPtr<xChar> copyright;

		if (xSucceeded(r = fvd->GetLegalCopyright(&copyright)))
		{
			X_LOG_INFO(copyright);
		}

		X_LOG_INFO(X_CHAR("http://cysirt.com/kraken"));
	}
}

void ShowHeader(IxCore* core)
{
	ShowLogo();

	X_LOG_BLANK;

	ShowVersion(core);

	X_LOG_BLANK;

	ShowLegal();

	X_LOG_BLANK;
}

void ShowLegal()
{
#ifdef X_CYSIRT_KRAKEN_DEMO
	X_LOG_HIGHLIGHT_RED(L"You are running the DEMO version of " X_PRODUCT_NAME);
#elif defined X_CYBERARMYKNIFE_DEMO && !defined _DEBUG 
	X_LOG_BLANK;

	while (true)
	{
		xLogGet().LogInterface(xLogCategoryInfo, L"Please confirm that you have read and agree to the license terms by typing \"YES\" or exit by typing \"NO\": ");

		xString answer;

		if (fgetws(answer.GetBuffer(4), 4, stdin))
		{
			answer.ReleaseBuffer();

			answer.TrimRight(L'\n');

			if (xCharCompareNoCase(answer, L"yes") == 0)
			{
				return;
			}
			else if (xCharCompareNoCase(answer, L"no") == 0)
			{
				ExitProcess(ERROR_EVALUATION_EXPIRATION);
			}
		}
		else
		{
			answer.ReleaseBuffer();

			ExitProcess(errno);
		}
	}
#else
	X_LOG_HIGHLIGHT_RED(L"You are running the COMMERCIAL version of " X_PRODUCT_NAME);
#endif // X_CYSIRT_KRAKEN_DEMO && !_DEBUG
}

xResult Execute(int argc, wchar_t* argv[])
{
	xInterfacePtr<IxCore> core;

	xResult r = xCoreObjectFactoryGet().GetCore(&core);

	if (xSucceeded(r))	
	{
		ShowHeader(core);

		xInterfacePtr<IxCommandManager> cm;

		if (xSucceeded(r = core->CreateCommandManager(&cm)))
		{
			xInterfacePtr<IxCommandManagerOption> cmo;

			if (xSucceeded(r = core->CreateCommandManagerOption(&cmo)))
			{
				// We're good to handle commands
				CSimpleOpt args(argc, argv, g_arguments);

				uint32_t flags = 0;

				while (args.Next())
				{
					if (args.LastError() == SO_SUCCESS)
					{
						if (args.OptionId() == xCommandManagerOptionFlagHelp)
						{
							ShowOptions();

							return X_S_OK;
						}
						else if (args.OptionId() == xCommandManagerOptionFlagAbout)
						{
							ShowAbout(core);

							return X_S_OK;
						}
						else if (args.OptionId() == xCommandManagerOptionFlagCollectQuick)
						{
							flags |= xCommandManagerOptionFlagCollect;

							cmo->SetCollectFlags(xCommandManagerCollectFlagQuick);
						}
						else if (args.OptionId() == xCommandManagerOptionFlagCollectFull)
						{
							flags |= xCommandManagerOptionFlagCollect;

							cmo->SetCollectFlags(xCommandManagerCollectFlagFull);
						}
						else if (args.OptionId() == xCommandManagerOptionFlagCollect)
						{
							flags |= xCommandManagerOptionFlagCollect;

							const uint32_t countOptions = xCharToInt(args.MultiArg(1)[0]);

							if (!countOptions)
							{
								X_LOG_ERROR(L"Command line error: No option count specified");

								return X_E_INVALID_ARGUMENT;
							}

							xChar** options = args.MultiArg(countOptions);

							if (!options)
							{
								X_LOG_ERROR(L"Command line error: No option arguments specified");

								return X_E_INVALID_ARGUMENT;
							}

							uint32_t optionFlags = 0;

							for (uint32_t ui = 0; ui < countOptions; ui++)
							{
								if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_OPERATING_SYSTEM_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagOperatingSystemData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_USER_ACCOUNT_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagUserAccountData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_PROCESS_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagProcessData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_REGISTRY_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagRegistryData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_DRIVER_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagDriverData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_CONNECTION_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagConnectionData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_PREFETCH_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagPrefetchData;
								}
								else if (xCharCompareNoCase(options[ui], X_COMMAND_MANAGER_COLLECT_FLAG_DNS_CACHE_DATA) == 0)
								{
									optionFlags |= xCommandManagerCollectFlagDnsCacheData;
								}
								else
								{
									X_LOG_ERROR(L"Command line error: Invalid option argument specified (%s)", args.OptionArg());

									return X_E_INVALID_ARGUMENT;
								}
							}

							cmo->SetCollectFlags(optionFlags);
						}
						else if (args.OptionId() == xCommandManagerOptionFlagNoNetwork)
						{
							flags |= xCommandManagerOptionFlagNoNetwork;
						}
						else if (args.OptionId() == xCommandManagerOptionFlagLogMode)
						{
							flags |= xCommandManagerOptionFlagLogMode;

							if (xCharCompareNoCase(args.OptionArg(), L"debug") == 0)
							{
								cmo->SetLogMode(xLogCategoryDebug);
							}
							else if (xCharCompareNoCase(args.OptionArg(), L"silent") == 0)
							{
								cmo->SetLogMode(xLogCategorySilent);
							}
						}
						else if (args.OptionId() == xCommandManagerOptionFlagLogFile)
						{
							flags |= xCommandManagerOptionFlagLogFile;

							cmo->SetLogFileName(args.OptionArg());
						}
						else if (args.OptionId() == xCommandManagerOptionFlagOutput)
						{
							flags |= xCommandManagerOptionFlagOutput;

							cmo->SetOutputFileName(args.OptionArg());
						}
						else if (args.OptionId() == xCommandManagerOptionFlagUpload)
						{
							flags |= xCommandManagerOptionFlagUpload;

							cmo->SetUploadUrl(args.OptionArg());
						}
					}
				}

				if (!flags)
				{
					ShowOptions();

					return X_S_FALSE;
				}

				cmo->SetFlags(flags);

				r = cm->Execute(cmo);
			}
		}
		else
		{
			X_LOG_ERROR(L"CommandManager initialization failed (code: " X_RESULT_FORMAT_STRING L")", r);
		}
	}
	else
	{
		X_LOG_ERROR(L"Core initialization failed (code: " X_RESULT_FORMAT_STRING L")", r);
	}

	return r;
}
