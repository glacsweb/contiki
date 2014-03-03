/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:10 $
 * $Revision: 1.13 $
 */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <math_double_gcc.c>
#else

#if _EWL_C99 || _EWL_C99_MATH_LEAK
#if _EWL_FLOATING_POINT

#ifndef _INSIDE_X87D_C_

/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 1

#endif	/*_INSIDE_X87D_C_*/

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <limits.h>

#if !(_EWL_USES_SUN_MATH_LIB)
#include "w_lgamma.c"
#include "s_erf.c"
#endif

/*
#ifndef __CWCC__
	#define _EWL_COMPILE_FOR_SPEED 1
#else
	#if __option(optimize_for_size)
		#define _EWL_COMPILE_FOR_SPEED 0
	#else
		#define _EWL_COMPILE_FOR_SPEED 1
	#endif
#endif
*/

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL nexttoward(f64_t x, ldbl_t y) _EWL_CANT_THROW
{
	int_t increase;
	int_t positive;

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* lx = (uint32_t*)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* ly = (uint32_t*)&y;

#if _EWL_LITTLE_ENDIAN
	uint32_t x_exp = lx[1] & 0x7FF00000;
	if ((((ly[1] & 0x7FF00000u) == 0x7FF00000u) && ((ly[1] & 0x000FFFFFu) | ly[0])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f64_t)y;
	}
	if ((x_exp == 0x7FF00000u) && ((lx[1] & 0x000FFFFFu) | lx[0])) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#else
	uint32_t x_exp = lx[0] & 0x7FF00000u;
	MISRA_EXCEPTION_RULE_13_3()
	if ((((ly[0] & 0x7FF00000u) == 0x7FF00000u) && ((ly[0] & 0x000FFFFFu) | ly[1])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f64_t)y;
	}
	if ((x_exp == 0x7FF00000u) && ((lx[0] & 0x000FFFFFu) | lx[1])) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#endif /* _EWL_LITTLE_ENDIAN */

	increase = y > x;
	positive = x > 0.0;

#if _EWL_LONGLONG
	if ((x_exp != 0x7FF00000u) || (increase != positive)) {
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		uint64_t* llx = (uint64_t*)&x;
		if (x == 0.0) {
			if (increase) {
				*llx = 0x0000000000000001ULL;
			} else {
				*llx = 0x8000000000000001ULL;
			}
		} else {
			if (increase == positive) {
				++(*llx);
			} else {
				--(*llx);
			}
		}
	#if _EWL_LITTLE_ENDIAN
		x_exp = lx[1] & 0x7FF00000u;
	#else
		x_exp = lx[0] & 0x7FF00000u;
	#endif /* _EWL_LITTLE_ENDIAN */
		MISRA_EXCEPTION_RULE_13_7()
		if ((x_exp == 0u) && (math_errhandling & MATH_ERREXCEPT)) {
			feraiseexcept((int_t)(FE_UNDERFLOW | FE_INEXACT));
		} else if (x_exp == 0x7FF00000u) {
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno = ERANGE;
			}
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
				feraiseexcept((int_t)(FE_OVERFLOW | FE_INEXACT));
			}
		}
	}
#endif
	return x;
}
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

#if !_EWL_USES_SUN_MATH_LIB


_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL cbrt(f64_t x) _EWL_CANT_THROW
{
	unsigned int sign;
	int_t exponent;
	f64_t y;
/* 		if x is 0 or -0 return x 																	*/
	if (x == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
/* 	if x is +/- inf or nan, return x, setting errors as appropriate									*/
	if (!isfinite(x))
	{
		if (math_errhandling)
		{
			if (isnan(x))
			{
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO)
					errno = EDOM;
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT)
					feraiseexcept((int_t)FE_INVALID);
			}
			else
			{
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO)
					errno = ERANGE;
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT)
					feraiseexcept((int_t)FE_OVERFLOW);
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

/* 	extract biased exponent (x is either normal or denormal											*/
	sign = __UHI(x) & 0x80000000;
	__UHI(x) ^= sign;
	exponent = (int)(__UHI(x) >> 20);
	if (exponent == 0)  /* if x is denormal															*/
	{
		/* normalize x by shifting left until the first 1 bit is in									*/
		/* position 11																				*/
		int c = __ewl_count_leading_zero32(__UHI(x)) - 11;
		if (c == 21)
			c += __ewl_count_leading_zero32(__ULO(x));
		if (c < 32)
		{
			__UHI(x) = (__UHI(x) << c) | (__ULO(x) >> (32 - c));
			__ULO(x) <<= c;
		}
		else
		{
			__UHI(x) = __ULO(x) << (c - 32);
			__ULO(x) = 0;
		}
		/* decrement exponent by the number of leading 0 bits in mantissa							*/
		exponent -= c - 1;
	}

	/* unbias exponent and normalize x to [0.5, 1)													*/
	exponent -= 1022;
	__UHI(x) &= 0x000FFFFF;
	__UHI(x) |= 0x3FE00000;
	/* make exponent evenly divisible by 3, normalizing x to [0.125, 1)								*/
	switch (exponent % 3)
	{
		case -2:
		case 1:
			exponent += 2;
			__UHI(x) -= 0x00200000;
			break;
		case -1:
		case 2:
			exponent += 1;
			__UHI(x) -= 0x00100000;
			break;
	}
	/* estimate first guess to cube root of x [0.125, 1) 											*/
	/* use polynomial derived from minimizing the error  from 0.125 to 1                        	*/
	y = (0.1950252994681516 + (8.93386164028292 +  (34.95109317740758 + 14.90468137136715 * x) * x) * x) /
	    (1.0                + (17.79274627019952 + (34.47634312279896 + 5.715646840256109 * x) * x) * x);
	/* perform newton iterations to improve the estimate											*/
	y = (2 * y + x / (y*y)) / 3;
	/* put result of final estimate back in x														*/
	x = (2 * y + x / (y*y)) / 3;
	/* x is now the cube root of [0.125, 1) -> [0.5, 1)												*/
	/* complete by taking the cube root of the exponent, and loading that							*/
	/* exponent into x																				*/
	exponent /= 3;
	exponent <<= 20;
	__UHI(x) += exponent;
	/* restore original sign of x																	*/
	__UHI(x) |= sign;
	/*x = x * sign; */
	return x;
}

_EWL_IMP_EXP_C int_t _EWL_MATH_CDECL ilogb(f64_t x)
{
#if _EWL_C99
	if ((x==0.0) || (isinf(x)) || (isnan(x)))	{	/* domain error may occur */
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
	}
#endif

	if (x == 0.0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (FP_ILOGB0);
	}
	if (isinf(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (INT_MAX);
	}
	if (isnan(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (FP_ILOGBNAN);
	}
	return ((int)logb(x));
}

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL tgamma(f64_t x) _EWL_CANT_THROW
{
	return((f64_t)tgammal(x));
}

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL nextafter(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return nexttoward(x, (ldbl_t)y);
}
#endif /*_EWL_USES_SUN_MATH_LIB */

#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_C99 */

#endif /* _GCCPORT_ */
