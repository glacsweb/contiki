/* @(#)s_rint.c 1.2 95/01/04 */
/* $Id: s_rint.c,v 1.5 2010/05/28 13:28:19 mviisor1 Exp $ */
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
 * rint(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	Inexact flag raised if x not equal to rint(x).
 */

#include <ewl_misra_types.h>
MISRA_QUIET_UNUSED_HEADER()
#include <fdlibm.h>

#if _EWL_FLOATING_POINT
#if !defined(__arm)

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
TWO52[2]={
  4.50359962737049600000e+15, /* 0x43300000, 0x00000000 */
 -4.50359962737049600000e+15, /* 0xC3300000, 0x00000000 */
};

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL rint(f64_t x)
#else
	f64_t rint(x)
	f64_t x;
#endif
{
	int32_t i0,j0,sx;
	uint32_t i,i1;
	f64_t w,t;
	
	i0 = __HI(x);
	sx = 0;
	if ((uint32_t)i0&0x80000000uL) {
		sx = 1;
	}
	i1 = __ULO(x);
	MISRA_EXCEPTION_RULE_10_3()
	j0 = (int32_t)(((uint32_t)i0>>20)&0x7ffuL)-0x3ffL;
	if (j0<20L) {
	    if(j0<0L) {
			if ((((uint32_t)i0&0x7fffffffuL)|i1)==0uL) {
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
			i1 |= ((uint32_t)i0&0x0fffffuL);
			MISRA_EXCEPTION_RULE_10_3()
			i0 = (int32_t)((uint32_t)i0&0xfffe0000uL);
			MISRA_EXCEPTION_RULE_10_3()
			i0 = (int32_t)((uint32_t)i0|(((i1|(uint32_t)-(int32_t)i1)>>12)&0x80000uL));
			__HI(x)=i0;
	        w = TWO52[sx]+x;
	        t = w-TWO52[sx];
	        i0 = __HI(t);
	        __UHI(t) = ((uint32_t)i0&0x7fffffffuL)|((uint32_t)sx<<31);
			MISRA_EXCEPTION_RULE_14_7()
	        return t;
	    } else {
			i = (0x000fffffuL)>>j0;
			if((((uint32_t)i0&i)|i1)==0UL) {	 /* x is integral */
				MISRA_EXCEPTION_RULE_14_7()
				return x;
			}
			i>>=1;
			if((((uint32_t)i0&i)|i1)!=0UL) {
			    if(j0==19L) {
			    	i1 = 0x40000000UL;
			    } else {
					MISRA_EXCEPTION_RULE_10_3()
			    	i0 = (int32_t)(((uint32_t)i0&(~i))|((0x20000uL)>>j0));
			    }
			}
	    }
	} else if (j0>51L) {
	    if(j0==0x400L) {		/* inf or NaN */
			MISRA_EXCEPTION_RULE_14_7()
			return x+x;
		}
	    else {					/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	} else {
	    i = (0xffffffffUL)>>(j0-20L);
	    if((i1&i)==0UL) {		/* x is integral */
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	    i>>=1;
	    if((i1&i)!=0UL) {i1 = (i1&(~i))|((0x40000000UL)>>(j0-20L));}
	}
	__HI(x) = i0;
	__ULO(x) = i1;
	w = TWO52[sx]+x;
	return w-TWO52[sx];
}
#endif /* !defined(__arm) */
#endif /* _EWL_FLOATING_POINT  */
