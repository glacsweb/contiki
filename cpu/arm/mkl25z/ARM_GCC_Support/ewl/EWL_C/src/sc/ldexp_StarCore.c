/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <math.h>
#include <float.h>
#include <errno.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

/*************************************************************************
 Function Name:  ldexp
 Description:    The inverse of frexp. It computes the value x*(2**n)
                 A range error may occur.
 Parameters:     x - fractional value
                 exp - an interger representing the exponent.
**************************************************************************/

#define TWO  2.0

f64_t ldexp(f64_t x, int_t xp)
{
  while ( xp > 0 ) {
    if ( x > (DBL_MAX/TWO)) {
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
      return HUGE_VAL;
    }

    x = x*TWO;
    xp--;
  }

  while ( xp < 0 ) {
    x = x/TWO;
    xp++;
  }
  return x;
}

