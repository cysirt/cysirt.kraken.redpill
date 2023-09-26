#pragma once

#include <Windows.h>
#include <LM.h>

#include "xReferenceCount.h"

#include "xCoreInterface.h"


class xUserAccountDataCollector : public xRefCount<IxUserAccountDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxUserAccountDataCollector)
X_INTERFACE_MAP_END

public:
	xUserAccountDataCollector();
	virtual ~xUserAccountDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(IxUserAccountDataEnum** userAccountDataEnum);

private:
	xResult Collect(const USER_INFO_3* userInfo, IxUserAccountData** userAccountData);
	xResult GetPrivilegesString(const uint32_t privileges, xChar** privilegesString);
	xResult GetControlFlagsStringEnum(const uint32_t controlFlags, IxStringEnum** controlFlagsStringEnum);
	xResult GetAuthenticationFlagsStringEnum(const uint32_t authenticationFlags, IxStringEnum** controlFlagsStringEnum);

};

