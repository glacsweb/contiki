/* @(#)w_remainderf.c 1.2 95/01/04 */
/* $Id: w_remainderf.c,v 1.2 2010/05/05 18:56:02 rat031 Exp $ */
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
/* w_remainderf.c -- float version of w_remainder.c.
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

#if !defined(__arm)

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: w_remainderf.c,v 1.4 1997/10/09 11:35:56 lukem Exp $");
#endif

/*
 * wrapper remainderf(x,p)
 */


#ifdef __STDC__
	f32_t _EWL_MATH_CDECL remainderf(f32_t x, f32_t y)	/* wrapper remainder */
#else
	f32_t remainderf(x,y)			/* wrapper remainder */
	f32_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_remainderf(x,y);
#else
	f32_t z;
	z = __ieee754_remainderf(x,y);
	if(_LIB_VERSION == _IEEE_ || isnanf(y)) {return z;}
	if(y==0.0F) {
	    /* remainder(x,0) */
	    return (f32_t)__kernel_standard((f64_t)x,(f64_t)y,128);
	} else {
	    return z;
	}
#endif
}
#endif /* _EWL_C99 */
#endif /*!defined(__arm) */
#endif /* _EWL_FLOATING_POINT  */
