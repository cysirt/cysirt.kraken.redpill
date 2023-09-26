#pragma once

#include <wchar.h>

#include "xAssert.h"
#include "xMemory.h"


#ifdef UNICODE
	typedef wchar_t xChar;
#else // UNICODE
	typedef char xChar;
#endif



#define X_CHAR_W(__Char)					L##__Char
#define X_CHAR_STRINGIZE_W(__Char)			L ## #__Char
#define X_CHAR_STRINGIZE_EXPAND_W(__Char)	X_CHAR_STRINGIZE_W(__Char)
#define X_CHAR_EMPTY_W						X_CHAR_W("")
#define X_CHAR_BLANK_W						X_CHAR_W(" ")
#define X_CHAR_FORMAT_HEX_2_W				X_CHAR_W("0x%02x")
#define X_CHAR_FORMAT_HEX_4_W				X_CHAR_W("0x%04x")
#define X_CHAR_FORMAT_HEX_8_W				X_CHAR_W("0x%08x")
#define X_CHAR_FORMAT_HEX_16_W				X_CHAR_W("0x%016llx")

#define X_CHAR_A(__Char)					__Char
#define X_CHAR_STRINGIZE_A(__Char)			#__Char
#define X_CHAR_STRINGIZE_EXPAND_A(__Char)	X_CHAR_STRINGIZE_A(__Char)
#define X_CHAR_EMPTY_A						X_CHAR_A("")
#define X_CHAR_BLANK_A						X_CHAR_A(" ")
#define X_CHAR_FORMAT_HEX_2_A				X_CHAR_A("0x%02x")
#define X_CHAR_FORMAT_HEX_4_A				X_CHAR_A("0x%04x")
#define X_CHAR_FORMAT_HEX_8_A				X_CHAR_A("0x%08x")
#define X_CHAR_FORMAT_HEX_16_A				X_CHAR_A("0x%016llx")

#ifdef UNICODE
	#define X_CHAR(__Char)					X_CHAR_W(__Char)
	#define X_CHAR_STRINGIZE(__Char)		X_CHAR_STRINGIZE_W(__Char)
	#define X_CHAR_STRINGIZE_EXPAND(__Char)	X_CHAR_STRINGIZE_EXPAND_W(__Char)
	#define X_CHAR_EMPTY					X_CHAR_EMPTY_W
	#define X_CHAR_BLANK					X_CHAR_BLANK_W
	#define X_CHAR_FORMAT_HEX_2				X_CHAR_FORMAT_HEX_2_W
	#define X_CHAR_FORMAT_HEX_4				X_CHAR_FORMAT_HEX_4_W
	#define X_CHAR_FORMAT_HEX_8				X_CHAR_FORMAT_HEX_8_W
	#define X_CHAR_FORMAT_HEX_16			X_CHAR_FORMAT_HEX_16_W
#else // UNICODE
	#define X_CHAR(__Char)					X_CHAR_A(__Char)
	#define X_CHAR_STRINGIZE(__Char)		X_CHAR_STRINGIZE_A(__Char)
	#define X_CHAR_STRINGIZE_EXPAND(__Char)	X_CHAR_STRINGIZE_EXPAND_A(__Char)
	#define X_CHAR_EMPTY					X_CHAR_EMPTY_A
	#define X_CHAR_BLANK					X_CHAR_BLANK_A
	#define X_CHAR_FORMAT_HEX_2				X_CHAR_FORMAT_HEX_2_A
	#define X_CHAR_FORMAT_HEX_4				X_CHAR_FORMAT_HEX_4_A
	#define X_CHAR_FORMAT_HEX_8				X_CHAR_FORMAT_HEX_8_A
	#define X_CHAR_FORMAT_HEX_16			X_CHAR_FORMAT_HEX_16_A
#endif



#define xCharCatA			strcat
#define xCharCompareA		strcmp
#define xCharCompareNoCaseA	stricmp
#define xCharCopyLengthA	strncpy
#define xCharFindA			strstr
#define xCharFormatTimeA	strftime
#define xCharLengthA		strlen
#define xCharToIntA			atoi

#define xCharCatW			wcscat
#define xCharCompareW		wcscmp
#define xCharCompareNoCaseW	_wcsicmp
#define xCharCopyLengthW	wcsncpy
#define xCharFindW			wcsstr
#define xCharFormatTimeW	wcsftime
#define xCharLengthW		wcslen
#define xCharToIntW			_wtoi

#ifdef UNICODE
	#define xCharAppend			xCharAppendW
	#define xCharCat			xCharCatW
	#define xCharCompare		xCharCompareW
	#define xCharCompareNoCase	xCharCompareNoCaseW
	#define xCharCopyLength		xCharCopyLengthW
	#define xCharFind			xCharFindW
	#define xCharFormatTime		xCharFormatTimeW
	#define xCharIsEmpty		xCharIsEmptyW
	#define xCharLength			xCharLengthW
	#define xCharToInt			xCharToIntW
	#define xCharClone			xCharCloneW
#else // UNICODE
	#define xCharAppend			xCharAppendA
	#define xCharCat			xCharCatA
	#define xCharCompare		xCharCompareA
	#define xCharCompareNoCase	xCharCompareNoCaseA
	#define xCharCopyLength		xCharCopyLengthA
	#define xCharFind			xCharFindA
	#define xCharFormatTime		xCharFormatTimeA
	#define xCharIsEmpty		xCharIsEmptyA
	#define xCharLength			xCharLengthA
	#define xCharToInt			xCharToIntA
	#define xCharClone			xCharCloneA
#endif



static wchar_t* xCharAppendW(const wchar_t* s, const wchar_t* appendee)
{
	X_ASSERT(s && appendee);

	if (!s || !appendee)
	{
		return NULL;
	}

	const size_t length1 = xCharLengthW(s);

	const size_t length2 = xCharLengthW(appendee);

	uint32_t size = (uint32_t) (xCharLengthW(s) + xCharLengthW(appendee) + 1) * sizeof(wchar_t);

	wchar_t* result =  (wchar_t*) xMemoryAlloc(size);

	if (!result)
	{
		return NULL;
	}

	xCharCatW(result, s);

	xCharCatW(result, appendee);

	return result;
}

static char* xCharAppendA(const char* s, const char* appendee)
{
	X_ASSERT(s && appendee);

	if (!s || !appendee)
	{
		return NULL;
	}

	const size_t length1 = xCharLengthA(s);

	const size_t length2 = xCharLengthA(appendee);

	uint32_t size = (uint32_t) (xCharLengthA(s) + xCharLengthA(appendee) + 1) * sizeof(char);

	char* result =  (char*) xMemoryAlloc(size);

	if (!result)
	{
		return NULL;
	}

	xCharCatA(result, s);

	xCharCatA(result, appendee);

	return result;
}

static wchar_t* xCharCloneW(const wchar_t* clonee)
{
	X_ASSERT(clonee);

	if (!clonee)
	{
		return NULL;
	}

	uint32_t size = (uint32_t) (xCharLengthW(clonee) + 1) * sizeof(wchar_t);

	return (wchar_t*) xMemoryClone(clonee, size);
}

static char* xCharCloneA(const char* clonee)
{
	X_ASSERT(clonee);

	if (!clonee)
	{
		return NULL;
	}

	uint32_t size = (uint32_t) (xCharLengthA(clonee) + 1) * sizeof(char);

	return (char*) xMemoryClone(clonee, size);
}

static bool xCharIsEmptyW(const wchar_t* s)
{
	X_ASSERT(s);

	if (!s)
	{
		return false;
	}

	return xCharCompareW(s, X_CHAR_EMPTY_W) == 0;
}

static bool xCharIsEmptyA(const char* s)
{
	X_ASSERT(s);

	if (!s)
	{
		return false;
	}

	return xCharCompareA(s, X_CHAR_EMPTY_A) == 0;
}
