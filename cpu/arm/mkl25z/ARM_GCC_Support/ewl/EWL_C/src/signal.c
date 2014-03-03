/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 08:05:47 $
 * $Revision: 1.6 $
 */

/*
 *	Routines
 *	--------
 *		signal
 *		raise
 */

#include <ansi_parms.h>

#include <critical_regions.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

__signal_func_ptr signal_funcs[__ewl_signal_max];

#if __dest_os != __win32_os

__signal_func_ptr signal(int_t sig, __signal_func_ptr signal_func)
{
	__signal_func_ptr	old_signal_func;

	if ((sig < 1) || (sig > __ewl_signal_max)) {
		MISRA_EXCEPTION_RULE_20_5()
		_EWL_LOCALDATA(errno) = ESIGPARM;
		MISRA_EXCEPTION_RULE_11_3()
		MISRA_EXCEPTION_RULE_14_7()
		return (SIG_ERR);
	}

	__begin_critical_region(signal_funcs_access);
	old_signal_func = signal_funcs[sig-1];
	signal_funcs[sig-1] = signal_func;
	__end_critical_region(signal_funcs_access);

	return(old_signal_func);
}

#endif /* __dest_os != __win32_os */

MISRA_EXCEPTION_RULE_16_2()
int_t _EWL_CDECL raise(int_t sig)
{
	__signal_func_ptr	signal_func;

	if ((sig < 1) || (sig > __ewl_signal_max)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	__begin_critical_region(signal_funcs_access);

	signal_func = signal_funcs[sig-1];

	MISRA_EXCEPTION_RULE_11_3()
	if (signal_func != SIG_IGN) {
		signal_funcs[sig-1] = SIG_DFL;
	}

	__end_critical_region(signal_funcs_access);

	MISRA_EXCEPTION_RULE_11_3()
	if ((signal_func == SIG_IGN) || ((signal_func == SIG_DFL) && (sig == SIGABRT))) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	MISRA_EXCEPTION_RULE_13_7()
	if ((signal_func == SIG_DFL) || (signal_func == NULL)) {
		MISRA_EXCEPTION_RULE_20_11()
		exit(0);
	}

	(*signal_func)(sig);

	return(0);
}

