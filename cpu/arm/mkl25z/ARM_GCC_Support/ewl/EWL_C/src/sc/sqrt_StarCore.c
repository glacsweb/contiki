/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

/*
 *	sqrt returns the square root of its double-precision argument,
 *	using Newton's method.
 *	Returns EDOM error and value 0 if argument negative.
 *	Calls frexp and ldexp.
 */

#ifndef _CW_X_SLLD_

#include <errno.h>
#include <math.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

#define ITERATIONS	8

f64_t
sqrt( f64_t x)
{
	f64_t y;
	int_t iexp; /* can't be in register because of frexp() below */
	int_t i = ITERATIONS;

	if (x <= 0.0) {
		if ( !x ) {
			return (x); /* sqrt(0) == 0 */
		}
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
		return ( 0.0 );
	}

	y = frexp(x, &iexp); /* 0.5 <= y < 1 */

	/* iexp is odd */
	if ( iexp % 2 ) {
		--iexp;
		y += y; /* 1 <= y < 2 */
	}

	y = ldexp(y + 1.0, (iexp/2) - 1); /* first guess for sqrt */

	do {
		y = 0.5 * (y + (x/y));
	} while (--i > 0);

	return (y);
}

#else
/* _CW_X_SLLD_ */
#include <math.h>
#include <errno.h>
#include <dblmath.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif


/*
 *	wrapper sqrt
 */
f64_t sqrt(f64_t x)
{

	/* a domain error occurs if the argument is less than zero */
	if(x < 0.0) {
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
	}
	return _d_sqrt(x);
}

#endif /* _CW_X_SLLD_ */

