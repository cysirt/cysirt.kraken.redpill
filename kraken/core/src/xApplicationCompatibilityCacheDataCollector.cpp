#include "xApplicationCompatibilityCacheDataCollector.h"

#include "xLog.h"
#include "xMemoryStream.h"
#include "xStreamReader.h"



xApplicationCompatibilityCacheDataCollector::xApplicationCompatibilityCacheDataCollector()
{

}

xApplicationCompatibilityCacheDataCollector::~xApplicationCompatibilityCacheDataCollector()
{

}

xResult xApplicationCompatibilityCacheDataCollector::Collect(IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	X_ASSERT(applicationCompatibilityCacheData);

	if (!applicationCompatibilityCacheData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	HKEY keyCache = NULL;

	xResult r = xResultFromWin32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Session Manager\\AppCompatCache"), 0, KEY_QUERY_VALUE, &keyCache));

	if (xFailed(r) &&
		xFailed(r = xResultFromWin32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Session Manager\\AppCompatibility"), 0, KEY_QUERY_VALUE, &keyCache))))
	{
		return r;
	}

	DWORD size = 0;

	if (xSucceeded(r = xResultFromWin32(RegQueryValueEx(keyCache, X_CHAR("AppCompatCache"), NULL, NULL, NULL, &size))))
	{
		xPtr<uint8_t> buffer((uint8_t*) xMemoryAlloc(size));

		if (!buffer)
		{
			return X_E_OUT_OF_MEMORY;
		}

		if (xSucceeded(r = xResultFromWin32(RegQueryValueEx(keyCache, X_CHAR("AppCompatCache"), NULL, NULL, buffer, &size))))
		{
			HKEY keySelect = NULL;

			if (xSucceeded(r = xResultFromWin32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, X_CHAR("System\\Select"), 0, KEY_QUERY_VALUE, &keySelect))))
			{
				uint32_t controlSet = 0;

				DWORD sizeControlSet = sizeof(controlSet);

				if (xSucceeded(r = xResultFromWin32(RegQueryValueEx(keySelect, X_CHAR("Current"), NULL, NULL, (uint8_t*) &controlSet, &sizeControlSet))))
				{
					HKEY keyEnvironment = NULL;

					if (xSucceeded(r = xResultFromWin32(RegOpenKeyEx(HKEY_LOCAL_MACHINE, X_CHAR("System\\CurrentControlSet\\Control\\Session Manager\\Environment"), 0, KEY_QUERY_VALUE, &keyEnvironment))))
					{
						DWORD sizeArchitecture = 0;

						if (xSucceeded(r = xResultFromWin32(RegQueryValueEx(keyEnvironment, X_CHAR("PROCESSOR_ARCHITECTURE"), NULL, NULL, NULL, &sizeArchitecture))))
						{
							xPtr<xChar> architecture((xChar*) xMemoryAlloc(sizeArchitecture));

							if (xSucceeded(r = xResultFromWin32(RegQueryValueEx(keyEnvironment, X_CHAR("PROCESSOR_ARCHITECTURE"), NULL, NULL, (uint8_t*) (xChar*) architecture, &sizeArchitecture))))
							{
								const bool is32 = xCharCompare(architecture, X_CHAR("x86")) == 0;

								r = Collect(buffer, size, controlSet, is32, applicationCompatibilityCacheData);
							}
						}

						RegCloseKey(keyEnvironment);
					}
				}

				RegCloseKey(keySelect);
			}
		}
	}

	RegCloseKey(keyCache);

	return r;
}

xResult xApplicationCompatibilityCacheDataCollector::Collect(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	X_ASSERT(buffer && size && applicationCompatibilityCacheData);

	if (!buffer || !size || !applicationCompatibilityCacheData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<IxStream> stream(new xMemoryStream);

	if (!stream)
	{
		return X_E_OUT_OF_MEMORY;
	}

	stream->Write(buffer, size);

	stream->SetPosition(0, xStreamOriginFlagBegin);

	xStreamReader reader;

	reader.Set(stream);

	uint32_t signature = 0;

	reader.Read(&signature);

	if (signature == 0xdeadbeef)
	{
		// Windows XP (unsupported)
		X_ASSERT(false);

		return X_E_NOT_IMPLEMENTED;
	}
	else if (signature == 0xbadc0ffe)
	{
		// Windows Vista / Windows Server 2003 / Windows Server 2008
		r = CollectWindowsVistaWindowsServer2003WindowsServer2008(buffer, size, controlSet, is32, applicationCompatibilityCacheData);   
	}
	else if (signature == 0xbadc0fee)
	{
		// Windows 7
		r = CollectWindows7(buffer, size, controlSet, is32, applicationCompatibilityCacheData);   
	}
	else if (xMemoryCompare(&signature, X_CHAR_A("00ts"), sizeof(signature)) == 0)
	{
		// Windows 8 / Windows Server 2012
		r = CollectWindows8WindowsServer2012(buffer, size, controlSet, applicationCompatibilityCacheData);
	}
	else if (xMemoryCompare(&signature, X_CHAR_A("10ts"), sizeof(signature)) == 0)
	{
		// Windows 8.1 / Windows Server 2012 R2
		r = CollectWindows81WindowsServer2012R2(buffer, size, controlSet, applicationCompatibilityCacheData);
	}
	else
	{
		// Windows 10?
		const uint32_t offset = signature;

		if (offset == 0x34)
		{
			stream->SetPosition(offset, xStreamOriginFlagBegin);

			reader.Read(&signature);

			if (xMemoryCompare(&signature, X_CHAR_A("10ts"), sizeof(signature)) == 0)
			{
				r = CollectWindows10(buffer, size, controlSet, applicationCompatibilityCacheData);
			}
			else
			{
				r = X_E_INVALID_DATA;
			}
		}
		else
		{
			r = X_E_INVALID_DATA;
		}
	}

	return r;
}

xResult xApplicationCompatibilityCacheDataCollector::CollectWindowsVistaWindowsServer2003WindowsServer2008(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	return X_S_OK;
}

xResult xApplicationCompatibilityCacheDataCollector::CollectWindows7(const uint8_t* buffer, const size_t size, const uint32_t controlSet, const bool is32, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	return X_S_OK;
}

xResult xApplicationCompatibilityCacheDataCollector::CollectWindows8WindowsServer2012(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	return X_S_OK;
}

xResult xApplicationCompatibilityCacheDataCollector::CollectWindows81WindowsServer2012R2(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	return CollectWindows8WindowsServer2012(buffer, size, controlSet, applicationCompatibilityCacheData);
}

xResult xApplicationCompatibilityCacheDataCollector::CollectWindows10(const uint8_t* buffer, const size_t size, const uint32_t controlSet, IxApplicationCompatibilityCacheData** applicationCompatibilityCacheData)
{
	X_ASSERT(buffer && size && applicationCompatibilityCacheData);

	if (!buffer || !size || !applicationCompatibilityCacheData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<IxStream> stream(new xMemoryStream);

	if (!stream)
	{
		return X_E_OUT_OF_MEMORY;
	}

	stream->Write(buffer, size);

	stream->SetPosition(0, xStreamOriginFlagBegin);

	xStreamReader reader;

	reader.Set(stream);

	uint32_t offset = 0;

	reader.Read(&offset);

	if (offset = 0x34)
	{
		stream->SetPosition(0x28, xStreamOriginFlagBegin);
	}
	else
	{
		stream->SetPosition(0x24, xStreamOriginFlagBegin);
	}
	
	uint32_t entryCount = 0;

	reader.Read(&entryCount);

	uint32_t index = offset;

	stream->SetPosition(index, xStreamOriginFlagBegin);

	while (index < size)
	{
		uint32_t signature = 0;

		reader.Read(&signature);

		if (xMemoryCompare(&signature, X_CHAR_A("10ts"), sizeof(signature)) != 0)
		{
			X_LOG_WARNING(X_CHAR("Unexpected application compatibility cache entry signature 0x%08x"), signature);

			r = X_E_INVALID_DATA;

			break;
		}

		stream->SetPosition(4, xStreamOriginFlagCurrent);

		uint32_t dataSize = 0;

		reader.Read(&dataSize);

		uint16_t fileNameSize = 0;

		reader.Read(&fileNameSize);

		xPtr<xChar> fileName;

		reader.Read(&fileName, fileNameSize);

		stream->SetPosition(fileNameSize, xStreamOriginFlagCurrent);


	}

	/*
	while (index < rawBytes.Length)
	{
		try
		{
			var ce = new CacheEntry
			{
				Signature = Encoding.ASCII.GetString(rawBytes, index, 4)
			};

			if (ce.Signature != "10ts")
			{
				break;
			}

			index += 4;

			// skip 4 unknown
			index += 4;

			var ceDataSize = BitConverter.ToUInt32(rawBytes, index);
			index += 4;

			ce.PathSize = BitConverter.ToUInt16(rawBytes, index);
			index += 2;
			ce.Path = Encoding.Unicode.GetString(rawBytes, index, ce.PathSize).Replace(@"\??\", "");
																					   index += ce.PathSize;

			ce.LastModifiedTimeUTC =
				DateTimeOffset.FromFileTime(BitConverter.ToInt64(rawBytes, index)).ToUniversalTime();
			index += 8;

			ce.DataSize = BitConverter.ToInt32(rawBytes, index);
			index += 4;

			ce.Data = rawBytes.Skip(index).Take(ce.DataSize).ToArray();
			index += ce.DataSize;

			ce.Executed = AppCompatCache.Execute.NA;

			ce.ControlSet = controlSet;
			ce.CacheEntryPosition = position;

			Entries.Add(ce);
			position += 1;
		}
		catch (Exception ex)
		{
			var _log = LogManager.GetCurrentClassLogger();
			_log.Error($"Error parsing cache entry. Position: {position} Index: {index}, Error: {ex.Message} ");
			//TODO Report this
			//take what we can get
			break;
		}
	}
*/

	return r;
}
