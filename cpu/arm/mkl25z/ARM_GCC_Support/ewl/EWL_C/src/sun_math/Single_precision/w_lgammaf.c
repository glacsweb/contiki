#if defined(__CWCC__)
#include <math.h>
#include <fdlibm.h>
#else
#include <sys/cdefs.h>
#include "math.h"
#include "math_private.h"
#endif

#if _EWL_FLOATING_POINT
/* w_lgammaf.c -- float version of w_lgamma.c.
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
__RCSID("$NetBSD: w_lgammaf.c,v 1.4 1997/10/09 11:35:31 lukem Exp $");
#endif

#ifdef __STDC__
	f32_t lgammaf(f32_t x)
#else
	f32_t lgammaf(x)
	f32_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,&signgam);
#else
        f32_t y;
        y = __ieee754_lgammaf_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=0.0F)
	        /* lgamma pole */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,115);
            else
	        /* lgamma overflow */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,114);
        } else
            return y;
#endif
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
