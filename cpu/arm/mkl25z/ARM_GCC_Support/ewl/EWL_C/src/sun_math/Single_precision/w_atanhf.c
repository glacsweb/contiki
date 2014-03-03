#if defined(__CWCC__)
#include <math.h>
#include <fdlibm.h>
#else
#include <sys/cdefs.h>
#include "math.h"
#include "math_private.h"
#endif

#if _EWL_FLOATING_POINT
/* w_atanhf.c -- float version of w_atanh.c.
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

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: w_atanhf.c,v 1.4 1997/10/09 11:34:16 lukem Exp $");
#endif

/*
 * wrapper atanhf(x)
 */



#ifdef __STDC__
	f32_t atanhf(f32_t x)		/* wrapper atanhf */
#else
	f32_t atanhf(x)			/* wrapper atanhf */
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atanhf(x);
#else
	f32_t z,y;
	z = __ieee754_atanhf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	y = fabsf(x);
	if(y>=1.0F) {
	    if(y>1.0F)
	        /* atanhf(|x|>1) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,130);
	    else
	        /* atanhf(|x|==1) */
	        return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,131);
	} else
	    return z;
#endif
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
