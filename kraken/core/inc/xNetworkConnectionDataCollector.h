#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"



class xNetworkConnectionDataCollector : public xRefCount<IxNetworkConnectionDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxNetworkConnectionDataCollector)
X_INTERFACE_MAP_END

public:
	xNetworkConnectionDataCollector();
	virtual ~xNetworkConnectionDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxNetworkConnectionDataEnum** networkConnectionDataEnum);

private:
	xResult GetTcp4(IxNetworkConnectionDataEnum** connectionDataEnum);
	xResult GetTcp6(IxNetworkConnectionDataEnum** connectionDataEnum);
	xResult GetUdp4(IxNetworkConnectionDataEnum** connectionDataEnum);
	xResult GetUdp6(IxNetworkConnectionDataEnum** connectionDataEnum);
	const xChar* GetType(const xNetworkConnectionType type);
	const xChar* GetTcpState(const DWORD state);
	xResult GetHostNameByIp4(const xChar* ip, const uint16_t port, xChar** host);
	xResult GetHostNameByIp6(const xChar* ip, const uint16_t port, xChar** host);
	xResult GetOwningProcess(const uint32_t processId, IxProcessData** processData);
};

