/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/05/25 16:47:43 $
 * $Revision: 1.16 $
 */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <math_float_gcc.c>
#else

#if _EWL_C99 || _EWL_C99_MATH_LEAK
#if _EWL_FLOATING_POINT

#ifndef _INSIDE_X87D_C_

/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 1
/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef	_EWL_INLINE
#define	_EWL_INLINE _EWL_DO_NOT_INLINE

#endif	/*_INSIDE_X87D_C_*/

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <limits.h>

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

#if !_EWL_USES_SUN_SP_MATH_LIB
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL cbrtf(f32_t x) _EWL_CANT_THROW
{
	f64_t     xd;
	uint32_t *ix;
	uint32_t  sign;
	int_t     exponent;

	typedef union {
		f32_t f32;
		uint32_t ui32;
	} u32_t;
	u32_t ux;
	
	if (x == 0.0f) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	ix = (uint32_t *)&x;

/* 	if x is +/- inf or nan, return x, setting errors as appropriate									*/
	if (!isfinite(x)) {
		MISRA_EXCEPTION_RULE_13_7()
		if (math_errhandling) {
			if (isnan(x)) {
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno = EDOM;
				}
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					feraiseexcept((int_t)FE_INVALID);
				}
			} else {
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
					MISRA_EXCEPTION_RULE_20_5()
					errno = ERANGE;
				}
				MISRA_EXCEPTION_RULE_13_7()
				if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
					feraiseexcept((int_t)FE_OVERFLOW);
				}
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	ux.f32 = x;
	sign = ux.ui32 & 0x80000000;
	ux.ui32 ^= sign;
	MISRA_EXCEPTION_RULE_10_3()
	exponent = (int_t)(ux.ui32 >> 23);
	if (exponent == 0) {
		uint32_t c = __ewl_count_leading_zero32(ux.ui32) - 8u;
		ux.ui32 <<= c;
		exponent -= (int_t)c - 1;
	}
	exponent -= 126;
	ux.ui32 &= 0x007FFFFFu;
	ux.ui32 |= 0x3F000000u;
	switch (exponent % 3) {
		case -2:
		case 1:
			exponent += 2;
			ux.ui32 -= 0x01000000u;
			break;
		case -1:
		case 2:
			exponent += 1;
			ux.ui32 -= 0x00800000u;
			break;
		default:
			break;
	}
	xd = ux.f32;
	ux.f32 = (f32_t)((0.164705865585441 + ((13.2513868634597 + ((115.1358553761178 +
				((181.9414139157457 + (41.95971194004274 * xd)) * xd)) * xd)) * xd)) /
	    (1.0    + ((33.30169492280659 + ((161.6940396106312 +
	            ((142.8167287366127 + (13.64061797885738 * xd)) * xd)) * xd)) * xd)));
	exponent /= 3;
	MISRA_EXCEPTION_RULE_10_3()
	exponent = (int_t)((uint_t)exponent << 23);
	ux.ui32 += (uint32_t)exponent;
	ux.ui32 |= sign;
	return ux.f32;
}

#endif /* _EWL_USES_SUN_SP_MATH_LIB */

#if !_EWL_USES_SUN_SP_MATH_LIB && !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

#if defined(__PPC_EABI__)
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL rintf(f32_t x) _EWL_CANT_THROW
{
	return (f32_t)rint((f64_t)x);
}
#endif

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL erff(f32_t x) _EWL_CANT_THROW
{
	f32_t res = (f32_t)erf((f64_t)x);
	return res;
}

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL erfcf(f32_t x) _EWL_CANT_THROW
{
	#if _EWL_C99
		if(x >= HUGE_VAL) {		/* range error may occur */
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		}	/* x too large */
	#endif
	return((f32_t)(1.0 - erf((f64_t)x)));
}

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL exp2f(f32_t x) _EWL_CANT_THROW
{
	const f32_t x_ln2 = x * 0.6931471805599453094172321f;
	/* exp sets the range error */
	f32_t res = (f32_t)exp((f64_t)x_ln2);
	return res;
}


_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL lgammaf(f32_t x) _EWL_CANT_THROW
{
	f32_t res = (f32_t)(log(tgamma((f64_t)x)));
	return res;
}

#endif /* !_EWL_USES_SUN_SP_MATH_LIB && !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nanf(const char_t* str) _EWL_CANT_THROW
{
	#pragma unused(str)
	MISRA_QUIET_UNUSED_ARGS()
	return NAN;
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nextafterf(f32_t x, f32_t y) _EWL_CANT_THROW
{
	return nexttowardf(x, (ldbl_t)y);
}

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nexttowardf(f32_t x, ldbl_t y) _EWL_CANT_THROW
{
	int_t increase;
	int_t positive;

	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* lx = (uint32_t *)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t* ly = (uint32_t*)&y;
	uint32_t x_exp = *lx & 0x7F800000u;

	MISRA_EXCEPTION_RULE_13_3()
#if _EWL_LITTLE_ENDIAN
	if ((((ly[1] & 0x7FF00000u) == 0x7FF00000u) && ((ly[1] & 0x000FFFFFu) | ly[0])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f32_t)y;
	}
#else
	if ((((ly[0] & 0x7FF00000u) == 0x7FF00000u) && ((ly[0] & 0x000FFFFFu) | ly[1])) ||
	    (x == y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (f32_t)y;
	}
#endif /* _EWL_LITTLE_ENDIAN */
	if ((x_exp == 0x7F800000u) && (*lx & 0x007FFFFFu)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	increase = y > x;
	positive = x > 0.0;
	if ((x_exp != 0x7F800000u) || (increase != positive)) {
		if (x == 0.0){
			if (increase) {
				*lx = 0x00000001UL;
			} else {
				*lx = 0x80000001UL;
			}
		} else {
			if (increase == positive) {
				++(*lx);
			} else {
				--(*lx);
			}
		}
		x_exp = *lx & 0x7F800000u;
		MISRA_EXCEPTION_RULE_13_7()
		if ((x_exp == 0u) && (math_errhandling & MATH_ERREXCEPT)) {
			feraiseexcept((int_t)(FE_UNDERFLOW | FE_INEXACT));
		} else if (x_exp == 0x7F800000u) {
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
	return x;
}

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL tgammaf(f32_t x) _EWL_CANT_THROW
{
	return((f32_t)tgamma((f64_t)x));
}

#else

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL tgammaf(f32_t x) _EWL_CANT_THROW
{
	f32_t coefs[] = {
		+1.0000000000000000F,
		+0.5772156649915329F,
		-0.6558780715202538F,
		-0.0420026350340952F,
		+0.1665386113822915F,
		-0.0421977345555443F,
		-0.0096219715278770F,
		+0.0072189432466630F,
		-0.0011651675918591F,
		-0.0002152416741149F,
		+0.0001280502823882F,
		-0.0000201348547807F,
		-0.0000012504934821F,
		+0.0000011330272320F,
		-0.0000002056338417F,
		+0.0000000061160960F,
		+0.0000000050020075F,
		-0.0000000011812746F,
		+0.0000000001043427F,
		+0.0000000000077823F,
		-0.0000000000036968F,
		+0.0000000000005100F,
		-0.0000000000000206F,
		-0.0000000000000054F,
		+0.0000000000000014F,
		+0.0000000000000001F
	};
	
	f32_t	argx = x;
	f32_t	dv = 0.0F;
	uint_t  index;

	#if _EWL_C99
		if (isfinite(x))
		{
			MISRA_EXCEPTION_RULE_13_3()
			if((floorf(x) == x) && (x <= 0.0F)) {			/* domain error may occur - negative integer OR zero */
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
			else if (x >= HUGE_VAL) {		/* range error may occur - magnitude of x too high */
				MISRA_EXCEPTION_RULE_20_5()
				errno = ERANGE;
			}
		}
		#if _EWL_IEC_559_ADDITIONS
			if (x == 0.0F) {
				/* raise divide by zero exception */
				if (signbit(x)) {
					MISRA_EXCEPTION_RULE_14_7()
					return -INFINITY;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return INFINITY;
				}
			}
			if (x < 0.0F) {
				f32_t fracpart, intpart;
				
				fracpart = modff(x, &intpart);
				if (fracpart == 0.0F) {
					/* raised invalid exception above */
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				}
			}
			if (x == -INFINITY) {
				/* raise invalid (and range) exception */
				errno = ERANGE;
				MISRA_EXCEPTION_RULE_14_7()
				return NAN;
			}
			if (x == INFINITY) {
				/* raise range exception */
				errno = ERANGE;
				MISRA_EXCEPTION_RULE_14_7()
				return INFINITY;
			}
		#endif /* _EWL_IEC_559_ADDITIONS */
	#endif	

	for (index = 0u; index < (sizeof(coefs)/sizeof(f32_t)); index++) {
		dv += argx * coefs[index];
		argx *= x;
	}

	return(1.0F/dv);
}

#endif /* _EWL_ONLY_SINGLE_PRECISION_MATH_LIB */
#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_C99 */

#endif
