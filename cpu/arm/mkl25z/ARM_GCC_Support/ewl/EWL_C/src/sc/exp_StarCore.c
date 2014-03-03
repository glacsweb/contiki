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
 * wrapper exp(x)
 */

#include <math.h>
#include <fdlibm.h>
#include <sc/trans.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

f64_t exp(f64_t x)		/* wrapper exp */
{
#ifdef _IEEE_LIBM
#if _EWL_C99
	/* errors defined in __ieee754_exp */
#else
	if (x < LN_MINDOUBLE) {
        MISRA_EXCEPTION_RULE_20_5()
        errno = ERANGE;
    }

	if (x > LN_MAXDOUBLE) {
        MISRA_EXCEPTION_RULE_20_5()
        errno = ERANGE;
    }
#endif
	return __ieee754_exp(x);
#else
/*
	f64_t z;
	z = __ieee754_exp(x);
	if(_LIB_VERSION == _IEEE_) return z;
	if(finite(x)) {
	    if(x>o_threshold)
	        return __kernel_standard(x,x,6); // exp overflow
	    else if(x<u_threshold)
	        return __kernel_standard(x,x,7); // exp underflow
	}
	return z;
*/
#endif
}

#else
/* Old version of exp */

/*changed */

/*
 *	exp returns the exponential function of its double-precision argument.
 *	Returns ERANGE error and value 0 if argument too small,
 *	   value DBL_MAX if argument too large.
 *	Algorithm and coefficients from Cody and Waite (1980).
 *	Calls ldexp.
 */

#include <float.h>
#include <math.h>
#include <sc/trans.h>
#include <errno.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

#define _REDUCE(TYPE, X, XN, C1, C2)    { \
	f64_t x1 = (f64_t)(TYPE)X, x2 = X - x1; \
	X = x1 - ((XN) * (C1)); X += x2; X -= ((XN) * (C2)); }

f64_t exp(f64_t x)
{
    static f64_t p[] =
    {
	0.31555192765684646356e-4,
	0.75753180159422776666e-2,
	0.25000000000000000000e0,
    }, q[] =
    {
	0.75104028399870046114e-6,
	0.63121894374398503557e-3,
	0.56817302698551221787e-1,
	0.50000000000000000000e0,
    };
    register f64_t y, x_orig;
    register int_t n;

    x_orig = x;

    if (x < 0.0) {
        x = -x;
    }

    /* use first term of Taylor series expansion */
    if (x < X_EPS) {
        return (1.0 + x_orig );
    }

    if ( x_orig <= LN_MINDOUBLE ) {
        MISRA_EXCEPTION_RULE_13_3()
        if ( x_orig == LN_MINDOUBLE ) { /* protect against roundoff */
            return ( DBL_MIN ); /* causing ldexp to underflow */
        }

		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
		#endif		
		return ( 0.0 );
    }

    if ( x_orig >= LN_MAXDOUBLE ) {
        MISRA_EXCEPTION_RULE_13_3()
        if ( x_orig == LN_MAXDOUBLE ) { /* protect against roundoff */
            return ( DBL_MAX ); /* causing ldexp to overflow */
        }

		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
		#endif		
		return ( HUGE_VAL );
    }

    MISRA_EXCEPTION_RULE_10_4()
    n = (int_t)((x * M_LOG2E) + 0.5);
    y = (f64_t)n;

    _REDUCE(int_t, x, y, 0.693359375, -2.1219444005469058277e-4)

    if (x_orig < 0.0) {
        x = -x;
        n = -n;
    }

    y = x * x;
    x *= _POLY2(y, p);
    return (ldexp(0.5 + (x/(_POLY3(y, q) - x)), n + 1 ));
}

#endif
