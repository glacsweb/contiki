/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/20 09:07:32 $
 * $Revision: 1.4 $
 */

#include <ansi_parms.h>

#if _EWL_THREADSAFE && _EWL_LOCALDATA_AVAILABLE

#include <critical_regions.h>
#include <lconv.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <fltmath.h>
#include <fenv.h>
#include <ewl_thread_local_data.h>

#include <ewl_misra_types.h>

#pragma data_seg_name "reserved_crt_tls"	/* this segment needs to be unshared amongst cores */

static int_t _ewl_main_data_initialized = 0;
static _ThreadLocalData _ewl_tld;

/* This function sets up a structure of thread local data needed by various EWL functions. */
/* When the thread ends it should call _DisposeThreadData. */
/* Returns false on error, true on success. */
static int_t _InitializeThreadData(void)
{
	_ThreadLocalData *tld;

	tld = &_ewl_tld;

	if (tld != NULL) {
		/* All initialization of items in the _ThreadLocalData structure goes here... */

		tld->fp_Round_Mode 		= ROUND_TO_NEAREST_EVEN;
		tld->fp_FLUSH_TO_ZERO 		= 0;
		tld->fp_IEEE_Exceptions 	= IEEE_No_Exception;
		tld->fp_EnableFPExceptions = 0;
		tld->fp_IEEE_Condition_Codes = 0;

		tld->errno = 0;
		tld->random_next = 1;
		tld->strtok_dn = (uchar_t *) "";
		tld->strtok_ds = (uchar_t *) "";
		tld->locale_name = "C";

	#if _EWL_OS_TIME_SUPPORT
		tld->localtime_tm.tm_sec   =  0;
		tld->localtime_tm.tm_min   =  0;
		tld->localtime_tm.tm_hour  =  0;
		tld->localtime_tm.tm_mday  =  1;
		tld->localtime_tm.tm_mon   =  0;
		tld->localtime_tm.tm_year  =  0;
		tld->localtime_tm.tm_wday  =  1;
		tld->localtime_tm.tm_yday  =  0;
		tld->localtime_tm.tm_isdst = -1;

		tld->gmtime_tm.tm_sec   =  0;
		tld->gmtime_tm.tm_min   =  0;
		tld->gmtime_tm.tm_hour  =  0;
		tld->gmtime_tm.tm_mday  =  1;
		tld->gmtime_tm.tm_mon   =  0;
		tld->gmtime_tm.tm_year  =  0;
		tld->gmtime_tm.tm_wday  =  1;
		tld->gmtime_tm.tm_yday  =  0;
		tld->gmtime_tm.tm_isdst = -1;
	#endif /* _EWL_OS_TIME_SUPPORT */

	#if !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
		tld->_current_locale.next_locale = NULL;
		strcpy(tld->_current_locale.locale_name, _current_locale.locale_name);
		tld->_current_locale.coll_cmpt_ptr = _current_locale.coll_cmpt_ptr;
		tld->_current_locale.ctype_cmpt_ptr = _current_locale.ctype_cmpt_ptr;
		tld->_current_locale.mon_cmpt_ptr = _current_locale.mon_cmpt_ptr;
		tld->_current_locale.num_cmpt_ptr = _current_locale.num_cmpt_ptr;
		tld->_current_locale.time_cmpt_ptr = _current_locale.time_cmpt_ptr;
		memcpy((void*) &tld->_current_locale.next_locale, (void*) &_current_locale.next_locale,
			sizeof(struct __locale));

		tld->__lconv = __lconv;
	#endif /* !_EMBEDDED_WARRIOR_HAS_NO_LOCALE */

		tld->__atexit_curr_func		= 0;

		return 1;
	}

	return 1;	/* data is already setup, just return success */
}

int_t _EWL_CDECL __ewl_InitializeMainThreadData(void)
{
    /* Check to see if data was already initialized */
    if (!_ewl_main_data_initialized) {
        /* If it wasn't initialized, setup the data for the main thread */
        if (_InitializeThreadData()) {
        	_ewl_main_data_initialized = 1;
            return 1;
        }
    }

    return 0;
}

void _EWL_CDECL __ewl_DisposeAllThreadData(void)
{
}

_ThreadLocalData * _EWL_CDECL __ewl_GetThreadLocalData(void)
{
	if (!_ewl_main_data_initialized) {
		if (!__ewl_InitializeMainThreadData()) {
			MISRA_EXCEPTION_RULE_20_4()
			exit(127);
		}
	}

	return &_ewl_tld;
}

int_t alloc_context_local_data(void)
{
	return __ewl_InitializeMainThreadData();
}


int_t free_context_local_data(void)
{
	return 1;
}

extern _ThreadLocalData * get_context_local_data (void)
{
	return __ewl_GetThreadLocalData();
}

#endif /* _EWL_THREADSAFE */
