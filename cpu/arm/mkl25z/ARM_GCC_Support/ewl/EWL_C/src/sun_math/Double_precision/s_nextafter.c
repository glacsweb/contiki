/* @(#)s_nextafter.c 1.2 95/01/04 */
/* $Id: s_nextafter.c,v 1.4 2010/05/12 07:07:17 anczane1 Exp $ */
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

/* IEEE functions
 *	nextafter(x,y)
 *	return the next machine floating-point number of x in the
 *	direction toward y.
 *   Special cases:
 */

#include <fdlibm.h>

#if _EWL_C99 || _EWL_C99_MATH_LEAK
#if _EWL_FLOATING_POINT

MISRA_EXCEPTION_RULE_13_3()

#ifdef __STDC__
	f64_t _EWL_MATH_CDECL nextafter(f64_t x, f64_t y)
#else
	f64_t nextafter(x,y)
	f64_t x,y;
#endif
{
	int32_t	hx,hy;
	uint32_t lx,ly,ix,iy;

	hx = __HI(x);		/* high word of x */
	lx = __ULO(x);		/* low  word of x */
	hy = __HI(y);		/* high word of y */
	ly = __ULO(y);		/* low  word of y */
	ix = (uint32_t)hx&0x7fffffffuL;		/* |x| */
	iy = (uint32_t)hy&0x7fffffffuL;		/* |y| */

	if(((ix>=0x7ff00000uL)&&(((ix-0x7ff00000uL)|lx)!=0uL)) ||   /* x is nan */
	   ((iy>=0x7ff00000uL)&&(((iy-0x7ff00000uL)|ly)!=0uL))) {     /* y is nan */
		MISRA_EXCEPTION_RULE_14_7()
	   return x+y;
	}
	if(x==y) {					/* x=y, return x */
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if((ix|lx)==0uL) {			/* x == 0 */
	    __UHI(x) = (uint32_t)hy&0x80000000uL;	/* return +-minsubnormal */
	    __LO(x) = 1L;
	    y = x*x;
	    if (y==x) {
			MISRA_EXCEPTION_RULE_14_7()
	    	return y;
	    } else {
			MISRA_EXCEPTION_RULE_14_7()
	    	return x;	/* raise underflow flag */
	    }
	}
	if(hx>=0L) {				/* x > 0 */
	    if((hx>hy)||((hx==hy)&&(lx>ly))) {	/* x > y, x -= ulp */
			if(lx==0UL) {hx -= 1L;}
			lx -= 1UL;
	    } else {				/* x < y, x += ulp */
			lx += 1UL;
			MISRA_QUIET_LINT_06()
			if(lx==0UL) {hx += 1L;}
	    }
	} else {				/* x < 0 */
	    if((hy>=0L)||(hx>hy)||((hx==hy)&&(lx>ly))){/* x < y, x -= ulp */
			if(lx==0UL) {hx -= 1L;}
			lx -= 1UL;
	    } else {				/* x > y, x += ulp */
			lx += 1UL;
			MISRA_QUIET_LINT_06()
			if(lx==0UL) {hx += 1L;}
	    }
	}
	MISRA_EXCEPTION_RULE_10_3()
	hy = (int32_t)((uint32_t)hx&0x7ff00000uL);
	if(hy>=0x7ff00000L) {		/* overflow  */
		MISRA_EXCEPTION_RULE_14_7()
		return x+x;
	}
	if(hy<0x00100000L) {		/* underflow */
	    y = x*x;
	    if (y!=x) {		/* raise underflow flag */
			__HI(y) = hx;
			__ULO(y) = lx;
			MISRA_EXCEPTION_RULE_14_7()
			return y;
	    }
	}
	__HI(x) = hx;
	__ULO(x) = lx;
	return x;
}
#endif /* _EWL_FLOATING_POINT  */
#endif /* _EWL_C99 */
