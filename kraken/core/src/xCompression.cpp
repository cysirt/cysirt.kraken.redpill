#include "xCompression.h"

#include "xPath.h"
#include "xReferenceCount.h"

#include "XZip.h"



xCompression::xCompression()
{
}

xCompression::~xCompression()
{
}

xResult xCompression::Add(const xChar* fileName)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	m_fileNames.Add(fileName);

	return X_S_OK;
}

xResult xCompression::Compress(const xChar* fileName)
{
	X_ASSERT(fileName);

	if (!fileName)
	{
		return X_E_INVALID_ARGUMENT;
	}

	HZIP zip = CreateZip((void*) fileName, 0, ZIP_FILENAME);

	if (!zip)
	{
		return X_E_FAIL;
	}
		
	const uint32_t count = m_fileNames.GetCount();

	for (uint32_t ui = 0; ui < count; ui++)
	{
		if (GetFileAttributes(m_fileNames[ui]) == INVALID_FILE_ATTRIBUTES)
		{
			continue;
		}
		
		xPtr<xChar> name;

		if (xFailed(xPathGetLast(m_fileNames[ui], &name)))
		{
			continue;
		}

		if (ZipAdd(zip, name, (void*) m_fileNames[ui].GetString(), 0, ZIP_FILENAME) != ZR_OK)
		{
			continue;
		}
	}

	CloseZip(zip);

	return X_S_OK;
}
