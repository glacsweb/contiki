/* @(#)s_copysign.c 1.2 95/01/04 */
/* $Id: s_copysign.c,v 1.2 2009/12/21 12:33:31 r58171 Exp $ */
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
 * copysign(double x, double y)
 * copysign(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_USES_SUN_MATH_LIB
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL copysign(f64_t x, f64_t y)
#else
	f64_t __copysign(x,y)
	f64_t x,y;
#endif
{
	MISRA_EXCEPTION_RULE_12_7()
	__HI(x) = (__HI(x)&0x7fffffffL)|(__HI(y)&(int32_t)0x80000000L);
    return x;
}
#endif /* _EWL_C99 || _EWL_USES_SUN_MATH_LIB */
#endif /* _EWL_FLOATING_POINT  */
