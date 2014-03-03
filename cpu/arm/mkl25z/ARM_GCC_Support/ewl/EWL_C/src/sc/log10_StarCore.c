#ifdef _CW_X_SLLD_
/* New version of log10 with double and long long support */
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
 * wrapper log10(X)
 */

#include "math.h"
#include "fdlibm.h"
#include <errno.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

f64_t log10(f64_t x)		/* wrapper log10 */
{
#if _EWL_C99
	/* errors set in __ieee754_log10 */
#else
	if (x<0.0) {
		MISRA_EXCEPTION_RULE_20_5()
        errno = EDOM;
    }

	if (x == 0.0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
    }
#endif	
    return __ieee754_log10(x);
}

#else

#include <float.h>
#include <math.h>
#include <sc/trans.h>
#include <errno.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

f64_t log10(f64_t x)
{
    MISRA_EXCEPTION_RULE_20_5()
	if ( 0.0 < x ) {
	    return log(x) * M_LOG10E;
	} else if ( 0.0 == x ) {	/* range error may occur */
		#if _EWL_C99
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		#else
			errno=ERANGE;
		#endif
	} else {
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
			errno = EDOM;
		#endif
	}

	return - HUGE_VAL;
}

#endif /* _CW_X_SLLD_ */

