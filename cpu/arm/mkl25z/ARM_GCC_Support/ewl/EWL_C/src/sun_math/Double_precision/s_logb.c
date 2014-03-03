/* @(#)s_logb.c 1.2 95/01/04 */
/* $Id: s_logb.c,v 1.6 2010/05/12 07:07:16 anczane1 Exp $ */
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
 * double logb(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#include <fdlibm.h>
#include <fenv.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL logb(f64_t x)
#else
	f64_t logb(x)
	f64_t x;
#endif
{
	uint32_t lx, ix;
	ix = (__UHI(x))&0x7fffffffuL;	/* high |x| */
	lx = __ULO(x);			/* low x */
	if((ix|lx)==0uL) {		/* domain or range error may occur */
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=EDOM;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				MISRA_EXCEPTION_RULE_20_5()
				feraiseexcept((int_t)FE_INVALID);
			}		
		#endif
		MISRA_EXCEPTION_RULE_14_7()
		return -1.0/fabs(x);
	}
	if(ix>=0x7ff00000uL) {
		MISRA_EXCEPTION_RULE_14_7()
		return x*x;
	}
	if((ix>>=20)==0uL) {			/* IEEE 754 logb */
		MISRA_EXCEPTION_RULE_14_7()
		return -1022.0;
	} else {
		int32_t res = (int32_t)ix-1023L;
		MISRA_EXCEPTION_RULE_14_7()
		return (f64_t) res;
	}
}
#endif /* _EWL_FLOATING_POINT  */
#endif /* _EWL_C99 */
