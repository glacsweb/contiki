/* @(#)w_sinhf.c 1.2 95/01/04 */
/* $Id: w_sinhf.c,v 1.1 2010/05/03 19:03:56 rat031 Exp $ */
#if defined(__CWCC__)
#include <math.h>
#include <fenv.h>
#include <fdlibm.h>
#else
#include <sys/cdefs.h>
#include "math.h"
#include "math_private.h"
#include <fenv.h>
#endif

#if _EWL_FLOATING_POINT
/* w_sinhf.c -- float version of w_sinh.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: w_sinhf.c,v 1.4 1997/10/09 11:36:09 lukem Exp $");
#endif

/*
 * wrapper sinhf(x)
 */


#ifdef __STDC__
	f32_t _EWL_MATH_CDECL sinhf(f32_t x)		/* wrapper sinhf */
#else
	f32_t sinhf(x)			/* wrapper sinhf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sinhf(x);
#else
	f32_t z;
	z = __ieee754_sinhf(x);
	if(_LIB_VERSION == _IEEE_) {return z;}
	if(!finitef(z)&&finitef(x)) {
	    /* sinhf overflow */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,125);
	} else {
	    return z;
	}
#endif
}
#endif /* _EWL_FLOATING_POINT  */
