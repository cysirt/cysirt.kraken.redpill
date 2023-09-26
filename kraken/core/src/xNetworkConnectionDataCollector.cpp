#include "xNetworkConnectionDataCollector.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <Mstcpip.h>

#include "xDataEnum.h"
#include "xLog.h"
#include "xNetworkConnectionData.h"
#include "xProcessDataCollector.h"



#define X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX	64



xNetworkConnectionDataCollector::xNetworkConnectionDataCollector()
{

}

xNetworkConnectionDataCollector::~xNetworkConnectionDataCollector()
{

}

xResult xNetworkConnectionDataCollector::Collect(IxNetworkConnectionDataEnum** networkConnectionDataEnum)
{
	X_ASSERT(networkConnectionDataEnum);

	if (!networkConnectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<xNetworkConnectionDataEnum> ncde(new xNetworkConnectionDataEnum);

	if (!ncde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		X_LOG_WARNING(L"WSAStartup failed (code: " X_RESULT_FORMAT_STRING L")", xResultFromWsaLastError());
	}

	xInterfacePtr<IxNetworkConnectionDataEnum> tcp4ncde;

	xResult r = GetTcp4(&tcp4ncde);

	if (xSucceeded(r))
	{
		ncde->Add(tcp4ncde, true);
	}

	xInterfacePtr<IxNetworkConnectionDataEnum> tcp6ncde;

	if (xSucceeded(r = GetTcp6(&tcp6ncde)))
	{
		ncde->Add(tcp6ncde, true);
	}

	xInterfacePtr<IxNetworkConnectionDataEnum> udp4ncde;

	if (xSucceeded(r = GetUdp4(&udp4ncde)))
	{
		ncde->Add(udp4ncde, true);
	}

	xInterfacePtr<IxNetworkConnectionDataEnum> udp6ncde;

	if (xSucceeded(r = GetUdp6(&udp6ncde)))
	{
		ncde->Add(udp6ncde, true);
	}

	*networkConnectionDataEnum = ncde.Detach();

	WSACleanup();

	return X_S_OK;
}

xResult xNetworkConnectionDataCollector::GetTcp4(IxNetworkConnectionDataEnum** connectionDataEnum)
{
	X_ASSERT(connectionDataEnum);

	if (!connectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xNetworkConnectionDataEnum> ncde(new xNetworkConnectionDataEnum);

	if (!ncde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	ULONG size = 0;

	ULONG result = NO_ERROR;

	if ((result = GetExtendedTcpTable(NULL, &size, TRUE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0)) != ERROR_INSUFFICIENT_BUFFER)
	{
		return xResultFromWin32(result);
	}

	xPtr<MIB_TCPTABLE_OWNER_MODULE> tcpTable((MIB_TCPTABLE_OWNER_MODULE*) xMemoryAlloc(size));

	if (!tcpTable)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if ((result = GetExtendedTcpTable(tcpTable, &size, TRUE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0)) != NO_ERROR)
	{
		xResultFromWin32(result);
	}

	for (uint32_t ui = 0; ui < tcpTable->dwNumEntries; ui++)
	{
		xInterfacePtr<xNetworkConnectionData> ncd(new xNetworkConnectionData);

		if (!ncd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		ncd->SetType(xNetworkConnectionTypeTcp4);

		ncd->SetTypeString(GetType(xNetworkConnectionTypeTcp4));

		ncd->SetState(GetTcpState(tcpTable->table[ui].dwState));

		in_addr ip;

		xString address;

		// Local
		ip.S_un.S_addr = (u_long) tcpTable->table[ui].dwLocalAddr;

		RtlIpv4AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetLocalAddress(address);

		xPtr<xChar> localHost;

		if (xSucceeded(r = GetHostNameByIp4(address, (uint16_t) tcpTable->table[ui].dwLocalPort, &localHost)))
		{
			ncd->SetLocalHost(localHost);
		}

		address.Empty();

		ncd->SetLocalPort(ntohs((u_short) tcpTable->table[ui].dwLocalPort));

		// Remote
		ip.S_un.S_addr = (u_long) tcpTable->table[ui].dwRemoteAddr;

		RtlIpv4AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetRemoteAddress(address);

		xPtr<xChar> remoteHost;

		if (xSucceeded(r = GetHostNameByIp4(address, (uint16_t) tcpTable->table[ui].dwRemotePort, &remoteHost)))
		{
			ncd->SetRemoteHost(remoteHost);
		}

		address.Empty();

		ncd->SetRemotePort(ntohs((u_short) tcpTable->table[ui].dwRemotePort));

		xInterfacePtr<IxProcessData> pd;

		if (xSucceeded(r = GetOwningProcess(tcpTable->table[ui].dwOwningPid, &pd)))
		{
			ncd->SetOwningProcess(pd);
		}

		ncde->Add(ncd);
	}

	*connectionDataEnum = ncde.Detach();

	return X_S_OK;
}

xResult xNetworkConnectionDataCollector::GetTcp6(IxNetworkConnectionDataEnum** connectionDataEnum)
{
	X_ASSERT(connectionDataEnum);

	if (!connectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xNetworkConnectionDataEnum> ncde(new xNetworkConnectionDataEnum);

	if (!ncde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	ULONG size = 0;

	ULONG result = NO_ERROR;

	if ((result = GetExtendedTcpTable(NULL, &size, TRUE, AF_INET6, TCP_TABLE_OWNER_MODULE_ALL, 0)) != ERROR_INSUFFICIENT_BUFFER)
	{
		return xResultFromWin32(result);
	}

	xPtr<MIB_TCP6TABLE_OWNER_MODULE> tcpTable((MIB_TCP6TABLE_OWNER_MODULE*) xMemoryAlloc(size));

	if (!tcpTable)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if ((result = GetExtendedTcpTable(tcpTable, &size, TRUE, AF_INET6, TCP_TABLE_OWNER_MODULE_ALL, 0)) != NO_ERROR)
	{
		xResultFromWin32(result);
	}

	for (uint32_t ui = 0; ui < tcpTable->dwNumEntries; ui++)
	{
		xInterfacePtr<xNetworkConnectionData> ncd(new xNetworkConnectionData);

		if (!ncd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		ncd->SetType(xNetworkConnectionTypeTcp6);

		ncd->SetTypeString(GetType(xNetworkConnectionTypeTcp6));

		ncd->SetState(GetTcpState(tcpTable->table[ui].dwState));

		in6_addr ip;

		xString address;

		// Local
		xMemoryCopy(ip.s6_addr, tcpTable->table[ui].ucLocalAddr, sizeof(ip.s6_addr));

		RtlIpv6AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetLocalAddress(address);

		xPtr<xChar> localHost;

		if (xSucceeded(r = GetHostNameByIp6(address, (uint16_t) tcpTable->table[ui].dwLocalPort, &localHost)))
		{
			ncd->SetLocalHost(localHost);
		}

		address.Empty();

		ncd->SetLocalPort(ntohs((u_short) tcpTable->table[ui].dwLocalPort));

		// Remote
		xMemoryCopy(ip.s6_addr, tcpTable->table[ui].ucRemoteAddr, sizeof(ip.s6_addr));

		RtlIpv6AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetRemoteAddress(address);

		xPtr<xChar> remoteHost;

		if (xSucceeded(r = GetHostNameByIp6(address, (uint16_t) tcpTable->table[ui].dwRemotePort, &remoteHost)))
		{
			ncd->SetRemoteHost(remoteHost);
		}

		address.Empty();

		ncd->SetRemotePort(ntohs((u_short) tcpTable->table[ui].dwRemotePort));

		xInterfacePtr<IxProcessData> pd;

		if (xSucceeded(r = GetOwningProcess(tcpTable->table[ui].dwOwningPid, &pd)))
		{
			ncd->SetOwningProcess(pd);
		}

		ncde->Add(ncd);
	}

	*connectionDataEnum = ncde.Detach();

	return X_S_OK;
}

xResult xNetworkConnectionDataCollector::GetUdp4(IxNetworkConnectionDataEnum** connectionDataEnum)
{
	X_ASSERT(connectionDataEnum);

	if (!connectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xNetworkConnectionDataEnum> ncde(new xNetworkConnectionDataEnum);

	if (!ncde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	ULONG size = 0;

	ULONG result = NO_ERROR;

	if ((result = GetExtendedUdpTable(NULL, &size, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0)) != ERROR_INSUFFICIENT_BUFFER)
	{
		return xResultFromWin32(result);
	}

	xPtr<MIB_UDPTABLE_OWNER_MODULE> udpTable((MIB_UDPTABLE_OWNER_MODULE*) xMemoryAlloc(size));

	if (!udpTable)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if ((result = GetExtendedUdpTable(udpTable, &size, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0)) != NO_ERROR)
	{
		xResultFromWin32(result);
	}

	for (uint32_t ui = 0; ui < udpTable->dwNumEntries; ui++)
	{
		xInterfacePtr<xNetworkConnectionData> ncd(new xNetworkConnectionData);

		if (!ncd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		ncd->SetType(xNetworkConnectionTypeUdp4);

		ncd->SetTypeString(GetType(xNetworkConnectionTypeUdp4));

		in_addr ip;

		xString address;

		// Local
		ip.S_un.S_addr = (u_long) udpTable->table[ui].dwLocalAddr;

		RtlIpv4AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetLocalAddress(address);

		xPtr<xChar> localHost;

		if (xSucceeded(r = GetHostNameByIp4(address, (uint16_t) udpTable->table[ui].dwLocalPort, &localHost)))
		{
			ncd->SetLocalHost(localHost);
		}

		address.Empty();

		ncd->SetLocalPort(ntohs((u_short) udpTable->table[ui].dwLocalPort));

		xInterfacePtr<IxProcessData> pd;

		if (xSucceeded(r = GetOwningProcess(udpTable->table[ui].dwOwningPid, &pd)))
		{
			ncd->SetOwningProcess(pd);
		}

		ncde->Add(ncd);
	}

	*connectionDataEnum = ncde.Detach();

	return X_S_OK;
}

xResult xNetworkConnectionDataCollector::GetUdp6(IxNetworkConnectionDataEnum** connectionDataEnum)
{
	X_ASSERT(connectionDataEnum);

	if (!connectionDataEnum)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xResult r = X_S_OK;

	xInterfacePtr<xNetworkConnectionDataEnum> ncde(new xNetworkConnectionDataEnum);

	if (!ncde)
	{
		return X_E_OUT_OF_MEMORY;
	}

	ULONG size = 0;

	ULONG result = NO_ERROR;

	if ((result = GetExtendedUdpTable(NULL, &size, TRUE, AF_INET6, UDP_TABLE_OWNER_MODULE, 0)) != ERROR_INSUFFICIENT_BUFFER)
	{
		return xResultFromWin32(result);
	}

	xPtr<MIB_UDP6TABLE_OWNER_MODULE> udpTable((MIB_UDP6TABLE_OWNER_MODULE*) xMemoryAlloc(size));

	if (!udpTable)
	{
		return X_E_OUT_OF_MEMORY;
	}

	if ((result = GetExtendedUdpTable(udpTable, &size, TRUE, AF_INET6, UDP_TABLE_OWNER_MODULE, 0)) != NO_ERROR)
	{
		xResultFromWin32(result);
	}

	for (uint32_t ui = 0; ui < udpTable->dwNumEntries; ui++)
	{
		xInterfacePtr<xNetworkConnectionData> ncd(new xNetworkConnectionData);

		if (!ncd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		ncd->SetType(xNetworkConnectionTypeUdp6);

		ncd->SetTypeString(GetType(xNetworkConnectionTypeUdp6));

		in6_addr ip;

		xString address;

		// Local
		xMemoryCopy(ip.s6_addr, udpTable->table[ui].ucLocalAddr, sizeof(ip.s6_addr));

		RtlIpv6AddressToString(&ip, address.GetBuffer(X_NETWORK_DATA_COLLECTOR_IP_ADDRESS_MAX));

		address.ReleaseBuffer();

		ncd->SetLocalAddress(address);

		xPtr<xChar> localHost;

		if (xSucceeded(r = GetHostNameByIp6(address, (uint16_t) udpTable->table[ui].dwLocalPort, &localHost)))
		{
			ncd->SetLocalHost(localHost);
		}

		address.Empty();

		ncd->SetLocalPort(ntohs((u_short) udpTable->table[ui].dwLocalPort));

		xInterfacePtr<IxProcessData> pd;

		if (xSucceeded(r = GetOwningProcess(udpTable->table[ui].dwOwningPid, &pd)))
		{
			ncd->SetOwningProcess(pd);
		}

		ncde->Add(ncd);
	}

	*connectionDataEnum = ncde.Detach();

	return X_S_OK;
}

const xChar* xNetworkConnectionDataCollector::GetType(const xNetworkConnectionType type)
{
	switch (type)
	{
		case xNetworkConnectionTypeTcp4:
		{
			return L"TCP4";
		}
		case xNetworkConnectionTypeTcp6:
		{
			return L"TCP6";
		}
		case xNetworkConnectionTypeUdp4:
		{
			return L"UDP4";
		}
		case xNetworkConnectionTypeUdp6:
		{
			return L"UDP6";
		}
		default:
		{
			return L"Unknown";
		}
	}
}

const xChar* xNetworkConnectionDataCollector::GetTcpState(const DWORD state)
{
	switch (state)
	{
		case MIB_TCP_STATE_CLOSED:
		{
			return L"CLOSED";
		}
		case MIB_TCP_STATE_LISTEN:
		{
			return L"LISTEN";
		}
		case MIB_TCP_STATE_SYN_SENT:
		{
			return L"SYN-SENT";
		}
		case MIB_TCP_STATE_SYN_RCVD:
		{
			return L"SYN-RECEIVED";
		}
		case MIB_TCP_STATE_ESTAB:
		{
			return L"ESTABLISHED";
		}
		case MIB_TCP_STATE_FIN_WAIT1:
		{
			return L"FIN-WAIT-1";
		}
		case MIB_TCP_STATE_FIN_WAIT2:
		{
			return L"FIN-WAIT-2";
		}
		case MIB_TCP_STATE_CLOSE_WAIT:
		{
			return L"CLOSE-WAIT";
		}
		case MIB_TCP_STATE_CLOSING:
		{
			return L"CLOSING";
		}
		case MIB_TCP_STATE_LAST_ACK:
		{
			return L"LAST-ACK";
		}
		case MIB_TCP_STATE_TIME_WAIT:
		{
			return L"TIME-WAIT";
		}
		case MIB_TCP_STATE_DELETE_TCB:
		{
			return L"DELETE-TCB";
		}
		default:
		{
			return L"UNKNOWN";
		}
	}
}

xResult xNetworkConnectionDataCollector::GetHostNameByIp4(const xChar* ip, const uint16_t port, xChar** host)
{
	X_ASSERT(ip && host);

	if (!ip || !host)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString hostName;

	sockaddr_in sa;

	sa.sin_family = AF_INET;

	if (InetPton(AF_INET, ip, &sa.sin_addr.s_addr) == 1)
	{
		sa.sin_port = htons(port);

		xChar name[NI_MAXHOST] = { 0 };

		xChar service[NI_MAXSERV] = { 0 };

		DWORD result = GetNameInfo((SOCKADDR*) &sa, sizeof(sockaddr_in), name, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

		if (result == 0)
		{
			hostName = name;
		}
	}

	if (!hostName.IsEmpty())
	{
		*host = hostName.Clone();

		return X_S_OK;
	}
	else
	{
		return X_E_FAIL;
	}
}

xResult xNetworkConnectionDataCollector::GetHostNameByIp6(const xChar* ip, const uint16_t port, xChar** host)
{
	X_ASSERT(ip && host);

	if (!ip || !host)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xString hostName;

	sockaddr_in6 sa;

	sa.sin6_family = AF_INET6;

	if (InetPton(AF_INET6, ip, &sa.sin6_addr.s6_addr) == 1)
	{
		sa.sin6_port = htons(port);

		xChar name[NI_MAXHOST] = { 0 };

		xChar service[NI_MAXSERV] = { 0 };

		DWORD result = GetNameInfo((SOCKADDR*) &sa, sizeof(sockaddr_in6), name, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

		if (result == 0)
		{
			hostName = name;
		}
	}

	if (!hostName.IsEmpty())
	{
		*host = hostName.Clone();

		return X_S_OK;
	}
	else
	{
		return X_E_FAIL;
	}
}

xResult xNetworkConnectionDataCollector::GetOwningProcess(const uint32_t processId, IxProcessData** processData)
{
	X_ASSERT(processData);

	if (!processData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	if (!processId)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xProcessDataCollector pdc;

	xInterfacePtr<IxProcessData> pd;

	xResult r = pdc.Collect(processId, &pd);

	if (xSucceeded(r))
	{
		*processData = pd.Detach();
	}
	
	return r;
}
