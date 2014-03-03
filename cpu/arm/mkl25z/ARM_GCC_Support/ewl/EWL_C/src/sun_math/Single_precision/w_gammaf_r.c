#if defined(__CWCC__)
#include <math.h>
MISRA_QUIET_UNUSED_HEADER()
#include <fdlibm.h>
#else
#include <sys/cdefs.h>
#include "math.h"
#include "math_private.h"
#endif

#if _EWL_FLOATING_POINT
/* w_gammaf_r.c -- float version of w_gamma_r.c.
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

#ifdef _REENTRANT

#if defined(LIBM_SCCS) && !defined(lint)
__RCSID("$NetBSD: w_gammaf_r.c,v 1.5 1997/10/09 11:34:57 lukem Exp $");
#endif

/*
 * wrapper f32_t gammaf_r(f32_t x, int_t *signgamp)
 */


f32_t gammaf_r __P((f32_t, int_t *));


#ifdef __STDC__
	f32_t gammaf_r(f32_t x, int_t *signgamp) /* wrapper lgammaf_r */
#else
	f32_t gammaf_r(x,signgamp)              /* wrapper lgammaf_r */
        f32_t x; int_t *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,signgamp);
#else
        f32_t y;
        y = __ieee754_lgammaf_r(x,signgamp);
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
#endif /* _REENTRANT */
#endif /* _EWL_FLOATING_POINT  */
