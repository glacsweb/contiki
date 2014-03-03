/* @(#)s_frexp.c 1.3 95/01/04 */
/* $Id: s_frexp.c,v 1.3 2010/05/12 07:07:16 anczane1 Exp $ */
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
 * for non-zero x
 *	x = frexp(arg,&exp);
 * return a double fp quantity x such that 0.5 <= |x| <1.0
 * and the corresponding binary exponent "exp". That is
 *	arg = x*2^exp.
 * If arg is inf, 0.0, or NaN, then frexp(arg,&exp) returns arg
 * with *exp=0.
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT

#ifdef __STDC__
static const f64_t
#else
static f64_t
#endif
two54 =  1.80143985094819840000e+16; /* 0x43500000, 0x00000000 */
#ifdef __STDC__
	f64_t _EWL_MATH_CDECL frexp(f64_t x, int_t *eptr)
#else
	f64_t frexp(x, eptr)
	f64_t x; int *eptr;
#endif
{
	int32_t  hx;
	uint32_t ix;

	hx = __HI(x);
	ix = 0x7fffffffuL&(uint32_t)hx;
	*eptr = 0;
	if((ix>=0x7ff00000uL)||((ix|__ULO(x))==0uL)) {		/* 0,inf,nan */
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (ix<0x00100000uL) {		/* subnormal */
	    x *= two54;
	    hx = __HI(x);
	    ix = (uint32_t)hx&0x7fffffffuL;
	    *eptr = -54;
	}
	MISRA_EXCEPTION_RULE_10_3()
	*eptr += (int_t)((ix>>20)-1022uL);
	ix = ((uint32_t)hx&0x800fffffuL)|0x3fe00000uL;
	__UHI(x) = ix;
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
