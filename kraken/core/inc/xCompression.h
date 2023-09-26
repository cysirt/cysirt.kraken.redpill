#pragma once

#include "xArray.h"
#include "xChar.h"
#include "xResult.h"
#include "xString.h"



class xCompression
{
public:
	xCompression();
	~xCompression();

	xResult Add(const xChar* fileName);
	xResult Compress(const xChar* fileName);

private:
	xArray<xString> m_fileNames;
};
