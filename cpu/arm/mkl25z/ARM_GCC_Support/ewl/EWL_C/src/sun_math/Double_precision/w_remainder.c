/* @(#)w_remainder.c 1.2 95/01/04 */
/* $Id: w_remainder.c,v 1.4 2010/05/28 13:28:19 mviisor1 Exp $ */
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
 * wrapper remainder(x,p)
 */

#include <ewl_misra_types.h>
MISRA_QUIET_UNUSED_HEADER()
#include <fdlibm.h>

#if _EWL_FLOATING_POINT
#if !defined(__arm)

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL remainder(f64_t x, f64_t y)	/* wrapper remainder */
#else
	f64_t remainder(x,y)			/* wrapper remainder */
	f64_t x,y;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_remainder(x,y);
#else
	f64_t z;
	z = __ieee754_remainder(x,y);
	if(_LIB_VERSION == _IEEE_ || isnan(y)) return z;
	if(y==0.0)
	    return __kernel_standard(x,y,28); /* remainder(x,0) */
	else
	    return z;
#endif
}
#endif /* _EWL_C99 */
#endif /*!defined(__arm) */
#endif /* _EWL_FLOATING_POINT  */
