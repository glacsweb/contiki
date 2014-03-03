#ifdef _CW_X_SLLD_
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
 * wrapper log(x)
 */

#include "math.h"
#include "fdlibm.h"
#include <errno.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

f64_t log(f64_t x)		/* wrapper log */
{
	#if _EWL_C99
		/* __ieee754_log defines the errors */
	#else
		if (x<0.0) {
			MISRA_EXCEPTION_RULE_20_5()
			errno = EDOM;
		}

		if (x == 0.0) {
			MISRA_EXCEPTION_RULE_20_5)
			errno = ERANGE;
		}
	#endif
	return __ieee754_log(x);
}

#else

/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

/*
 *	log returns the natural logarithm of its double-precision argument.
 *	log10 returns the base-10 logarithm of its double-precision argument.
 *	Returns EDOM error and value -HUGE if argument <= 0.
 *	Algorithm and coefficients from Cody and Waite (1980).
 *	Calls frexp.
 */

#include <float.h>
#include <math.h>
#include <sc/trans.h>
#include <errno.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

#define C1	0.693359375
#define C2	-2.121944400546905827679e-4

f64_t
log(f64_t x)
{
	static f64_t p[] = {
		-0.78956112887491257267e0,
		 0.16383943563021534222e2,
		-0.64124943423745581147e2,
	}, q[] = {
		 1.0,
		-0.35667977739034646171e2,
		 0.31203222091924532844e3,
		-0.76949932108494879777e3,
	};
	f64_t y;
	int_t n;
	if ( x < 0.0 ) {
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=EDOM;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				MISRA_EXCEPTION_RULE_20_5()
				feraiseexcept(FE_INVALID);
			}	
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno=EDOM;
		#endif
	    return - HUGE_VAL;
	} 
    
    if ( 0.0 == x ) {
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		#endif
	    return - HUGE_VAL;
	}

	y = 1.0;
	x = frexp(x, &n);

	if (x < M_SQRT1_2) {
		n--;
		y = 0.5;
	}

	x = (x - y)/(x + y);
	x += x;
	y = x * x;
	x += ((x * y) * _POLY2(y, p))/_POLY3(y, q);
	y = (f64_t)n;
	x += y * C2;

	return ( x + (y * C1));
}

#endif

