/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:34 $
 * $Revision: 1.2 $
 */

#include <ansi_parms.h>
#include <abort_exit.h>
#include <critical_regions.h>
#include <cxxabi_globaldestr.h>
#include <ewl_thread_local_data.h>

#include <ewl_misra_types.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
#define max_funcs 32
DestructorChain __atexit_funcs[max_funcs];
int32_t __atexit_curr_func;
#else
#define max_funcs MAX_EXIT_FUNCTIONS
#endif

extern void __crt0_end(void);

void _EWL_CDECL _ExitProcess(void)
{
	__crt0_end();
}


int_t __register_atexit(void (_EWL_CDECL *func)(void))
{
	if (_EWL_LOCALDATA(__atexit_curr_func) == max_funcs) {
		return(-1);
	}

	__begin_critical_region(atexit_funcs_access);

	(void)__register_global_object(0L, (void (*)(void *func))func, &_EWL_LOCALDATA(__atexit_funcs)[_EWL_LOCALDATA(__atexit_curr_func)++]);

	__end_critical_region(atexit_funcs_access);

	return 0;
}
