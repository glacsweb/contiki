/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <errno.h>
#include <math.h>
#include <sc/trans.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

#ifdef atan2
#undef atan2
#endif

f64_t atan2 ( f64_t y, f64_t x )
{
    f64_t y2, at;
    int_t neg_y = 0;

    if (( 0.0 == x ) && ( 0.0 == y )) {	/* domain error may occur */
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

    if ( 0.0 > y ) {
        y2 = -y;
        neg_y = 1;
    } else {
        y2 = y;
    }

    MISRA_EXCEPTION_RULE_13_3()
    if (( y2 - ((0.0 > x)? -x : x )) == y2 ) {
        if (neg_y) {
            return -M_PI_2;
        } else {
            return M_PI_2;
        }
    }

    at = atan(y/x);

    if (0.0 < x) {
        return at;
    }

    if (neg_y) {
        return (at - M_PI);
    } else {
        return (at + M_PI);
    }
}
