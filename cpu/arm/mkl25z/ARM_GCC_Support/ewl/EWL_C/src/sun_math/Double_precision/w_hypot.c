/* @(#)w_hypot.c 1.2 95/01/04 */
/* $Id: w_hypot.c,v 1.3 2010/03/05 09:00:39 anczane1 Exp $ */
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
 * wrapper hypot(x,y)
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL hypot(f64_t x, f64_t y)/* wrapper hypot */
#else
	f64_t hypot(x,y)		/* wrapper hypot */
	f64_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	/* range error may occur; not specified */
	return __ieee754_hypot(x,y);
#else
	f64_t z;
	z = __ieee754_hypot(x,y);
	if(_LIB_VERSION == _IEEE_) return z;
	if((!isfinite(z))&&isfinite(x)&&isfinite(y))
	    return __kernel_standard(x,y,4); /* hypot overflow */
	else
	    return z;
#endif
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
