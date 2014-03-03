/* @(#)w_cosh.c 1.2 95/01/04 */
/* $Id: w_cosh.c,v 1.2 2009/12/21 12:33:29 r58171 Exp $ */
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
 * wrapper cosh(x)
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL cosh(f64_t x)		/* wrapper cosh */
#else
	f64_t cosh(x)			/* wrapper cosh */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_cosh(x);
#else
	f64_t z;
	z = __ieee754_cosh(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(fabs(x)>7.10475860073943863426e+02) {
	        return __kernel_standard(x,x,5); /* cosh overflow */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
