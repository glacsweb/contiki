/* Metrowerks Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 20:54:45 $
 * $Revision: 1.2 $
 */

#include <ansi_parms.h>
#include <stddef.h>

#if __dest_os == __mac_os
	#include <MacMemory.h>
	#define SYSMALLOCFUNC(x)	NewPtr(x)
	#define SYSMFREEFUNC(x)		DisposePtr(x)
	#define SYSPTRTYPE			Ptr
#else
	#include <stdlib.h>
	#define SYSMALLOCFUNC(x)	malloc(x)
	#define SYSMFREEFUNC(x)		free(x)
	#define SYSPTRTYPE			void *
#endif /* __dest_os == __mac_os */

_EWL_IMP_EXP_RUNTIME extern void *__vla_alloc(size_t size);
_EWL_IMP_EXP_RUNTIME extern void __vla_free(void *ptr);

void *__vla_alloc(size_t size)
{
	return (void *) SYSMALLOCFUNC(size);
}

void __vla_free(void *ptr)
{
	SYSMFREEFUNC((SYSPTRTYPE) ptr);
}
