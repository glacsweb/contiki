/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <math.h>
#include <errno.h>
#include <sc/trans.h>
#include <fenv.h>

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

/* algorithm and coefficients from cody and waite ( 1980 ). */

static f64_t
asin_acos( f64_t x, int_t acosflag)
{
    f64_t y;
    int_t neg = 0, large = 0;

    if (x < 0.0) {
        x = -x;
        neg++;
    }

    if (x > 1.0) {
        return ( 0.0 );
    }

   /* skip for efficiency and to prevent underflow */
    if ( x > X_EPS ) {
#ifdef _CW_X_SLLD_
        static f64_t p[5], q[6];
        p[0] = LL_to_Db((int64_t)0xbfe64bbdb5e61e65LL);
        p[1] = LL_to_Db((int64_t)0x40244e1764ec3927LL);
        p[2] = LL_to_Db((int64_t)0xc043d82ca9a6da9fLL);
        p[3] = LL_to_Db((int64_t)0x404c9aa7360ad48aLL);
        p[4] = LL_to_Db((int64_t)0xc03b5e55a83a0a62LL);

        q[0] = LL_to_Db((int64_t)0x3ff0000000000000LL);
        q[1] = LL_to_Db((int64_t)0xc037d2e86ef9861fLL);
        q[2] = LL_to_Db((int64_t)0x4062de7c96591c70LL);
        q[3] = LL_to_Db((int64_t)0xc077ddcefc56a848LL);
        q[4] = LL_to_Db((int64_t)0x407a124f101eb843LL);
        q[5] = LL_to_Db((int64_t)0xc06486c03e2b87ccLL);
#else
        static f64_t
        p[] = { -0.69674573447350646411e0,
              0.10152522233806463645e2,
             -0.39688862997504877339e2,
              0.57208227877891731407e2,
             -0.27368494524164255994e2,
              },
        q[] = {  1.0,
             -0.23823859153670238830e2,
              0.15095270841030604719e3,
             -0.38186303361750149284e3,
              0.41714430248260412556e3,
             -0.16421096714498560795e3,
              };
#endif

        if ( x <= 0.5 ) {
            y = x * x;
        } else {
            large++;
            y = 0.5 - (0.5 * x);
            x = -sqrt(y);
            x += x;
        }
        x += ((x * y) * _POLY4(y, p)) / _POLY5(y, q);
    }

    if (acosflag) {
        if (neg == 0) {
            x = -x;
        }
        if (large == 0) {
            return M_PI_2 + x;
        } else if (neg) {
            return M_PI + x;
        } else {
            return x;
        }
    }

    if (large) {
        x += M_PI_2;
    }

    if (neg) {
        return -x;
    } else {
        return x;
    }
}


f64_t
asin( f64_t x )
{
    if (( 1.0 < x ) || ( -1.0 > x )) {
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
    return (asin_acos(x, 0));
}


f64_t
acos( f64_t x )
{
    if (( 1.0 < x ) || ( -1.0 > x )) {
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
    return (asin_acos(x, 1));
}
