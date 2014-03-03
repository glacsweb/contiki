/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 08:05:47 $
 * $Revision: 1.6 $
 */

#include <ansi_parms.h>
#include <ansi_files.h>
#include <abort_exit.h>
#include <critical_regions.h>
#include <misc_io.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <ewl_thread_local_data.h>

#include <ewl_misra_types.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
#define max_funcs 32L
static void (*__atexit_funcs_C[max_funcs])(void); /* these will be used only for C compliant entry points */
int32_t __atexit_curr_func_C = 0;
#if _EWL_OS_DISK_FILE_SUPPORT
void _EWL_CDECL (* __stdio_exit)  (void) = __close_all;
#else
void _EWL_CDECL (* __stdio_exit)  (void) = 0;
#endif
void _EWL_CDECL (* __console_exit)(void) = 0;
#endif

void _EWL_CDECL abort(void)
{
	MISRA_EXCEPTION_RULE_20_8()
    (void)raise(SIGABRT);

    /* duplicate the exit code to prevent using yet another global */
    if (_EWL_LOCALDATA(__console_exit)) {
        (*_EWL_LOCALDATA(__console_exit))();
        _EWL_LOCALDATA(__console_exit) = 0;
    }

    _ExitProcess();
}

int_t _EWL_CDECL atexit(void (_EWL_CDECL *func)(void))
{
    int_t result;

    __begin_critical_region(atexit_funcs_access);

    result = __register_atexit(func);

    __end_critical_region(atexit_funcs_access);

    return result;
}

void _EWL_CDECL exit(int_t status)
{
    #pragma unused(status)
	MISRA_QUIET_UNUSED_ARGS()
 
    __destroy_global_chain(); /* need to move destroy global chain above __stdio_exit
		     as some static objects may have destructors that flush streams */
    if (_EWL_LOCALDATA(__stdio_exit)) {
        (*_EWL_LOCALDATA(__stdio_exit))();
        _EWL_LOCALDATA(__stdio_exit) = 0;
    }

    if (_EWL_LOCALDATA(__console_exit)) {
        (*_EWL_LOCALDATA(__console_exit))();
        _EWL_LOCALDATA(__console_exit) = 0;
    }

    _ExitProcess();
}

int_t _EWL_CDECL __atexit(void (_EWL_CDECL *func)(void))
{
    if (_EWL_LOCALDATA(__atexit_curr_func_C) == max_funcs) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    } 

    __begin_critical_region(atexit_funcs_access);

    _EWL_LOCALDATA(__atexit_funcs_C)[_EWL_LOCALDATA(__atexit_curr_func_C)++] = func;

    __end_critical_region(atexit_funcs_access);

    return 0; 
}

MISRA_EXCEPTION_RULE_16_2()
void _EWL_CDECL __exit(int_t status)
{
    #pragma unused(status)
    MISRA_QUIET_UNUSED_ARGS()

    __begin_critical_region(atexit_funcs_access);

    while (_EWL_LOCALDATA(__atexit_curr_func_C) > 0L) {
         (*_EWL_LOCALDATA(__atexit_funcs_C)[--_EWL_LOCALDATA(__atexit_curr_func_C)])();
    }

    __end_critical_region(atexit_funcs_access);

    __kill_critical_regions();

    if (_EWL_LOCALDATA(__console_exit)) {
        (*_EWL_LOCALDATA(__console_exit))();
        _EWL_LOCALDATA(__console_exit) = 0;
    }

    _ExitProcess();
}

#if _EWL_C99
void _EWL_CDECL _Exit(int_t status)
{
    #pragma unused(status)
    MISRA_QUIET_UNUSED_ARGS()
    _ExitProcess();
}
#endif

