#pragma once

#ifndef _GLIB
	#define _GLIB 2
#endif

#ifdef WIN32
	#define _USES_WINDOWS_DRIVER
#elif defined(__LINUX__)
	#define _USES_LINUX_DRIVER
#else
	#define _USES_DEFAULT_DRIVER
#endif

#ifndef NULL
	#ifdef __cplusplus
		#if __cplusplus < 17
			#define NULL 0
		#else
			#define NULL nullptr
		#endif
	#else
		#define NULL (void *)0
	#endif
#endif

#ifndef _INC_STDIO
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#include <stdio.h>
#endif

#ifndef _INC_STDLIB
	#include <stdlib.h>
#endif

#undef EXTERN_C

#ifdef __cplusplus
	#define EXTERN_C extern "C"
#else
	#define EXTERN_C extern
#endif

#ifndef EXTERN
	#define EXTERN EXTERN_C
#endif

#ifdef _USES_WINDOWS_DRIVER
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef unsigned __int8 u_int8;
typedef unsigned __int16 u_int16;
typedef unsigned __int32 u_int32;
#else
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef unsigned int8_t u_int8;
typedef unsigned int16_t u_int16;
typedef unsigned int32_t u_int32;
#endif

typedef unsigned int u_int;

typedef u_int8 byte;
typedef u_int16 word;
typedef u_int32 dword;

#define MAKE_WORD(byte1, byte2) ((word)((byte1 & 0xff) | ((byte2 & 0xff) << 8)))
#define MAKE_DWORD(byte1, byte2, byte3, byte4) ((word)((byte1 & 0xff) | ((byte2 & 0xff) << 8) | ((byte3 & 0xff) << 16) | ((byte4 & 0xff) << 24)))
#define FREE_WORD(_word, byte_ptr) do { *byte_ptr = (byte)(_word & 0xff); *(byte_ptr + 1) = (byte)((_word >> 8) & 0xff); } while(0)
#define FREE_DWORD(_dword, byte_ptr) do { *byte_ptr = (byte)(_dword & 0xff); *(byte_ptr + 1) = (byte)((_dword >> 8) & 0xff); \
								*(byte_ptr + 2) = (byte)((_dword >> 16) & 0xff); *(byte_ptr + 3) = (byte)((_dword >> 24) & 0xff);} while(0)

#undef FAILED
#define FAILED(x) (x < 0)

#ifndef __cplusplus

#undef bool
#undef true
#undef false

typedef int bool;
#define true 1
#define false 0
#endif

#ifdef _USES_WINDOWS_DRIVER
#define _FUNCTION_HANDLE(__name__) WIN_##__name__
#elif defined(_USES_LINUX_DRIVER)
#define _FUNCTION_HANDLE(__name__) LINUX_##__name__
#elif defined(_USES_DEFAULT_DRIVER)
#define _FUNCTION_HANDLE(__name__) DEF_##__name__
#endif

#define EX_MainLoop  _FUNCTION_HANDLE(MainLoop)

EXTERN int glibLoop();

EXTERN_C int gmain(char * argv[], int argc);

typedef void * Handle;

#include "sys/event.h"
#include "sys/graphics.h"
#include "sys/window.h"
