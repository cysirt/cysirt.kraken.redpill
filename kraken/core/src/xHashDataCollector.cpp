#include "xHashDataCollector.h"

#include "xConversion.h"
#include "xFile.h"
#include "xLog.h"

#include "xDataCache.h"
#include "xHashData.h"

#include "LibMd5.h"
#include "LibSha1.h"
#include "LibSha256.h"



#define X_HASH_DATA_COLLECTOR_SIZE_FILE_BUFFER	4096



xHashDataCollector::xHashDataCollector()
{

}

xHashDataCollector::~xHashDataCollector()
{

}

xResult xHashDataCollector::Collect(const xChar* fileName, IxHashData** hashData)
{
	X_ASSERT(fileName && hashData);

	if (!fileName || !hashData)
	{
		return X_E_INVALID_ARGUMENT;
	}

	xInterfacePtr<IxInterface> iface;

	xResult r = xDataCacheGet().Contains(fileName, IID_IxHashData, &iface);

	if (xSucceeded(r))
	{
		return iface->QueryInterface(IID_IxTimeData, (void**) hashData);
	}
	else
	{
		FILE* file = xFileOpen(fileName, L"rb");

		if (!file)
		{
			return xResultFromErrNo();
		}

		Md5Context md5Ctx;

		Md5Initialise(&md5Ctx);

		Sha1Context sha1Ctx;

		Sha1Initialise(&sha1Ctx);

		Sha256Context sha256Ctx;

		Sha256Initialise(&sha256Ctx);

		size_t size = 0;

		uint8_t buffer[X_HASH_DATA_COLLECTOR_SIZE_FILE_BUFFER];

		while (size = fread(buffer, 1, X_HASH_DATA_COLLECTOR_SIZE_FILE_BUFFER, file))
		{
			Md5Update(&md5Ctx, buffer, (uint32_t) size);

			Sha1Update(&sha1Ctx, buffer, (uint32_t) size);

			Sha256Update(&sha256Ctx, buffer, (uint32_t) size);
		}

		xFileClose(file);

		MD5_HASH md5Hash;

		Md5Finalise(&md5Ctx, &md5Hash);

		SHA1_HASH sha1Hash;

		Sha1Finalise(&sha1Ctx, &sha1Hash);

		SHA256_HASH sha256Hash;

		Sha256Finalise(&sha256Ctx, &sha256Hash);

		xInterfacePtr<xHashData> hd(new xHashData);

		if (!hd)
		{
			return X_E_OUT_OF_MEMORY;
		}

		hd->SetMd5(xConvertBinaryToHex(md5Hash.bytes, MD5_HASH_SIZE));

		hd->SetSha1(xConvertBinaryToHex(sha1Hash.bytes, SHA1_HASH_SIZE));

		hd->SetSha256(xConvertBinaryToHex(sha256Hash.bytes, SHA256_HASH_SIZE));

		xDataCacheGet().Add(fileName, IID_IxHashData, hd);

		*hashData = hd.Detach();
	}

	return X_S_OK;
}