#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>

#include "xAssert.h"



static void* xMemoryCopy(void* target, const void* source, const size_t size)
{
	return memcpy(target, source, size);
}



static void* xMemoryMove(void* target, const void* source, const size_t size)
{
	return memmove(target, source, size);
}



static int32_t xMemoryCompare(const void* mem1, const void* mem2, const size_t size)
{
	return memcmp(mem1, mem2, size);
}



static void* xMemorySet(void* mem, const int32_t data, const size_t size)
{
	return memset(mem, data, size);
}



static void* xMemoryZero(void* mem, const size_t size)
{
	return xMemorySet(mem, 0, size);
}



#define xMemoryAllocStack(__Size)		_alloca(__Size)



static void* xMemoryAlloc(const size_t size)
{
#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif

	void* mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);

#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif

	return mem;
}



static void* xMemoryRealloc(void* mem, const size_t size)
{
#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif

	void* temp = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mem, size);

#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif

	return temp;
}



static void xMemoryFree(void* mem)
{
#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif

	HeapFree(GetProcessHeap(), 0, mem);

#ifdef _DEBUG
	HeapValidate(GetProcessHeap(), 0, NULL);
#endif
}



static void* xMemoryClone(const void* mem, const size_t size)
{
	void* temp = xMemoryAlloc(size);

	if (temp)
	{
		xMemoryCopy(temp, mem, size);
	}

	return temp;
}