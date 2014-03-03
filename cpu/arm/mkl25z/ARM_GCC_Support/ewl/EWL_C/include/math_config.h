/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:28 $
 * $Revision: 1.3 $
 */

#ifndef _EWL_MATH_CONFIG_H
#define _EWL_MATH_CONFIG_H

/********************************************************************************
 *	Project...:	Standard C99 math library/ ANSI/ISO C Standard Lib
 *  Author..... Matthew D. Fassiotto
 *	Purpose...:	this header is meant to include platform specific
 *              implementations of standard math functions according to
 *              the target architecture as set by the CodeWarrior compiler.
 *
 *   NOTE: This header gets included automatically by math.h.  It is not a
 *         standalone header meant to be included directly by any application.
 *         We do not support any source file including this header directly.
 *         This header depends on macros and other types defined in math.h
 *         which must be defined before this header is inlcuded.  Therefore
 *         you are guaranteed this will NOT compile when included directly.
 *
 ********************************************************************************/
#include <ewl_t.h>

#if defined(__m56800__)
	#include <dsp/m56800_math.h>
#elif defined(__m56800E__)
	#include <dsp/m56700_math.h>
#elif defined(__arm)
	#include <arm/math.ARM.h>
#elif defined(__POWERPC__)
	#include <pa/math_ppc.h>
#elif defined(__INTEL__)
	#if defined(__CWCC__) && __option(k63d)
		#include <x86/math_k63d.h>
	#else
		#include <x86/math_x87.h>
	#endif
#elif __dest_os ==  __emb_coldfire
	#include <coldfire/math_cf.h>
#elif defined(__STARCORE__)
	# include <sc/math_StarCore.h>
#else
	#error "Unsupported platform"
#endif

#endif /* _EWL_MATH_CONFIG_H */
