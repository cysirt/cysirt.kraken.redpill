#include "xSignatureDataCollector.h"

#include <Mscat.h>
#include <SoftPub.h>
#include <Wincrypt.h>
#include <Wintrust.h>

#include "xConversion.h"
#include "xDynamicCoreApi.h"
#include "xLog.h"

#include "xDataCache.h"
#include "xSignatureData.h"



#define X_SIGNATURE_DATA_COLLECTOR_FILE_NAME_WINTRUST_DLL	L"Wintrust.dll"
#define X_SIGNATURE_DATA_COLLECTOR_FILE_NAME_NTDLL_DLL		L"ntdll.dll"

static GUID IID_WinTrustActionGenericVerifyV2 = WINTRUST_ACTION_GENERIC_VERIFY_V2;
static GUID IID_DriverActionVerify =			DRIVER_ACTION_VERIFY;




xSignatureDataCollector::xSignatureDataCollector()
{

}

xSignatureDataCollector::~xSignatureDataCollector()
{

}

xResult xSignatureDataCollector::Collect(const xChar* fileName, IxSignatureData** signatureData)
{
	X_ASSERT(fileName && signatureData);

	if (!fileName || !signatureData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxSignatureData, &iface);

	if (xSucceeded(r))
	{
		return iface->QueryInterface(IID_IxSignatureData, (void**) signatureData);
	}
	else
	{
		xInterfacePtr<xSignatureData> sd(new xSignatureData);

		uint32_t state = TRUST_E_NOSIGNATURE;

		xPtr<xChar> signer;

		PCERT_CONTEXT* signatures = NULL;

		uint32_t countSignatures = 0;

		if (xSucceeded(r = Verify(fileName, &state, &signatures, &countSignatures)))
		{
			if (countSignatures)
			{
				GetSigner(signatures[0], &signer);
			}

			Free(signatures, countSignatures);
		}

		sd->SetState(state);

		if (signer)
		{
			sd->SetSigner(signer);
		}

		xDataCacheGet().Add(fileName, IID_IxSignatureData, sd);

		*signatureData = sd.Detach();
	}

	return r;
}

xResult xSignatureDataCollector::Verify(const xChar* fileName, uint32_t* state, PCERT_CONTEXT** signatures, uint32_t* countSignatures)
{
	HANDLE file = CreateFile(fileName, FILE_GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		return xResultFromLastError();
	}

	WINTRUST_FILE_INFO wtfi = { 0 };

	wtfi.cbStruct = sizeof(WINTRUST_FILE_INFO);

	wtfi.pcwszFilePath = fileName;

	wtfi.hFile = file;

	PCERT_CONTEXT* signaturesTemp = NULL;

	uint32_t countSignaturesTemp = 0;

	uint32_t signatureState = Verify(file, WTD_CHOICE_FILE, &wtfi, &IID_WinTrustActionGenericVerifyV2, NULL, &signaturesTemp, &countSignaturesTemp);

	if (signatureState == TRUST_E_NOSIGNATURE)
	{
		if (X_CORE_WINTRUST_IS_AVAILABLE(CryptCATAdminAcquireContext2) && X_CORE_WINTRUST_IS_AVAILABLE(CryptCATAdminCalcHashFromFileHandle2))
		{
			Free(signaturesTemp, countSignaturesTemp);

			signatureState = VerifyFromCatalog(fileName, file, BCRYPT_SHA256_ALGORITHM, &signaturesTemp, &countSignaturesTemp);
		}

		if (signatureState != 0)
		{
			Free(signaturesTemp, countSignaturesTemp);

			signatureState = VerifyFromCatalog(fileName, file, NULL, &signaturesTemp, &countSignaturesTemp);
		}
	}

	*state = signatureState;

	*signatures = signaturesTemp;

	*countSignatures = countSignaturesTemp;

	CloseHandle(file);

	return X_S_OK;
}

uint32_t xSignatureDataCollector::Verify(HANDLE file, unsigned long unionChoice, void* unionData, GUID* actionId, void* policyCallbackData, PCERT_CONTEXT** signatures, uint32_t* countSignatures)
{
	WINTRUST_DATA wtd = { 0 };

	wtd.cbStruct = sizeof(WINTRUST_DATA);

	wtd.pPolicyCallbackData = policyCallbackData;

	wtd.dwUIChoice = WTD_UI_NONE;

	wtd.fdwRevocationChecks = WTD_REVOKE_WHOLECHAIN;

	wtd.dwUnionChoice = unionChoice;

	wtd.dwStateAction = WTD_STATEACTION_VERIFY;

	wtd.dwProvFlags = WTD_SAFER_FLAG;

	wtd.pFile = (WINTRUST_FILE_INFO*) unionData;

	if (unionChoice == WTD_CHOICE_CATALOG)
	{
		wtd.pCatalog = (WINTRUST_CATALOG_INFO*) unionData;
	}

	wtd.fdwRevocationChecks = WTD_REVOKE_NONE;

	wtd.dwProvFlags |= WTD_CACHE_ONLY_URL_RETRIEVAL;

	long status = WinVerifyTrust(NULL, actionId, &wtd);

	GetSignatures(wtd.hWVTStateData, signatures, countSignatures);

	wtd.dwStateAction = WTD_STATEACTION_CLOSE;

	WinVerifyTrust(NULL, actionId, &wtd);

	return status;
}

xResult xSignatureDataCollector::GetFileHash(HANDLE file, const xChar* hashAlgorithm, uint8_t** hash, uint32_t* sizeHash, HANDLE* catAdmin)
{
	HANDLE catAdminTemp = NULL;

	if (X_CORE_WINTRUST_IS_AVAILABLE(CryptCATAdminAcquireContext2))
	{
		if (!X_CORE_WINTRUST_CALL(CryptCATAdminAcquireContext2)(&catAdminTemp, &IID_DriverActionVerify, hashAlgorithm, NULL, 0))
		{
			return xResultFromLastError();
		}
	}
	else
	{
		if (!CryptCATAdminAcquireContext(&catAdminTemp, &IID_DriverActionVerify, 0))
		{
			return xResultFromLastError();
		}
	}

	unsigned long sizeHashTemp = 32;

	uint8_t* hashTemp = (uint8_t*) xMemoryAlloc(sizeHashTemp);

	if (hashTemp)
	{
		if (X_CORE_WINTRUST_IS_AVAILABLE(CryptCATAdminCalcHashFromFileHandle2))
		{
			if (!X_CORE_WINTRUST_CALL(CryptCATAdminCalcHashFromFileHandle2)(catAdminTemp, file, &sizeHashTemp, hashTemp, 0))
			{
				xMemoryFree(hashTemp);

				hashTemp = (uint8_t*) xMemoryAlloc(sizeHashTemp);

				if (hashTemp)
				{
					if (!X_CORE_WINTRUST_CALL(CryptCATAdminCalcHashFromFileHandle2)(catAdminTemp, file, &sizeHashTemp, hashTemp, 0))
					{
						CryptCATAdminReleaseContext(catAdminTemp, 0);

						xMemoryFree(hashTemp);

						return xResultFromLastError();
					}
				}
			}
		}
		else
		{
			if (!CryptCATAdminCalcHashFromFileHandle(file, &sizeHashTemp, hashTemp, 0))
			{
				xMemoryFree(hashTemp);

				hashTemp = (uint8_t*) xMemoryAlloc(sizeHashTemp);

				if (hashTemp)
				{
					if (!CryptCATAdminCalcHashFromFileHandle(file, &sizeHashTemp, hashTemp, 0))
					{
						CryptCATAdminReleaseContext(catAdminTemp, 0);

						xMemoryFree(hashTemp);

						return xResultFromLastError();
					}
				}
			}
		}
	}
	else
	{
		return X_E_OUT_OF_MEMORY;
	}

	*hash = hashTemp;

	*sizeHash = sizeHashTemp;

	*catAdmin = catAdminTemp;

	return X_S_OK;
}

uint32_t xSignatureDataCollector::VerifyFromCatalog(const xChar* fileName, HANDLE file, const xChar* hashAlgorithm, PCERT_CONTEXT** signatures, uint32_t* countSignatures)
{
	uint32_t stateSignature = TRUST_E_NOSIGNATURE;

	LARGE_INTEGER sizeFile = { 0 };

	if (!GetFileSizeEx(file, &sizeFile))
	{
		return TRUST_E_NOSIGNATURE;
	}

	PCERT_CONTEXT* signaturesTemp = NULL;

	uint32_t countSignaturesTemp = 0;

	xPtr<uint8_t> hash;

	uint32_t sizeHash;

	HANDLE catAdmin = NULL;

	xResult r = GetFileHash(file, hashAlgorithm, &hash, &sizeHash, &catAdmin);

	if (xSucceeded(r))
	{
		xPtr<xChar> hashTagTemp;

		hashTagTemp.Attach(xConvertBinaryToHex(hash, sizeHash));

		xString hashTag = hashTagTemp;

		hashTag.ToUpper();

		// Search the system catalogs
		HANDLE catInfo = CryptCATAdminEnumCatalogFromHash(catAdmin, hash, sizeHash, 0, NULL);

		if (catInfo)
		{
			CATALOG_INFO ci = { 0 };

			DRIVER_VER_INFO vi = { 0 };

			if (CryptCATCatalogInfoFromContext(catInfo, &ci, 0))
			{
				// Disable OS version checking by passing in a "DRIVER_VER_INFO" structure
				vi.cbStruct = sizeof(DRIVER_VER_INFO);

				WINTRUST_CATALOG_INFO wtci = { 0 };

				wtci.cbStruct = sizeof(wtci);

				wtci.pcwszCatalogFilePath = ci.wszCatalogFile;

				wtci.pcwszMemberFilePath = fileName;

				wtci.pcwszMemberTag = hashTag;

				wtci.pbCalculatedFileHash = hash;

				wtci.cbCalculatedFileHash = sizeHash;

				wtci.hCatAdmin = catAdmin;

				stateSignature = Verify(file, WTD_CHOICE_CATALOG, &wtci, &IID_DriverActionVerify, &vi, &signaturesTemp, &countSignaturesTemp);

				if (vi.pcSignerCertContext)
				{
					CertFreeCertificateContext(vi.pcSignerCertContext);
				}
			}

			CryptCATAdminReleaseCatalogContext(catAdmin, catInfo, 0);
		}
		else
		{
			// TODO: Other catalogs?
		}

		CryptCATAdminReleaseContext(catAdmin, 0);
	}

	*signatures = signaturesTemp;

	*countSignatures = countSignaturesTemp;

	return stateSignature;
}

xResult xSignatureDataCollector::GetSignatures(HANDLE stateData, PCERT_CONTEXT** signatures, uint32_t* countSignatures)
{
	PCRYPT_PROVIDER_DATA cpd = WTHelperProvDataFromStateData(stateData);

	if (!cpd)
	{
		return X_E_FAIL;
	}

	PCRYPT_PROVIDER_SGNR cpsgnr = NULL;

	uint32_t ui = 0;

	uint32_t countSignaturesTemp = 0;

	while (cpsgnr = WTHelperGetProvSignerFromChain(cpd, ui, FALSE, 0))
	{
		if (cpsgnr->csCertChain != 0)
		{
			countSignaturesTemp++;
		}

		ui++;
	}

	PCERT_CONTEXT* signaturesTemp = NULL;

	if (countSignaturesTemp != 0)
	{
		signaturesTemp = (PCERT_CONTEXT*) xMemoryAlloc(countSignaturesTemp * sizeof(PCERT_CONTEXT));

		ui = 0;

		uint32_t index = 0;

		while (cpsgnr = WTHelperGetProvSignerFromChain(cpd, ui, FALSE, 0))
		{
			if (cpsgnr->csCertChain != 0)
			{
				signaturesTemp[index++] = (PCERT_CONTEXT) CertDuplicateCertificateContext(cpsgnr->pasCertChain[0].pCert);
			}

			ui++;
		}
	}

	if (signaturesTemp && countSignaturesTemp)
	{
		*signatures = signaturesTemp;

		*countSignatures = countSignaturesTemp;
	}

	return X_S_OK;
}

xResult xSignatureDataCollector::GetSigner(PCERT_CONTEXT certificate, xChar** signer)
{
	// Certificate context -> certificate info
	PCERT_INFO ci = certificate->pCertInfo;

	if (!ci)
	{
		return X_E_FAIL;
	}

	// Certificate info subject -> subject X.500 string
	uint32_t size = CertNameToStr(X509_ASN_ENCODING, &ci->Subject, CERT_X500_NAME_STR, NULL, 0);

	xPtr<xChar> name((xChar*) xMemoryAlloc(size * sizeof(xChar)));

	CertNameToStr(X509_ASN_ENCODING, &ci->Subject, CERT_X500_NAME_STR, name, size);

	// Subject X.500 string -> CN or OU value
	xPtr<xChar> value;

	xResult r = GetSubjectValue(name, L"CN", &value);

	if (xFailed(r) && xFailed(r = GetSubjectValue(name, L"OU", &value)))
	{
		return r;
	}

	*signer = value.Detach();

	return r;
}

xResult xSignatureDataCollector::GetSubjectValue(const xChar* name, const xChar* key, xChar** value)
{
	xString nameTemp(name);

	xString keyTemp(key);

	keyTemp += L'=';

	int32_t start = nameTemp.Find(keyTemp);

	if (start == -1)
	{
		return X_E_FAIL;
	}

	start += keyTemp.GetLength();

	xString valueTemp = nameTemp.Right(nameTemp.GetLength() - start);

	valueTemp.TrimLeft('"');

	valueTemp.TrimRight('"');

	*value = valueTemp.Clone();

	return X_S_OK;
}

void xSignatureDataCollector::Free(PCERT_CONTEXT* signatures, uint32_t countSignatures)
{
	if (signatures)
	{
		for (uint32_t ui = 0; ui < countSignatures; ui++)
		{
			CertFreeCertificateContext(signatures[ui]);
		}

		xMemoryFree(signatures);
	}
}
