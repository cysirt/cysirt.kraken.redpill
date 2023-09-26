#pragma once

#include <Windows.h>

#include <wincrypt.h>
#include <LM.h>
#include <Mscat.h>

#include "xReferenceCount.h"
#include "xCoreInterface.h"



class xSignatureDataCollector: public xRefCount<IxSignatureDataCollector>
{
X_INTERFACE_MAP_BEGIN
	X_INTERFACE_MAP_ENTRY(IxSignatureDataCollector)
X_INTERFACE_MAP_END

public:
	xSignatureDataCollector();
	virtual ~xSignatureDataCollector();

	X_INTERFACE_METHOD(xResult, Collect)(const xChar* fileName, IxSignatureData** signatureData);

private:
	xResult Verify(const xChar* fileName, uint32_t* state, PCERT_CONTEXT** signatures, uint32_t* countSignatures);
	uint32_t Verify(HANDLE file, unsigned long unionChoice, void* unionData, GUID* actionId, void* policyCallbackData, PCERT_CONTEXT** signatures, uint32_t* countSignatures);
	xResult GetFileHash(HANDLE file, const xChar* hashAlgorithm, uint8_t** hash, uint32_t* sizeHash, HANDLE* catAdmin);
	uint32_t VerifyFromCatalog(const xChar* fileName, HANDLE file, const xChar* hashAlgorithm, PCERT_CONTEXT** signatures, uint32_t* countSignatures);
	xResult GetSignatures(HANDLE stateData, PCERT_CONTEXT** signatures, uint32_t* countSignatures);
	xResult GetSigner(PCERT_CONTEXT certificate, xChar** signer);
	xResult GetSubjectValue(const xChar* name, const xChar* key, xChar** value);
	void Free(PCERT_CONTEXT* signatures, uint32_t countSignatures);
};

