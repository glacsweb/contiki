/* @(#)w_gamma_r.c 1.2 95/01/04 */
/* $Id: w_gamma_r.c,v 1.3 2010/04/14 13:43:49 r58171 Exp $ */
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

/*
 * wrapper double gamma_r(double x, int *signgamp)
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT
#ifdef _REENTRANT
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL gamma_r(f64_t x, int32_t *signgamp) /* wrapper lgamma_r */
#else
	f64_t gamma_r(x,signgamp)              /* wrapper lgamma_r */
        f64_t x; int32_t *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_gamma_r(x,signgamp);
#else
    f64_t y;
    y = __ieee754_gamma_r(x,signgamp);
    if(_LIB_VERSION == _IEEE_) {return y;}
    if(!isfinite(y)&& isfinite(x)) {
        if(floor(x)==x&&x<=0.0) {
            return __kernel_standard(x,x,41); /* gamma pole */
        } else {
            return __kernel_standard(x,x,40); /* gamma overflow */
    	}
    } else {
        return y;
    }
#endif
}
#else
MISRA_QUIET_UNUSED_HEADER()
#endif /* _REENTRANT */
#endif /* _EWL_FLOATING_POINT  */
