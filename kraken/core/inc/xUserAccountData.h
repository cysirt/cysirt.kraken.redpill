#pragma once

#include "xReferenceCount.h"
#include "xStringEnum.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xUserAccountData : public xCorePropertyEnum<IxUserAccountData>
{
typedef xCorePropertyEnum<IxUserAccountData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxUserAccountData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_PROPERTY_MAP_SET_GET(Name, xChar*)
	X_PROPERTY_MAP_SET_GET(PasswordAge, uint32_t)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(Privileges, uint32_t)
	X_PROPERTY_MAP_SET_GET(HomeDirectory, xChar*)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_ENUM(ControlFlags, uint32_t)
	X_PROPERTY_MAP_SET_GET(ScriptPath, xChar*)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_ENUM(AuthenticationFlags, uint32_t)
	X_PROPERTY_MAP_SET_GET(FullName, xChar*)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(LastLogon)
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_TIME(AccountExpires)
	X_PROPERTY_MAP_SET_GET(BadPasswordCount, uint32_t)
	X_PROPERTY_MAP_SET_GET(LogonCount, uint32_t)
	X_PROPERTY_MAP_SET_GET(LogonServer, xChar*)
	X_PROPERTY_MAP_SET_GET(CountryCode, uint32_t)
	X_PROPERTY_MAP_SET_GET(CodePage, uint32_t)
	X_PROPERTY_MAP_SET_GET(UserId, uint32_t)
	X_PROPERTY_MAP_SET_GET(PrimaryGroupId, uint32_t)
	X_PROPERTY_MAP_SET_GET(Profile, xChar*)
	X_PROPERTY_MAP_SET_GET(HomeDirectoryDrive, xChar*)
	X_PROPERTY_MAP_SET_GET(PasswordExpired, uint32_t)
X_PROPERTY_MAP_END

public:
	xUserAccountData()
	{
	}

	virtual ~xUserAccountData()
	{
	}

	X_INTERFACE_METHOD(xResult, GetPrivilegesString)(xChar** privilegesString)
	{
		X_ASSERT(privilegesString);

		if (!privilegesString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t privileges = 0;

		xResult r = GetPrivileges(&privileges);

		if (xFailed(r))
		{
			return r;
		}

		switch (privileges)
		{
			case USER_PRIV_GUEST:
			{
				*privilegesString = xCharClone(X_CHAR("USER_PRIV_GUEST"));

				break;
			}
			case USER_PRIV_USER:
			{
				*privilegesString = xCharClone(X_CHAR("USER_PRIV_USER"));

				break;
			}
			case USER_PRIV_ADMIN:
			{
				*privilegesString = xCharClone(X_CHAR("USER_PRIV_ADMIN"));

				break;
			}
			default:
			{
				X_ASSERT(false);

				return X_E_NOT_PRESENT;
			}
		}

		return X_S_OK;
	}

	X_INTERFACE_METHOD(xResult, GetControlFlagsStringEnum)(IxStringEnum** controlFlagsStringEnum)
	{
		X_ASSERT(controlFlagsStringEnum);

		if (!controlFlagsStringEnum)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t controlFlags = 0;

		xResult r = GetControlFlags(&controlFlags);

		if (xFailed(r))
		{
			return r;
		}

		xInterfacePtr<IxStringEnum> cfse(new xStringEnum);

		if (!cfse)
		{
			return X_E_INVALID_ARGUMENT;
		}

		if (controlFlags & UF_SCRIPT)
		{
			cfse->Add(X_CHAR("UF_SCRIPT"));
		}

		if (controlFlags & UF_ACCOUNTDISABLE)
		{
			cfse->Add(X_CHAR("UF_ACCOUNTDISABLE"));
		}

		if (controlFlags & UF_HOMEDIR_REQUIRED)
		{
			cfse->Add(X_CHAR("UF_HOMEDIR_REQUIRED"));
		}

		if (controlFlags & UF_PASSWD_NOTREQD)
		{
			cfse->Add(X_CHAR("UF_PASSWD_NOTREQD"));
		}

		if (controlFlags & UF_PASSWD_CANT_CHANGE)
		{
			cfse->Add(X_CHAR("UF_PASSWD_CANT_CHANGE"));
		}

		if (controlFlags & UF_LOCKOUT)
		{
			cfse->Add(X_CHAR("UF_LOCKOUT"));
		}

		if (controlFlags & UF_DONT_EXPIRE_PASSWD)
		{
			cfse->Add(X_CHAR("UF_DONT_EXPIRE_PASSWD"));
		}

		if (controlFlags & UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED)
		{
			cfse->Add(X_CHAR("UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED"));
		}

		if (controlFlags & UF_NOT_DELEGATED)
		{
			cfse->Add(X_CHAR("UF_NOT_DELEGATED"));
		}

		if (controlFlags & UF_SMARTCARD_REQUIRED)
		{
			cfse->Add(X_CHAR("UF_SMARTCARD_REQUIRED"));
		}

		if (controlFlags & UF_USE_DES_KEY_ONLY)
		{
			cfse->Add(X_CHAR("UF_USE_DES_KEY_ONLY"));
		}

		if (controlFlags & UF_DONT_REQUIRE_PREAUTH)
		{
			cfse->Add(X_CHAR("UF_DONT_REQUIRE_PREAUTH"));
		}

		if (controlFlags & UF_TRUSTED_FOR_DELEGATION)
		{
			cfse->Add(X_CHAR("UF_TRUSTED_FOR_DELEGATION"));
		}

		if (controlFlags & UF_PASSWORD_EXPIRED)
		{
			cfse->Add(X_CHAR("UF_PASSWORD_EXPIRED"));
		}

		if (controlFlags & UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION)
		{
			cfse->Add(X_CHAR("UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION"));
		}

		if (controlFlags & UF_NORMAL_ACCOUNT)
		{
			cfse->Add(X_CHAR("UF_NORMAL_ACCOUNT"));
		}

		if (controlFlags & UF_TEMP_DUPLICATE_ACCOUNT)
		{
			cfse->Add(X_CHAR("UF_TEMP_DUPLICATE_ACCOUNT"));
		}

		if (controlFlags & UF_WORKSTATION_TRUST_ACCOUNT)
		{
			cfse->Add(X_CHAR("UF_WORKSTATION_TRUST_ACCOUNT"));
		}

		if (controlFlags & UF_SERVER_TRUST_ACCOUNT)
		{
			cfse->Add(X_CHAR("UF_SERVER_TRUST_ACCOUNT"));
		}

		if (controlFlags & UF_INTERDOMAIN_TRUST_ACCOUNT)
		{
			cfse->Add(X_CHAR("UF_INTERDOMAIN_TRUST_ACCOUNT"));
		}

		const uint32_t count = cfse->GetCount();

		if (count)
		{
			*controlFlagsStringEnum = cfse.Detach();
		}

		return count ? X_S_OK : X_E_NOT_PRESENT;
	}

	X_INTERFACE_METHOD(xResult, GetAuthenticationFlagsStringEnum)(IxStringEnum** authenticationFlagsStringEnum)
	{
		X_ASSERT(authenticationFlagsStringEnum);

		if (!authenticationFlagsStringEnum)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t authenticationFlags = 0;

		xResult r = GetAuthenticationFlags(&authenticationFlags);

		if (xFailed(r))
		{
			return r;
		}

		xInterfacePtr<IxStringEnum> afse(new xStringEnum);

		if (!afse)
		{
			return X_E_OUT_OF_MEMORY;
		}

		if (authenticationFlags & AF_OP_PRINT)
		{
			afse->Add(X_CHAR("AF_OP_PRINT"));
		}

		if (authenticationFlags & AF_OP_COMM)
		{
			afse->Add(X_CHAR("AF_OP_COMM"));
		}

		if (authenticationFlags & AF_OP_SERVER)
		{
			afse->Add(X_CHAR("AF_OP_SERVER"));
		}

		if (authenticationFlags & AF_OP_ACCOUNTS)
		{
			afse->Add(X_CHAR("AF_OP_ACCOUNTS"));
		}

		const uint32_t count = afse->GetCount();

		if (count)
		{
			*authenticationFlagsStringEnum = afse.Detach();
		}

		return count ? X_S_OK : X_E_NOT_PRESENT;
	}
};

