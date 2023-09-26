#include "xDnsCacheDataCollector.h"

#include <Mstcpip.h>
#include <time.h>
#include <Windns.h>

#include "xConversion.h"
#include "xFile.h"
#include "xLog.h"
#include "xPath.h"
#include "xString.h"

#include "xDataEnum.h"
#include "xDynamicCoreApi.h"
#include "xDnsCacheData.h"



#define X_DNS_CACHE_DATA_COLLECTOR_IP_ADDRESS_MAX	64



xDnsCacheDataCollector::xDnsCacheDataCollector()
{

}

xDnsCacheDataCollector::~xDnsCacheDataCollector()
{

}

xResult xDnsCacheDataCollector::Collect(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline)
{
	X_ASSERT(dnsCacheDataEnum);

	if (!dnsCacheDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xDnsCacheDataEnum> dcde(new xDnsCacheDataEnum);

	if (!dcde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xInterfacePtr<IxDnsCacheDataEnum> dcdeDnsCache;

	xResult r = GetDnsCacheDataDnsCache(&dcdeDnsCache, offline);

	if (xSucceeded(r))
	{
		dcde->Add(dcdeDnsCache, true);
	}

	xInterfacePtr<IxDnsCacheDataEnum> dcdeHosts;

	if (xSucceeded(r = GetDnsCacheDataHosts(&dcdeHosts)))
	{
		dcde->Add(dcdeHosts, true);
	}

	*dnsCacheDataEnum = dcde.Detach();

	return X_S_OK;
}

xResult xDnsCacheDataCollector::GetDnsCacheDataDnsCache(IxDnsCacheDataEnum** dnsCacheDataEnum, const bool offline)
{
	X_ASSERT(dnsCacheDataEnum);

	if (!dnsCacheDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xDnsCacheDataEnum> dcde(new xDnsCacheDataEnum);

	if (!dcde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xPtr<xDnsCacheEntry> rootEntry((xDnsCacheEntry*) xMemoryAlloc(sizeof(xDnsCacheEntry)));

	xDnsCacheEntry* entry = NULL;

	if (X_CORE_DNSAPI_IS_AVAILABLE(DnsGetCacheDataTable))
	{
		if (X_CORE_DNSAPI_CALL(DnsGetCacheDataTable)(rootEntry))
		{
			// First entry is for whatever reason always "0"
			entry = rootEntry->next;

			while (entry)
			{
				DNS_RECORD* rootRecord = NULL;

				DNS_RECORD* record = NULL;

				DNS_STATUS status = DnsQuery(entry->name, DNS_TYPE_A, DNS_QUERY_NO_WIRE_QUERY, NULL, &rootRecord, NULL);

				if (status != ERROR_SUCCESS && !offline)
				{
					status = DnsQuery(entry->name, DNS_TYPE_A, DNS_QUERY_WIRE_ONLY, NULL, &rootRecord, NULL);
				}

				if (status == ERROR_SUCCESS)
				{
					record = rootRecord;

					while (record)
					{
						xInterfacePtr<xDnsCacheData> dcd(new xDnsCacheData);

						if (!dcd)
						{
							DnsRecordListFree(rootRecord, DnsFreeRecordList);

							return X_E_OUT_OF_MEMORY;
						}

						dcd->SetName(entry->name);

						dcd->SetType(entry->type);

						dcd->SetDataLength(entry->dataLength);

						dcd->SetFlags(entry->flags);

						in_addr ip;

						ip.S_un.S_addr = record->Data.A.IpAddress;

						xString address;

						RtlIpv4AddressToString(&ip, address.GetBuffer(X_DNS_CACHE_DATA_COLLECTOR_IP_ADDRESS_MAX));

						address.ReleaseBuffer();

						dcd->SetAddress(address);

						dcd->SetTimeToLive(record->dwTtl);

						record = record->pNext;

						dcde->Add(dcd);
					}

					DnsRecordListFree(rootRecord, DnsFreeRecordList);
				}
				else
				{
					// If DNS query fails just add the cache entry
					xInterfacePtr<xDnsCacheData> dcd(new xDnsCacheData);

					if (!dcd)
					{
						DnsRecordListFree(rootRecord, DnsFreeRecordList);

						return X_E_OUT_OF_MEMORY;
					}

					dcd->SetName(entry->name);

					dcd->SetType(entry->type);

					dcd->SetDataLength(entry->dataLength);

					dcd->SetFlags(entry->flags);

					dcde->Add(dcd);

					if (!offline)
					{
						X_LOG_WARNING(L"DNS query failed (code: " X_RESULT_FORMAT_STRING L")", xResultFromWin32(status));
					}
				}

				entry = entry->next;
			}
		}
		else
		{
			X_LOG_WARNING(L"Failed to call \"DnsGetCacheDataTable\"");
		}
	}
	else
	{
		X_LOG_WARNING(L"Failed to get \"DnsGetCacheDataTable\" API");
	}

	*dnsCacheDataEnum = dcde.Detach();

	return X_S_OK;
}

xResult xDnsCacheDataCollector::GetDnsCacheDataHosts(IxDnsCacheDataEnum** dnsCacheDataEnum)
{
	X_ASSERT(dnsCacheDataEnum);

	if (!dnsCacheDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xDnsCacheDataEnum> dcde(new xDnsCacheDataEnum);

	if (!dcde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	xString systemDirectory;

	if (!GetSystemDirectory(systemDirectory.GetBuffer(X_PATH_MAX), X_PATH_MAX))
	{
		return xResultFromLastError();
	}

	systemDirectory.ReleaseBuffer();

	xString hosts = systemDirectory + X_CHAR("\\drivers\\etc\\hosts");

	WIN32_FILE_ATTRIBUTE_DATA fad;

	if (!GetFileAttributesEx(hosts, GetFileExInfoStandard, &fad))
	{
		return xResultFromLastError();
	}

	uint64_t sizeHosts = (static_cast<uint64_t>(fad.nFileSizeHigh) << (8 * sizeof(fad.nFileSizeLow))) | fad.nFileSizeLow;

	time_t now = 0;

	time(&now);

	time_t lastModified = xConvertFileTimeToTime(fad.ftLastWriteTime);

	time_t timeToLive = now - lastModified;

	FILE* file = xFileOpen(hosts, X_CHAR("rb"));

	if (!file)
	{
		return xResultFromErrNo();
	}

	uint32_t bufferSize = 128;

	uint32_t position = 0;

	char current = '\0';

	xPtr<char> buffer((char*) xMemoryAlloc(bufferSize));

	do
	{
		position = 0;

		do
		{
			current = (char) fgetc(file);

			if (current != EOF)
			{
				buffer[position++] = current;
			}

			if (position >= (bufferSize - 1))
			{
				bufferSize *= 2;

				buffer = (char*) xMemoryAlloc(bufferSize);
			}

		} while (current != EOF && current != '\n');

		buffer[position] = 0;

		xPtr<xChar> temp(xConvertUtf8ToUtf16(buffer));

		xString line(temp);

		uint32_t length = line.GetLength();

		while (length && (line.GetAt(0) == X_CHAR(' ') || line.GetAt(0) == X_CHAR('\t')))
		{
			line.Delete(0);

			length = line.GetLength();
		}

		length = line.GetLength();

		while (length && (line.GetAt(length - 1) == X_CHAR(' ') || line.GetAt(length - 1) == X_CHAR('\t') || line.GetAt(length - 1) == X_CHAR('\r') || line.GetAt(length - 1) == X_CHAR('\n')))
		{
			line.Delete(length - 1);

			length = line.GetLength();
		}

		if (line.GetLength() && line.GetAt(0) != X_CHAR('#'))
		{
			int32_t delimiter = -1;

			if ((delimiter = line.Find(X_CHAR(' '))) != -1 || (delimiter = line.Find(X_CHAR('\t'))) != -1)
			{
				xString ip = line.Left(delimiter);

				xString host = line.Right(length - delimiter);

				length = ip.GetLength();

				while (length && (ip.GetAt(length - 1) == X_CHAR(' ') || ip.GetAt(length - 1) == X_CHAR('\t')))
				{
					ip.Delete(length - 1);

					length = ip.GetLength();
				}

				length = host.GetLength();

				while (length && (host.GetAt(0) == X_CHAR(' ') || host.GetAt(0) == X_CHAR('\t')))
				{
					host.Delete(0);

					length = host.GetLength();
				}

				xInterfacePtr<xDnsCacheData> dcd(new xDnsCacheData);

				dcd->SetAddress(ip);

				dcd->SetName(host);

				dcd->SetTimeToLive((uint32_t) timeToLive);

				dcde->Add(dcd);
			}
		}

	} while (current != EOF);

	xFileClose(file);

	*dnsCacheDataEnum = dcde.Detach();

	return X_S_OK;
}
