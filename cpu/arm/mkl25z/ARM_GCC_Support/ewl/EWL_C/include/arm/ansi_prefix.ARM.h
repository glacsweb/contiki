/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/02/17 09:43:34 $
 * $Revision: 1.12 $
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>

#define __dest_os __arm_bare

#define _USE_AEABI_PRIVATE_  1

/*********************************************/
/* 		OS Specific Configuration Flags		 */
/*********************************************/

/* Console IO Support Configuration Flags */

#ifndef _EWL_CONSOLE_SUPPORT
	#define _EWL_CONSOLE_SUPPORT	1
#endif

#if _EWL_CONSOLE_SUPPORT
	#define _EWL_BUFFERED_CONSOLE			0
#endif

#if !defined(__STDC_WANT_LIB_EXT1__)
#define __STDC_WANT_LIB_EXT1__ 				1
#endif

#if !defined(_lint)
#define _EWL_C99_TC2_FENV					1
#endif

#define _EWL_C99_MATH_LEAK                  1

/* File IO Support Configuration Flags */

#ifndef _EWL_OS_DISK_FILE_SUPPORT
	#define _EWL_OS_DISK_FILE_SUPPORT	0
#endif

/* Time Support Configuration Flags */

#ifndef _EWL_OS_TIME_SUPPORT
	#define _EWL_OS_TIME_SUPPORT	1
#endif

#ifndef _EWL_CLOCK_T_AVAILABLE
	#define _EWL_CLOCK_T_AVAILABLE 	1
#endif

#ifndef _EWL_TIME_T_AVAILABLE
	#define _EWL_TIME_T_AVAILABLE	1
#endif

#if _EWL_OS_TIME_SUPPORT && ((!_EWL_CLOCK_T_AVAILABLE)  || (!_EWL_TIME_T_AVAILABLE))
	#error _EWL_CLOCK_T_AVAILABLE and _EWL_CLOCK_T_AVAILABLE must match _EWL_OS_TIME_SUPPORT
#endif

/* Thread Safety Configuration Flags */

#ifndef _EWL_THREADSAFE
	#define _EWL_THREADSAFE 0
#endif

#ifndef _EWL_PTHREADS
	#define _EWL_PTHREADS 0
#endif

#if _EWL_PTHREADS && !_EWL_THREADSAFE
	#error _EWL_PTHREADS and _EWL_THREADSAFE must match
#endif

#ifndef _EWL_LOCALDATA_AVAILABLE
	#define _EWL_LOCALDATA_AVAILABLE 0
#endif

/* Memory Allocation Flags */

/*  See alloc.c for more details on configuration options */

#ifndef _EWL_OS_ALLOC_SUPPORT
	#define _EWL_OS_ALLOC_SUPPORT 		0
#endif

#ifndef _EMBEDDED_WARRIOR_MALLOC
#define _EMBEDDED_WARRIOR_MALLOC      1
#endif

#ifndef _EWL_HEAP_EXTERN_PROTOTYPES
	#define _EWL_HEAP_EXTERN_PROTOTYPES \
		extern char_t __heap_addr[]; \
		extern char_t __heap_size[];
#endif

#ifndef _EWL_HEAP_START
	#define _EWL_HEAP_START __heap_addr
#endif

#ifndef _EWL_HEAP_SIZE
	#define _EWL_HEAP_SIZE __heap_size
#endif

/* #define _EWL_NO_MALLOC_DESIGN_2 */ /* lower overhead but slower allocation */

/* #define _EWL_NO_FIX_MALLOC_POOLS */ /* For heaps that are less than 64k */


/********************************************/
/* 		Additional Portability Flags 		*/
/********************************************/

#ifndef _EWL_COMPLEX_SUPPORT
	#define _EWL_COMPLEX_SUPPORT 0
#endif

#ifndef _EWL_LONGLONG
	#define _EWL_LONGLONG 1
#endif

#ifndef _EWL_FLOATING_POINT
	#define _EWL_FLOATING_POINT 0
#endif

#ifndef _EWL_WIDE_CHAR
	#define _EWL_WIDE_CHAR 0
#endif

#ifndef _EWL_USES_SUN_MATH_LIB
	#define _EWL_USES_SUN_MATH_LIB 1
#endif

#ifndef _EWL_POSIX
	#define _EWL_POSIX 0
#endif

#ifndef _EWL_NEEDS_EXTRAS
	#define _EWL_NEEDS_EXTRAS 0
#endif

#ifndef _EWL_ACCURATE_BUT_LARGE_ANSI_FP
    #define _EWL_ACCURATE_BUT_LARGE_ANSI_FP	0
#endif

#ifndef __EWL_ROUND_DECIMAL_ANSI_FP__
	#define __EWL_ROUND_DECIMAL_ANSI_FP__ 	1
#endif

/************************************************/
/* 		Flags to reduce code and data size 		*/
/************************************************/

#ifndef _EMBEDDED_WARRIOR_CTYPE
#define _EMBEDDED_WARRIOR_CTYPE 			1
#endif

#ifndef _EWL_STRERROR_KNOWS_ERROR_NAMES
	#define _EWL_STRERROR_KNOWS_ERROR_NAMES 0
#endif

#ifndef _EWL_ASSERT_DISPLAYS_FUNC
	#define _EWL_ASSERT_DISPLAYS_FUNC	0
#endif

#ifndef _EWL_C_LOCALE_ONLY
	#define _EWL_C_LOCALE_ONLY	1
#endif

#ifndef _EWL_C99
	#define _EWL_C99	0
#endif

#if !_EWL_C99 && !_EWL_C_LOCALE_ONLY
	#error _EWL_C_LOCALE_ONLY must be turned on if _EWL_C99 is off
#endif

#ifndef _EWL_FLOATING_POINT_IO
	#define _EWL_FLOATING_POINT_IO	0
#endif

#ifndef _EWL_C99_PRINTF_SCANF
	#define _EWL_C99_PRINTF_SCANF 			0
#endif

#ifndef _EWL_USES_EXIT_PROCESS
	#define _EWL_USES_EXIT_PROCESS 1
#endif

#ifndef _EWL_FLT_EVAL_METHOD
	#define _EWL_FLT_EVAL_METHOD  0
#endif

#if defined(__cplusplus)
    #define _EWL_USING_NAMESPACE
#endif

#define __ANSI_OVERLOAD__

#define _EWL_INTEGRAL_MATH

/************************************************/
/* 		ARM EABI								*/
/************************************************/

#ifndef _AEABI_PORTABILITY_LEVEL
	#define _AEABI_PORTABILITY_LEVEL 0
#endif

#if _AEABI_PORTABILITY_LEVEL && (!_lint)
	typedef long long __int64;
#endif

#define _EWL_DOUBLE_SIZE 			64

#endif /*	__ansi_prefix__	  */
