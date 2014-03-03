/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/13 08:25:09 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		__get_clock
 *		__get_time
 *
 *		__to_gm_time
 *		__isdst
 */

#include <ansi_parms.h>
#include <time.h>
#include <time_api.h>
#include <errno.h>
#include <syscall.h>

#include <ewl_misra_types.h>

#ifdef __STARCORE__
#pragma far __syscall
#endif

#if _EWL_OS_TIME_SUPPORT

#if	_EWL_CLOCK_T_AVAILABLE
clock_t __get_clock(void)
{
	clock_t clck;

#if CLOCK_T_IS_LONG_LONG
	(void)__syscall(SYSCALL_clock64, &clck);
#else
	(void)__syscall(SYSCALL_clock, &clck);
#endif

	return clck;
}
#endif /* _EWL_CLOCK_T_AVAILABLE */

#ifdef _EWL_TIME_T_AVAILABLE
time_t __get_time(void)
{
	int_t errnum;
	time_t tme;

#if CLOCK_T_IS_LONG_LONG
	errnum = __syscall(SYSCALL_time64, &tme, NULL);
#else
	errnum = __syscall(SYSCALL_time, &tme, NULL);
#endif

	if ( errnum ) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = errnum;
	}

	return tme;
}

int_t __to_gm_time(time_t * tme)
{
	#pragma unused(tme)
	MISRA_QUIET_UNUSED_ARGS()

	/* Possible return values:
		1 = unable to convert time_t *time from local to global mean time
		0 = time_t *time converted properly from local to global mean time
	*/

	return 1;
}

#if !_EWL_TIME_T_IS_LOCALTIME
int __to_local_time(time_t * tme)
{
	#pragma unused(tme)
	MISRA_QUIET_UNUSED_ARGS()
	/* Possible return values:
		1 = unable to convert time_t *time from global mean to local time
		0 = time_t *time converted properly from global mean to local time
	*/

	return 1;
}
#endif /* _EWL_TIME_T_IS_LOCALTIME */

int_t __isdst(void)
{
	/* Possible return values:
		-1 = daylight savings time information not obtainable
		0 = not currently daylight savings time
		1 = daylight savings time is in effect
	*/

	return -1;
}
#endif /* _EWL_TIME_T_AVAILABLE */

#endif /* _EWL_OS_TIME_SUPPORT */
