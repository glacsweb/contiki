#if defined(__CWCC__)
#include <math.h>
#include <fdlibm.h>
#else
#include <sys/cdefs.h>
#include "math.h"
#include "math_private.h"
#endif

#if _EWL_FLOATING_POINT
/* w_gammaf.c -- float version of w_gamma.c.
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
__RCSID("$NetBSD: w_gammaf.c,v 1.5 1997/10/09 11:34:54 lukem Exp $");
#endif

#ifdef __STDC__
	f32_t gammaf(f32_t x)
#else
	f32_t gammaf(x)
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
	        /* gammaf pole */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,141);
            else
	        /* gammaf overflow */
                return (f32_t)__kernel_standard((f64_t)x,(f64_t)x,140);
        } else
            return y;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
