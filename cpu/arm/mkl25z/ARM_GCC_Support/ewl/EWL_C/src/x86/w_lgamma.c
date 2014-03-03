/* @(#)w_lgamma.c 1.2 95/01/04 */
/* $Id: w_lgamma.c,v 1.3 2010/01/29 13:41:59 r58171 Exp $ */
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

/* double lgamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __ieee754_lgamma_r
 */

#include <ansi_parms.h>
#include "e_lgamma_r.c"

#ifndef _INSIDE_X87D_C_

#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 1
#undef	_EWL_INLINE
#define	_EWL_INLINE _EWL_DO_NOT_INLINE

#endif	/*_INSIDE_X87D_C_*/

#include <math.h>

#if _EWL_C99
/*#if _EWL_FLOATING_POINT*/
/*extern*/ _INT32 signgam;

#ifdef __STDC__
	double lgamma(double x)
#else
	double lgamma(x)
	double x;
#endif
{
/*#ifdef _IEEE_LIBM */
	return __ieee754_lgamma_r(x,&signgam);
#if 0
/*#else */
        double y;
        y = __ieee754_lgamma_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!isfinite(y)&&isfinite(x)) {
            if(floor(x)==x&&x<=0.0)
                return __kernel_standard(x,x,15); /* lgamma pole */
            else
                return __kernel_standard(x,x,14); /* lgamma overflow */
        } else
            return y;
/*#endif*/
#endif /* if 0 */
}
#endif /* _EWL_C99 */           
/*#endif /* _EWL_FLOATING_POINT  */
