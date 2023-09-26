#pragma once



#define __X_NOOP		__noop
#define __X_STDCALL		__stdcall
#define __X_CDECL		__cdecl
#define __X_NOVTABLE	__declspec(novtable)
#define __X_SELECTANY	__declspec(selectany)
#define __X_INLINE		inline



#define X_PURE = 0



#define X_SELF (*this)



#define X_TOKENIZE(__Token)			__Token
#define X_TOKENIZE_EXPAND(__Token)	X_TOKENIZE(__Token)

#define X_CONCAT_EXPAND_2(__Concat_1, __Concat_2)				X_TOKENIZE_EXPAND(__Concat_1) ## X_TOKENIZE_EXPAND(__Concat_2)
#define X_CONCAT_EXPAND_3(__Concat_1, __Concat_2, __Concat_3)	X_TOKENIZE_EXPAND(__Concat_1) ## X_TOKENIZE_EXPAND(__Concat_2) ## X_TOKENIZE_EXPAND(__Concat_3)
