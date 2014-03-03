/* @(#)w_sinh.c 1.2 95/01/04 */
/* $Id: w_sinh.c,v 1.2 2009/12/21 12:33:30 r58171 Exp $ */
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
 * wrapper sinh(x)
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL sinh(f64_t x)		/* wrapper sinh */
#else
	f64_t sinh(x)			/* wrapper sinh */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_sinh(x);
#else
	f64_t z;
	z = __ieee754_sinh(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(!isfinite(z)&&isfinite(x)) {
	    return __kernel_standard(x,x,25); /* sinh overflow */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
