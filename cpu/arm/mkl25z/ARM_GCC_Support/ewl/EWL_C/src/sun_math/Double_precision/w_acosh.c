
/* @(#)w_acosh.c 1.2 95/01/04 */
/* $Id: w_acosh.c,v 1.2 2009/12/21 12:33:29 r58171 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/*
 * wrapper acosh(x)
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL acosh(f64_t x)		/* wrapper acosh */
#else
	f64_t acosh(x)			/* wrapper acosh */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acosh(x);
#else
	f64_t z;
	z = __ieee754_acosh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<1.0) {
	        return __kernel_standard(x,x,29); /* acosh(x<1) */
	} else
	    return z;
#endif
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
