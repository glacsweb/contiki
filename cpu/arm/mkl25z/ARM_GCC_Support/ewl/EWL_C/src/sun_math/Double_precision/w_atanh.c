/* @(#)w_atanh.c 1.2 95/01/04 */
/* $Id: w_atanh.c,v 1.2 2009/12/21 12:33:29 r58171 Exp $ */
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
 * wrapper atanh(x)
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL atanh(f64_t x)		/* wrapper atanh */
#else
	f64_t atanh(x)			/* wrapper atanh */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_atanh(x);
#else
	f64_t z,y;
	z = __ieee754_atanh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	y = fabs(x);
	if(y>=1.0) {
	    if(y>1.0)
	        return __kernel_standard(x,x,30); /* atanh(|x|>1) */
	    else
	        return __kernel_standard(x,x,31); /* atanh(|x|==1) */
	} else
	    return z;
#endif
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
