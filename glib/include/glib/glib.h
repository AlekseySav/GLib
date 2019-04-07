#ifndef _GLIB
#define _GLIB 3

#undef _NULL_DRIVER
#undef _WINDOWS_DRIVER
#undef _LINUX_DRIVER

#define _NULL_DRIVER 0
#define _WINDOWS_DRIVER 1
#define _LINUX_DRIVER 2

#undef _SYSTEM_32
#undef _SYSTEM_64

#undef _USER_DRIVER

#if defined(_WIN64)
	#define _USER_DRIVER _WINDOWS_DRIVER
	#define _SYSTEM_64
#elif defined(_WIN32)
	#define _USER_DRIVER _WINDOWS_DRIVER
	#define _SYSTEM_32
#elif defined(__LINUX__)
	#define _USER_DRIVER _LINUX_DRIVER

	#if defined(__LP64__) || defined(_M_IA64)
		#define _SYSTEM_64
	#else
		#define _SYSTEM_32
	#endif
#else
	#define _USER_DRIVER _NULL_DRIVER
#endif

#if _USER_DRIVER != _NULL_DRIVER
#if defined(__cplusplus)

#ifndef NULL
	#if __cplusplus < 11
		#define NULL 0
	#else
		#define NULL nullptr
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

#ifndef LINK_C
	#define LINK_C extern
#endif

#ifndef EXTERN
	#define EXTERN extern "C++"
#endif

#ifdef _SYSTEM_32
	typedef unsigned int u_int32;
	typedef signed int int32;
	typedef unsigned long int dword;
#elif defined(_SYSTEM_64)
	#if _USER_DRIVER == _WINDOWS_DRIVER
		typedef unsigned __int32 u_int32;
		typedef signed __int32 int32;
	#elif _USER_DRIVER == _LINUX_DRIVER
		typedef unsigned int u_int32;
		typedef signed int int32;
	#endif
	typedef unsigned int dword;
#endif

typedef unsigned int u_int;
typedef unsigned short u_int16;
typedef signed short int16;

typedef unsigned char byte;
typedef unsigned short int word;

#if __cplusplus < 11
	#undef bool
	#undef true
	#undef false

	typedef int bool;
	#define true 1
	#define false 0
#endif

#if _USER_DRIVER == _WINDOWS_DRIVER
	#define _FUNCTION_HANDLE(__name__) WIN_##__name__
#elif _USER_DRIVER == _LINUX_DRIVER
	#define _FUNCTION_HANDLE(__name__) LINUX_##__name__
#endif

#define EX_MainLoop  _FUNCTION_HANDLE(MainLoop)
#define EX_CreateConsole _FUNCTION_HANDLE(CreateConsole)

EXTERN int glibLoop();
EXTERN int glibCreateConsole();

LINK_C int gmain(char * argv[], int argc); //main function

typedef void * Handle;

#include "sys/event.h"
#include "sys/bitmap.h"
#include "sys/graphics.h"
#include "sys/text.h"
#include "sys/window.h"

#endif
#else
#undef _GLIB
#endif
#else
#undef _GLIB
#endif

#ifndef _GLIB
	#error Can't implement GLib
#endif
