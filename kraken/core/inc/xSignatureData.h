#pragma once

#include "xReferenceCount.h"

#include "xCoreInterface.h"
#include "xCorePropertyEnum.h"



class xSignatureData : public xCorePropertyEnum<IxSignatureData>
{
typedef xCorePropertyEnum<IxSignatureData> _Base;

X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxSignatureData)
	X_INTERFACE_MAP_CHAIN(_Base)
X_INTERFACE_MAP_END

X_PROPERTY_MAP_BEGIN
	X_CORE_PROPERTY_MAP_SET_GET_COMPANION_STRING_RAW(State, uint32_t)
	X_PROPERTY_MAP_SET_GET(Signer, xChar*)
X_PROPERTY_MAP_END

public:
	xSignatureData()
	{
	}

	virtual ~xSignatureData()
	{
	}

	X_INTERFACE_METHOD(xResult, GetStateString)(xChar** stateString)
	{
		X_ASSERT(stateString);

		if (!stateString)
		{
			return X_E_INVALID_ARGUMENT;
		}

		uint32_t state = 0;

		xResult r = GetState(&state);

		if (xFailed(r))
		{
			return r;
		}

		switch (state)
		{
			case 0:
			{
				*stateString = xCharClone(X_CHAR("TRUSTED"));

				break;
			}
			case TRUST_E_NOSIGNATURE:
			{
				*stateString = xCharClone(X_CHAR("TRUST_E_NOSIGNATURE"));

				break;
			}
			case CERT_E_EXPIRED:
			{
				*stateString = xCharClone(X_CHAR("CERT_E_EXPIRED"));

				break;
			}
			case CERT_E_REVOKED:
			{
				*stateString = xCharClone(X_CHAR("CERT_E_REVOKED"));

				break;
			}
			case TRUST_E_EXPLICIT_DISTRUST:
			{
				*stateString = xCharClone(X_CHAR("TRUST_E_EXPLICIT_DISTRUST"));

				break;
			}
			case CRYPT_E_SECURITY_SETTINGS:
			{
				*stateString = xCharClone(X_CHAR("CRYPT_E_SECURITY_SETTINGS"));

				break;
			}
			case TRUST_E_BAD_DIGEST:
			{
				*stateString = xCharClone(X_CHAR("TRUST_E_BAD_DIGEST"));

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
};

