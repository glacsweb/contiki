/*  MSL  */

/*  $Date: 2010/04/13 08:25:13 $ 
 *  $Revision: 1.3 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-2007 Freescale Corporation.
 *		All rights reserved.
 */
 
/*
 *	sysenv.ppceabi.c
 *	
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 */

#include <cstdlib>				/* for getenv */

#include <ewl_misra_types.h>

char_t * getenv(const char_t * inVarName)
{
	#pragma unused(inVarName)
	MISRA_QUIET_UNUSED_ARGS()
	return 0;
}

int_t system(const char_t * cmdLine)
{
	#pragma unused(cmdLine)
	MISRA_QUIET_UNUSED_ARGS()
	return 0;	
}
