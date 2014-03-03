/* @(#)w_acos.c 1.2 95/01/04 */
/* $Id: w_acos.c,v 1.2 2009/12/21 12:33:31 r58171 Exp $ */
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
 * wrap_acos(x)
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL acos(f64_t x)		/* wrapper acos */
#else
	f64_t acos(x)			/* wrapper acos */
	f64_t x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_acos(x);
#else
	f64_t z;
	z = __ieee754_acos(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(fabs(x)>1.0) {
	        return __kernel_standard(x,x,1); /* acos(|x|>1) */
	} else
	    return z;
#endif
}
#endif /* _EWL_FLOATING_POINT  */
