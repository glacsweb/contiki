/* @(#)w_log10.c 1.2 95/01/04 */
/* $Id: w_log10.c,v 1.2 2009/12/21 12:33:30 r58171 Exp $ */
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
 * wrapper log10(X)
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL log10(f64_t x)		/* wrapper log10 */
#else
	f64_t log10(x)			/* wrapper log10 */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_log10(x);
#else
	f64_t z;
	z = __ieee754_log10(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<=0.0) {
	    if(x==0.0)
	        return __kernel_standard(x,x,18); /* log10(0) */
	    else
	        return __kernel_standard(x,x,19); /* log10(x<0) */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
