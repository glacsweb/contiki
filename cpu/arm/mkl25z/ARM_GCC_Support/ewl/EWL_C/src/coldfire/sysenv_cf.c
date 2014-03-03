/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/13 08:25:11 $
 * $Revision: 1.3 $
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


#include <ansi_parms.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

char_t * getenv(const char_t * name)
{
	#pragma unused(name)
	MISRA_QUIET_UNUSED_ARGS()

	return(NULL);
}

int_t system(const char_t* cmdLine)
{
	#pragma unused(cmdLine)
	MISRA_QUIET_UNUSED_ARGS()

	return(NULL);
}
