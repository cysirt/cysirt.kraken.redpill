#pragma once

#include <stdint.h>
#include <Windows.h>

#include "xReferenceCount.h"



class xCriticalSection
{
public:
	xCriticalSection()
	{
		InitializeCriticalSection(&m_cs);
	}

	~xCriticalSection()
	{
		DeleteCriticalSection(&m_cs);
	}

   bool Lock(const bool wait = true)
   {
		return wait ? (EnterCriticalSection(&m_cs), true) : !!TryEnterCriticalSection(&m_cs);
   }

   void Unlock()
   {
	   LeaveCriticalSection(&m_cs);
   }

   struct _Traits
   {
	   struct _RefCount
	   {
		   typedef xCriticalSection* Type;

		   inline static Type Invalid()
		   {
			   return NULL;
		   }

		   inline static uint32_t AddRef(Type type)
		   {
			   X_ASSERT(type);

			   if (type)
			   {
				   type->Lock();
			   }

			   return 1;
		   }

		   inline static uint32_t Release(Type type)
		   {
			   X_ASSERT(type);

			   if (type)
			   {
				   type->Unlock();
			   }

			   return 0;
		   }
	   };
   };

private:
	xCriticalSection(const xCriticalSection& cs)
	{
	}
	
	xCriticalSection& operator=(const xCriticalSection& cs)
	{
	}

private:
	CRITICAL_SECTION m_cs;
};

typedef xRefCountType<xCriticalSection::_Traits::_RefCount> xCriticalSectionLock;
