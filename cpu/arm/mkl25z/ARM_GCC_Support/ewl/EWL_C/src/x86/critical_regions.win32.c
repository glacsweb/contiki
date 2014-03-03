/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/14 12:39:16 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		__init_critical_regions
 *		__begin_critical_region
 *		__end_critical_region
 */

#include <stddef.h>
#define WIN32_LEAN_AND_MEAN
#include <critical_regions.h>
#include <windows.h>

CRITICAL_SECTION __cs[num_critical_regions];

_EWL_BEGIN_EXTERN_C	/*- cc 010410 -*/

	extern CRITICAL_SECTION __cs[num_critical_regions];

	void _EWL_CDECL __init_critical_regions(void) _EWL_CANT_THROW
	{
			int i;
			for (i = 0; i < num_critical_regions; i++)
					InitializeCriticalSection(&__cs[i]);
	}

	void _EWL_CDECL __kill_critical_regions(void) _EWL_CANT_THROW
	{
			int i;
			for (i = 0; i < num_critical_regions; i++)
					DeleteCriticalSection(&__cs[i]);
	}
	
#if _EWL_THREADSAFE
	void _EWL_CDECL __begin_critical_region(int region) _EWL_CANT_THROW
	{
			EnterCriticalSection(&__cs[region]);	
	}

	void _EWL_CDECL __end_critical_region(int region) _EWL_CANT_THROW
	{
			LeaveCriticalSection(&__cs[region]);
	}
#endif /* _EWL_THREADSAFE */

_EWL_END_EXTERN_C	/*- cc 010410 -*/

/* Change record:
 * hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
 *           broken.  The following 2 includes must be carefully ordered as shown, because
 *           they are broken too.          
 * mf 980624 don't include x86_prefix.h
 * cc 000518 fixed #include 
 * mf 000534 moved implementation of these into header
 *           critical_regions.win32.h as inlines since these
 *           were small and performance critical.
 */

