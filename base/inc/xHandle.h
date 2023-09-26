#pragma once

#include <Windows.h>

#include "xReferenceCount.h"



class xHandlePlatform
{
public:
	struct _Traits
	{
		struct _RefCount
		{
			typedef HANDLE Type;

			static Type Invalid()
			{
				return NULL;
			}

			static uint32_t AddRef(Type type)
			{
				return ~0u;
			}

			static uint32_t Release(Type type)
			{
				return CloseHandle(type) ? 0 : ~0u;
			}
		};
	};
};

typedef xRefCountType<xHandlePlatform::_Traits::_RefCount> xHandle;



class xFileHandlePlatform
{
public:
	struct _Traits
	{
		struct _RefCount
		{
			typedef HANDLE Type;

			static Type Invalid()
			{
				return INVALID_HANDLE_VALUE;
			}

			static uint32_t AddRef(Type type)
			{
				return ~0u;
			}

			static uint32_t Release(Type type)
			{
				return CloseHandle(type) ? 0 : ~0u;
			}
		};
	};
};

typedef xRefCountType<xFileHandlePlatform::_Traits::_RefCount> xFileHandle;



class xModuleHandlePlatform
{
public:
	struct _Traits
	{
		struct _RefCount
		{
			typedef HMODULE Type;

			inline static Type Invalid()
			{
				return NULL;
			}

			inline static uint32_t AddRef(Type type)
			{
				return ~0u;
			}

			inline static uint32_t Release(Type type)
			{
				return FreeLibrary(type) ? 0 : ~0u;
			}
		};
	};
};

typedef xRefCountType<xModuleHandlePlatform::_Traits::_RefCount> xModuleHandle;



class xEventHandlePlatform
{
public:
	struct _Traits
	{
		struct _RefCount
		{
			typedef HMODULE Type;

			inline static Type Invalid()
			{
				return NULL;
			}

			inline static uint32_t AddRef(Type type)
			{
				return ~0u;
			}

			inline static uint32_t Release(Type type)
			{
				return FreeLibrary(type) ? 0 : ~0u;
			}
		};
	};
};

typedef xRefCountType<xModuleHandlePlatform::_Traits::_RefCount> xModuleHandle;