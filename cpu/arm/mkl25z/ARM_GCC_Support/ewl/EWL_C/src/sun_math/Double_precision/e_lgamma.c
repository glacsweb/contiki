/* @(#)e_lgamma.c 1.2 95/01/04 */
/* $Id: e_lgamma.c,v 1.2 2009/12/21 12:33:32 r58171 Exp $ */
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

/* __ieee754_lgamma(x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __ieee754_lgamma_r
 */

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL __ieee754_lgamma(f64_t x)
#else
	f64_t __ieee754_lgamma(x)
	f64_t x;
#endif
{
	return __ieee754_lgamma_r(x,&signgam);
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT  */
