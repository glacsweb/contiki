/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/08/04 09:46:57 $
 * $Revision: 1.5 $
 */

/*
 *	sysenv.c
 *
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 *
 */

#include <stdlib.h>
#include <ewl_misra_types.h>
#include <syscall.h>
#include <errno.h>

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
typedef errno_t check_error_t;
#else
typedef int check_error_t;
#endif

char * getenv(const char * name)
{
	#pragma unused(name)
	MISRA_QUIET_UNUSED_ARGS()

	return(NULL);
}

/*
 * If cmdLine is not a null pointer, the system function passes the string 
 * pointed to by cmdLine to the command processor to be executed.
 *   
 * Returns 0 if the argument is not a null pointer, and the system function 
 * does return. Otherwise, it returns 1. 
 */ 
int system(const char* cmdLine)
{
	int retval;
	check_error_t error;
	
	if (cmdLine == NULL)
		return 1;
	
	error = __syscall(SYSCALL_system, &retval, cmdLine);
	if (error > 0) {
		errno = error;
	}
 	
	if (retval == 0) {
		return 0;
	}

	return 1;
}
