#ifdef _CW_X_SLLD_
/* New version of pow with double and long long support */
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
 * wrapper pow(x,y) return x**y
 */

#include <math.h>
#include <fdlibm.h>
#include <fenv.h>

f64_t pow(f64_t x, f64_t y)	/* wrapper pow */
{
#ifdef _IEEE_LIBM
	return  __ieee754_pow(x,y);
#else
/*
	double z;
	z=__ieee754_pow(x,y);
	if(_LIB_VERSION == _IEEE_|| isnan(y)) return z;
	if(isnan(x)) {
	    if(y==0.0)
	        return __kernel_standard(x,y,42);
	    else
		return z;
	}
	if(x==0.0){
	    if(y==0.0)
	        return __kernel_standard(x,y,20);
	    if(finite(y)&&y<0.0)
	        return __kernel_standard(x,y,23);
	    return z;
	}
	if(!finite(z)) {
	    if(finite(x)&&finite(y)) {
	        if(isnan(z))
	            return __kernel_standard(x,y,24);
	        else
	            return __kernel_standard(x,y,21);
	    }
	}
	if(z==0.0&&finite(x)&&finite(y))
	    return __kernel_standard(x,y,22);
	return z;
*/
#endif
}

#else
/* Old version of pow with no double and long long support */
/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

/*
 *	pow(x, y) returns x ** y.
 *	Returns EDOM error and value 0 for 0 to a non-positive power
 *	or negative to a non-integral power;
 *	ERANGE error and value HUGE or -HUGE when the correct value
 *	would overflow, or 0 when the correct value would underflow.
 *	uses log and exp
 *	This version accepts negative x and integral y,
 *	apparently in violation of the ANSI FORTRAN standard for x ** y.
 *	There is a much more accurate but much more elaborate algorithm
 *	in Cody and Waite, which should be substituted for this.
 */

#include <float.h>
#include <math.h>
#include <sc/trans.h>
#include <errno.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

f64_t
pow( f64_t x, f64_t y)
{
    int_t neg;
    f64_t dummy;

    /* checking for domain errors. */
    MISRA_EXCEPTION_RULE_12_4()
    if (((0.0 > x ) && (0.0 != modf(y, &dummy))) ||
        ((0.0 == x) && (0.0 >= y))) {
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
			errno = EDOM;
		#endif
    	return 0.0;
    }
	
	#if _EWL_C99
		if (0.0 == y && 0.0 == x) {	/* domain error may occur */
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
		}
	#endif

    /* easy special cases */
    if (0.0 == y) {
    	return(1.0);
    } else { if (1.0 == y) {
    	return (x);
    }}

    if (0.0 == x) {
    	return(0.0);
    }

    neg = 0;

    /* test using integer arithmetic if possible */
    if (x < 0.0) {
        if ((y >= (f64_t)-MAXLONG) && (y <= (f64_t)MAXLONG)) {
            int32_t ly = (int32_t)y;

            if ((f64_t)ly != y) {
                /* y not integral */
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

            neg = (int_t)ly % 2;
        } else {
            f64_t fr, dum;

            if ((fr = modf(0.5 * y, &dum))!= 0.0) {
                if ( fr != 0.5 ) {
                    /* y not integral */
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

                neg++; /* y is odd */
            }
        }
        x = -x;
    }

    if (neg < 0) {
        neg = -neg;
    }

    /* x isn't the final result */
    if (x != 1.0) {
        /* the following code protects against multiplying x and y
         * until there is no chance of multiplicative overflow */

        /* preserve sign of product */
        if ((x = log(x)) < 0.0) {
            x = -x;
            y = -y;
        }

        if (y > (LN_MAXDOUBLE/x)) {
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
            return ( ( neg == 1 ) ? -HUGE : HUGE );
        }

        if (y < (LN_MINDOUBLE/x)) {
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

        x = exp(x * y); /* finally; no mishap can occur */
    }

    return ( ( neg == 1 ) ? -x : x);

}

#endif /* _CW_X_SLLD_ */
