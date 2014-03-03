/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/14 12:39:16 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		__sys_alloc
 *		__sys_free
 *
 *	Note
 *	----
 *
 *		These routines, being platform-specific, were split off of pool_alloc.c.
 *
 */

#include <stddef.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pool_alloc.h"
#if !(_EWL_CLASSIC_MALLOC || _EWL_PRO4_MALLOC)

#ifndef __SYS_ALLOC
	#define __SYS_ALLOC __sys_alloc
#endif

#ifndef __SYS_FREE
	#define __SYS_FREE __sys_free
#endif

void * _EWL_CDECL __SYS_ALLOC(size_t size)   /*- hh 980122 -*/
{
	char* ptr;
	size += 8;
#if __dest_os==__win32_os
	ptr = (char*)GlobalAlloc(GMEM_FIXED, size);
#else
	ptr = (char*)LocalAlloc(LMEM_FIXED, size);
#endif
	if (ptr == 0)
		return 0;
	if ((size_t)ptr % 8 == 0)
	{
		ptr += 4;
		*(size_t*)ptr = 0;
	}
	else
		*(size_t*)ptr = 1;
	ptr += 4;
	return (void*)ptr;
}

void _EWL_CDECL __SYS_FREE(void *p)   /*- hh 980122 -*/
{
	char* ptr = (char*)p;
	ptr -= 4;
	if (*(size_t*)ptr == 0)
		ptr -= 4;
#if __dest_os==__win32_os
	GlobalFree((HGLOBAL) ptr);
	#else
	LocalFree((HGLOBAL) ptr);
	#endif
}

size_t _EWL_CDECL __sys_pointer_size(void* p)
{
	char* ptr = (char*)p;
	ptr -= 4;
	if (*(size_t*)ptr == 0)
		ptr -= 4;
#if __dest_os==__win32_os
	return GlobalSize(ptr);
#else
	return LocalSize(ptr);
#endif
}

#else /* _EWL_CLASSIC_MALLOC */

void * __sys_alloc(mem_size size, mem_pool_obj* /*poolObj*/)   /*- hh 980122 -*/
{
    #if __dest_os==__win32_os
	return((void *) GlobalAlloc(GMEM_FIXED,size));
	#else
	return((void *) LocalAlloc(LMEM_FIXED,size));
	#endif
}

void __sys_free(void *ptr, mem_pool_obj* /*poolObj*/)   /*- hh 980122 -*/
{
#if __dest_os==__win32_os
	GlobalFree((HGLOBAL) ptr);
	#else
	LocalFree((HGLOBAL) ptr);
	#endif
}

#endif

/* Change record:
 * hh  980122 Replaced <windows.h> with the following TWO includes because it is seriously
 *            broken.  The following 2 includes must be carefully ordered as shown, because
 *            they are broken too.
 * hh  980122 commented out unused arg in 2 places
 * mf  980518 wince changes
 * mf  980624 don't include x86_prefix.h
 * hh  990228 removed unused arg, interface changed
 * hh  990228 changed mem_size to size_t, interface changed
 * hh  990409 Patched for fact that GlobalAlloc returns 4-byte aligned on '95/'98
 * hh  000523 Added __sys_pointer_size
 * cc  020109 Added _EWL_CDECL
 * hh  020214 Renamed _EWL_PRO4_MALLOC to _EWL_CLASSIC_MALLOC
 */
