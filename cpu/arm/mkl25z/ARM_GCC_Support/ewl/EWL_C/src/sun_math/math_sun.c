/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:14 $
 * $Revision: 1.12 $
 */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <math_sun_gcc.c>
#else

#include <ewl_misra_types.h>

#if _EWL_FLOATING_POINT

#include <math.h>
#include <float.h>
#include <fenv.h>
#include <errno.h>
#include <limits.h>

#if (_EWL_C99 || _EWL_USES_SUN_MATH_LIB) && !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL scalbn(f64_t x, int_t n)
{
	f64_t mant;
	int_t    xp;
	f64_t result;

	mant = frexp(x, &xp);
	xp += n;
	result = ldexp(mant, xp);

	return(result);
}
#endif

#if _EWL_C99 || _EWL_C99_MATH_LEAK
#if _EWL_USES_SUN_MATH_LIB
#if 0
_EWL_IMP_EXP_C	int _EWL_MATH_CDECL isgreater(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return (__ewl_relation(x, y) == 0);
}

_EWL_IMP_EXP_C	int _EWL_MATH_CDECL isgreaterequal(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return (__ewl_relation(x, y) % 2 == 0);
}

_EWL_IMP_EXP_C	int _EWL_MATH_CDECL isless(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return (__ewl_relation(x, y) == 1);
}

_EWL_IMP_EXP_C	int _EWL_MATH_CDECL islessequal(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return ((__ewl_relation(x, y)+1)/2 == 1);
}

_EWL_IMP_EXP_C	int _EWL_MATH_CDECL islessgreater(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return (__ewl_relation(x, y) <= 1);
}

_EWL_IMP_EXP_C	int _EWL_MATH_CDECL isunordered(f64_t x, f64_t y) _EWL_CANT_THROW
{
	return (__ewl_relation(x, y) == 3);
}

#endif

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL exp2(f64_t x) _EWL_CANT_THROW
{
	const f64_t _ln2 = 0.6931471805599453094172321;
	/* exp sets the range error */
	return(exp(x * _ln2));
}
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

#if _EWL_LONGLONG
#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

#if !defined(__arm)
_EWL_IMP_EXP_C int64_t _EWL_MATH_CDECL llrint(f64_t x) _EWL_CANT_THROW
{
	/* domain or range error may occur */
	#if _EWL_C99
		if (x > (f64_t)LONG_MAX)
		{
			MISRA_EXCEPTION_RULE_13_7()
			if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
				MISRA_EXCEPTION_RULE_20_5()
				errno=ERANGE;
			}
		}
	#endif
	return ((int64_t)rint(x));
}
#endif /* !defined(__arm) */


_EWL_IMP_EXP_C int64_t _EWL_MATH_CDECL llround(f64_t num)
{
	int64_t result = 0LL;
	int8_t neg = 0, correction = 0;
	int64_t intpart;
	f64_t fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}
	if ((num > (f64_t)LLONG_MAX) || (num < (f64_t)LLONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LLONG_MAX);
	}
	if (num < 0.0)
	{
		if (num == (f64_t)LLONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LLONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int64_t)num;
	fracpart = num - (f64_t)intpart;
	num -= fracpart;
	result = (int64_t)num;
	if (fracpart >= 0.5) {
		result += 1LL;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1LL;
		}
	}
	return(result);
}

#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C int64_t _EWL_MATH_CDECL llroundf(f32_t num)
{
	int64_t result = 0LL;
	int8_t  neg = 0, correction = 0;
	int64_t intpart;
	f32_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}
	if ((num > (f32_t)LLONG_MAX) || (num < (f32_t)LLONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LLONG_MAX);
	}
	if (num < 0.0F)
	{
		if (num == (f32_t)LLONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LLONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int64_t)num;
	fracpart = num - (f32_t)intpart;
	num -= fracpart;
	result = (int64_t)num;
	if (fracpart >= 0.5F) {
		result += 1LL;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1LL;
		}
	}
	return(result);
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C int64_t _EWL_MATH_CDECL llroundl(ldbl_t num)
{
	int64_t result = 0LL;
	int8_t   neg = 0, correction = 0;
	int64_t intpart;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0LL);
	}
	if ((num > (ldbl_t)LLONG_MAX) || (num < (ldbl_t)LLONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LLONG_MAX);
	}
	if (num < 0.0L)
	{
		if (num == (ldbl_t)LLONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LLONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int64_t)num;
	fracpart = num - (ldbl_t)intpart;
	num -= fracpart;
	result = (int64_t)num;
	if (fracpart >= 0.5L) {
		result += 1LL;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1LL;
		}
	}
	return(result);
}

#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */
#endif /* _EWL_LONGLONG */

#if !defined(__arm)
#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lrint(f64_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	f64_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (int32_t)num;
	}
	if ((num > (f64_t)LONG_MAX) || (num < (f64_t)LONG_MIN))
	{
		/* domain or range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0)
	{
		if (num == (f64_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (f64_t)intpart;
	num -= fracpart;
	result = (int32_t)num;

	if (fracpart > 0.0)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1L;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5) {
					result += 1L;
				} else if (fracpart == 0.5) {
					/* only round up if the current integer is odd */
					MISRA_EXCEPTION_RULE_12_7a()
					if (intpart & 1L) {
						result += 1L;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1L;
				}
				break;
			default:
				break;
		}
	}

	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}

#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lrintf(f32_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	f32_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (int32_t)num;
	}
	if ((num > (f32_t)LONG_MAX) || (num < (f32_t)LONG_MIN))
	{
 		/* domain or range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0F)
	{
		if (num == (f32_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (f32_t)intpart;
	num -= fracpart;
	result = (int32_t)num;

	if (fracpart > 0.0F)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1L;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5F) {
					result += 1L;
				} else if (fracpart == 0.5F) {
					/* only round up if the current integer is odd */
					MISRA_EXCEPTION_RULE_12_7a()
					if (intpart & 1L) {
						result += 1L;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1L;
				}
				break;
			default:
				break;
		}
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}
#endif /* !defined(__arm) */

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lrintl(ldbl_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (int32_t)num;
	}
	if ((num > (ldbl_t)LONG_MAX) || (num < (ldbl_t)LONG_MIN))
	{
 		/* domain or range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0L)
	{
		if (num == (ldbl_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (ldbl_t)intpart;
	num -= fracpart;
	result = (int32_t)num;

	if (fracpart > 0.0L)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1L;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5L) {
					result += 1L;
				} else if (fracpart == 0.5L) {
					/* only round up if the current integer is odd */
					MISRA_EXCEPTION_RULE_12_7a()
					if (intpart & 1L) {
						result += 1L;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1L;
				}
				break;
			default:
				break;
		}
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}

_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lround(f64_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	f64_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0L);
	}
	if ((num > (f64_t)LONG_MAX) || (num < (f64_t)LONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0)
	{
		if (num == (f64_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (f64_t)intpart;
	num -= fracpart;
	result = (int32_t)num;
	if (fracpart >= 0.5) {
		result += 1L;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lroundf(f32_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	f32_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0L;
	}
	if ((num > (f32_t)LONG_MAX) || (num < (f32_t)LONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0F)
	{
		if (num == (f32_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (f32_t)intpart;
	num -= fracpart;
	result = (int32_t)num;
	if (fracpart >= 0.5F) {
		result += 1L;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C int32_t _EWL_MATH_CDECL lroundl(ldbl_t num)
{
	int32_t result = 0;
	int8_t  neg = 0, correction = 0;
	int32_t intpart;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0L);
	}
	if ((num > (ldbl_t)LONG_MAX) || (num < (ldbl_t)LONG_MIN))
	{
		/* range error may occur */
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
		MISRA_EXCEPTION_RULE_14_7()
		return(LONG_MAX);
	}
	if (num < 0.0L)
	{
		if (num == (ldbl_t)LONG_MIN)
		{
			/* The absolute value of this number can not be represented as a	*/
			/* positive number because it will be one greater than LONG_MAX.	*/
			/* See num = -num below.  We note that with 'correction' and		*/
			/* subtract 1 from the result after we convert result back to		*/
			/* negative with result = -result.									*/
			correction = 1;
		}
		neg = 1;
		num = -num;
	}
	intpart = (int32_t)num;
	fracpart = num - (ldbl_t)intpart;
	num -= fracpart;
	result = (int32_t)num;
	if (fracpart >= 0.5L) {
		result += 1L;
	}
	if (neg) {
		result = -result;
		if (correction) {
			result -= 1L;
		}
	}
	return(result);
}

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL nearbyint(f64_t num)
{
	f64_t 	result = 0.0;
	int8_t  neg = 0;
	f64_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modf(num, &result);
	if (fracpart > 0.0)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1.0;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5) {
					result += 1.0;
				} else if (fracpart == 0.5) {
					/* only round up if the current integer is odd */
					if ((int32_t)fmod(num, 2.0)) {
						result += 1.0;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1.0;
				}
				break;
			default:
				break;
		}
	}
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL nearbyintf(f32_t num)
{
	f32_t 	result = 0.0F;
	int8_t  neg = 0;
	f32_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0F)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modff(num, &result);
	if (fracpart > 0.0F)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1.0F;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5F) {
					result += 1.0F;
				} else if (fracpart == 0.5F) {
					/* only round up if the current integer is odd */
					if ((int32_t)fmodf(num, 2.0F)) {
						result += 1.0F;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1.0F;
				}
				break;
			default:
				break;
		}
	}
	if (neg) {
		result = -result;
	}
	return(result);
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL nearbyintl(ldbl_t num)
{
	ldbl_t 	result = 0.0L;
	int8_t  neg = 0;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0L)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modfl(num, &result);
	if (fracpart > 0.0L)
	{
		switch(fegetround())
		{
			case(FE_DOWNWARD):
				if (neg) {
					result += 1.0L;
				}
				break;
			case(FE_TONEAREST):
				/* Round to nearest, ties to even */
				if (fracpart > 0.5L) {
					result += 1.0L;
				} else if (fracpart == 0.5L) {
					/* only round up if the current integer is odd */
					if ((int32_t)fmodl(num, 2.0L)) {
						result += 1.0L;
					}
				} else {}
				break;
			case(FE_TOWARDZERO):
				break;
			case(FE_UPWARD):
				if (!neg) {
					result += 1.0L;
				}
				break;
			default:
				break;
		}
	}
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /*!_EWL_ONLY_SINGLE_PRECISION_MATH_LIB*/

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
#if !defined(__STARCORE__) || defined(_CW_X_SLLD_)
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL remquo(f64_t x, f64_t y, int_t *quo)
{
	f64_t  result;
	f64_t  fracpart;
	f64_t  tempquo;
	uint8_t sign = 0U;

	/* remainder raises the domain error */
	result = remainder(x, y);
	if (x < 0.0) {
		sign = 1U;
		x = -x;
	}
	if (y < 0.0) {
		sign ^= 1U;
		y = -y;
	}
	tempquo = x / y;
	*quo = (int_t)tempquo;
	fracpart = fabs((x / y) - (f64_t)*quo);
	if (fracpart > 0.5) {
		*quo += 1;
	} else if (fracpart == 0.5) {
		*quo += (*quo % 2);
	}
	else {}
	if (sign) {
		*quo = -*quo;
	}
	return(result);
}
#endif /* !defined(__STARCORE__) || defined(_CW_X_SLLD_)*/
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

#if !defined(__STARCORE__) || defined(_CW_X_SLLD_)
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL remquof(f32_t x, f32_t y, int_t *quo)
{
	f32_t  result;
	f32_t  fracpart;
	f32_t  tempquo;
	uint8_t sign = 0U;

	result = remainderf(x, y);
	if (x < 0.0F)
	{
		sign = 1U;
		x = -x;
	}
	if (y < 0.0F)
	{
		sign ^= 1U;
		y = -y;
	}
	tempquo = x / y;
	*quo = (int_t)tempquo;
	fracpart = fabsf((x / y) - (f32_t)*quo);
	if (fracpart > 0.5F) {
		*quo += 1;
	} else if (fracpart == 0.5F) {
		*quo += (*quo % 2);
	} else {}
	if (sign) {
		*quo = -*quo;
	}
	return(result);
}
#endif /* !defined(__STARCORE__) || defined(_CW_X_SLLD_)*/

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
#if !defined(__STARCORE__) || defined(_CW_X_SLLD_)
_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL remquol(ldbl_t x, ldbl_t y, int_t *quo)
{
	ldbl_t result;
	ldbl_t fracpart;
	ldbl_t tempquo;
	uint8_t sign = 0U;

	result = remainderl(x, y);
	if (x < 0.0L)
	{
		sign = 1U;
		x = -x;
	}
	if (y < 0.0L)
	{
		sign ^= 1U;
		y = -y;
	}
	tempquo = x / y;
	*quo = (int_t)tempquo;
	fracpart = fabsl((x / y) - (ldbl_t)*quo);
	if (fracpart == 0.5L) {
		*quo += (*quo % 2);
	} else if (fracpart > 0.5L) {
		*quo += 1;
	} else {}
	if (sign) {
		*quo = -*quo;
	}
	return(result);
}
#endif /* !defined(__STARCORE__) || defined(_CW_X_SLLD_) */

#if !__has_intrinsic(__builtin_round)
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL round(f64_t num)
{
	f64_t   result = 0.0;
	int8_t  neg = 0;
	f64_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modf(num, &result);
	if (fracpart >= 0.5) {
		result += 1.0;
	}
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* !__has_intrinsic(__builtin_round) */
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */


_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL roundf(f32_t num)
{
	f32_t   result = 0.0F;
	int8_t  neg = 0;
	f32_t   fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0F)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modff(num, &result);
	if (fracpart >= 0.5F) {
		result += 1.0F;
	}
	if (neg) {
		result = -result;
	}
	return(result);
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL roundl(ldbl_t num)
{
	ldbl_t result = 0.0L;
	int8_t  neg = 0;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
	}
	if (num < 0.0L)
	{
		neg = 1;
		num = -num;
	}
	fracpart = modfl(num, &result);
	if (fracpart >= 0.5L) {
		result += 1.0L;
	}
	if (neg) {
		result = -result;
	}
	return(result);
}

#if (FLT_RADIX != 2)
#error Unsupported value of FLT_RADIX
#endif


_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL scalbnl(ldbl_t  x, int_t n)
{
	ldbl_t mant;
	int_t  xp;
	ldbl_t result;

	mant = frexpl(x, &xp);
	xp += n;
	result = ldexpl(mant, xp);

	return(result);
}

#if !defined(__STARCORE__) || defined(_CW_X_SLLD_)
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL tgamma(f64_t x) _EWL_CANT_THROW
{
	return((f64_t)tgammal((ldbl_t)x));
}
#endif /* !defined(__STARCORE__) || defined(_CW_X_SLLD_) */

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL trunc(f64_t num)
{
	int8_t  neg = 0;
	f64_t   result;

	if (!isfinite(num)) {
#if _EWL_IEC_559_ADDITIONS
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(0.0);
#endif /* _EWL_IEC_559_ADDITIONS */
	}
	if (num < 0.0)
	{
		neg = 1;
		num = -num;
	}
	(void)modf(num, &result);
	if (neg) {
		result = -result;
	}
	return(result);
}
#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL truncf(f32_t num)
{
	int8_t  neg = 0;
	f32_t   result;

	if (!isfinite(num)) {
#if _EWL_IEC_559_ADDITIONS
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(0.0F);
#endif /* _EWL_IEC_559_ADDITIONS */
	}
	if (num < 0.0F)
	{
		neg = 1;
		num = -num;
	}
	(void)modff(num, &result);
	if (neg) {
		result = -result;
	}
	return(result);
}

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL truncl(ldbl_t num)
{
	int8_t  neg = 0;
	ldbl_t  result;

	if (!isfinite(num)) {
#if _EWL_IEC_559_ADDITIONS
		MISRA_EXCEPTION_RULE_14_7()
		return(num);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(0.0L);
#endif /* _EWL_IEC_559_ADDITIONS */
	}
	if (num < 0.0L)
	{
		neg = 1;
		num = -num;
	}
	(void)modfl(num, &result);
	if (neg) {
		result = -result;
	}
	return(result);
}

#endif /*!_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */

#endif /* _EWL_C99 */

#endif /* _EWL_FLOATING_POINT */

#endif /* _EWL_USES_SUN_MATH_LIB */

#endif

