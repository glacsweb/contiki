/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/27 13:22:31 $
 * $Revision: 1.11 $
 */

/*	include header and instantiate _EWL_INLINEs as functions */
#include <math.h>
#include <fenv.h>
#include <limits.h>

#if _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL nan(const char_t* str) _EWL_CANT_THROW
{
	extern int32_t __double_nan[];
	MISRA_QUIET_UNUSED_ARGS()
	return *(f64_t *) __double_nan;
}

f32_t sqrtf(f32_t x)
{
	f32_t z;
	int32_t sign = (int32_t)0x80000000L;
	int32_t ix,s,q,m,t,i;
	uint32_t r;
	f32_t	one	= 1.0F, tiny=1.0e-30F;

	uint32_t NaN = 0x7fffffffu;
    MISRA_EXCEPTION_RULE_11_4()
	f32_t qNaN = *((f32_t*)&NaN);

    MISRA_EXCEPTION_RULE_11_4()
	ix = *((int32_t*)&x);

    /* take care of Inf and NaN */
	if (ix == 0x7f800000) {
		return x;			/* sqrt (+inf) = +inf */
    }
    if (((uint32_t)ix&0x7f800000u)==0x7f800000u) {
	    return qNaN;
	}
    /* take care of zero */
	if (ix<=0) {
        MISRA_EXCEPTION_RULE_12_7()
	    if ((ix&(~sign))==0) {
            return x;	/* sqrt(+-0) = +-0 */
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
			errno=EDOM;  /* mf-- added to conform to old ANSI standard */
		#endif
   	    return qNaN;
	}
    /* normalize x */
    MISRA_EXCEPTION_RULE_12_7()
	m = (ix>>23);
	if (m==0) {
	    for(i=0;((uint32_t)ix&0x00800000u)==0u;i++) {
            MISRA_EXCEPTION_RULE_12_7()
            ix<<=1;
        }
	    m -= i-1;
	}
	m -= 127;	/* unbias exponent */
    MISRA_EXCEPTION_RULE_10_3()
	ix = (int32_t)(((uint32_t)ix&0x007fffffu)|0x00800000u);
	if ((uint32_t)m&1u) {	/* odd m, double x to make it even */
	    ix += ix;
    }
    MISRA_EXCEPTION_RULE_12_7()
	m >>= 1;	/* m = [m/2] */

    /* generate sqrt(x) bit by bit */
	ix += ix;
	q = 0; s = 0;		/* q = sqrt(x) */
	r = 0x01000000u;		/* r = moving bit from right to left */

	while (r!=0u) {
	    t = s+(int32_t)r;
	    if (t<=ix) {
            s    = t+(int32_t)r;
            ix  -= t;
            q   += (int32_t)r;
	    }
	    ix += ix;
	    r>>=1;
	}

    /* use floating add to find out rounding direction */
	if (ix!=0) {
	    z = one-tiny; /* trigger inexact flag */
	    if (z>=one) {
	        z = one+tiny;
            if (z>one) {
                q += 2;
            } else {
                MISRA_EXCEPTION_RULE_10_3()
                q += (int32_t)((uint32_t)q&1u);
            }
	    }
	}
    MISRA_EXCEPTION_RULE_12_7()
	ix = (q>>1)+0x3f000000;
    MISRA_EXCEPTION_RULE_12_7()
	ix += (m <<23);
    MISRA_EXCEPTION_RULE_11_4()
	z = *((f32_t*)&ix);
	return z;
}

#endif /* _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS */

#if _EWL_C99
#ifdef _CW_X_SLLD_
_EWL_IMP_EXP_C  f64_t      _EWL_MATH_CDECL log2(f64_t x)
{
	#if _EWL_LITTLE_ENDIAN
		static const uint32_t f[2] = {0x652b82fe, 0x3ff71547};
	#else
		static const uint32_t f[2] = {0x3ff71547, 0x652b82fe};
	#endif
	/* domain error set in log */
	return log(x) * *(f64_t*)f;
}

_EWL_IMP_EXP_C f64_t  _EWL_MATH_CDECL scalbln (f64_t x, int32_t n)
{
	return ldexp(x,(int_t)n);
}

#endif /* _CW_X_SLLD_ */
#endif /* _EWL_C99 */

#if _EWL_C99
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL rint(f64_t num)
{
	int32_t result = 0;
	int8_t  neg = 0;
	int32_t intpart;
	f64_t   fracpart;

	if (!isfinite(num)) {
		return(num);
	}
	if (num > (f64_t)LONG_MAX)
	{
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
		result = LONG_MAX;
	}
	else
	{
		if (num < 0.0)
		{
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
					if (fracpart >= 0.5) {
						result += 1L;
					}
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
		}
	}
	if (result != num) 
	{
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
			MISRA_EXCEPTION_RULE_20_5()
			feraiseexcept(FE_INVALID);
		}	
	}	
	return(f64_t)result;
}

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL rintf(f32_t num)
{
	int32_t result = 0;
	int8_t  neg = 0;
	int32_t intpart;
	f32_t   fracpart;

	if (!isfinite(num)) {
		return(num);
	}
	if (num > (f32_t)LONG_MAX)
	{
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
		result = LONG_MAX;
	}
	else
	{
		if (num < 0.0F)
		{
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
					if (fracpart >= 0.5F) {
						result += 1L;
					}
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
		}
	}
	if (result != num) 
	{
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
			MISRA_EXCEPTION_RULE_20_5()
			feraiseexcept(FE_INVALID);
		}	
	}	
	return(f32_t)result;
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL rintl(ldbl_t num)
{
	int32_t result = 0;
	int8_t  neg = 0;
	int32_t intpart;
	ldbl_t  fracpart;

	if (!isfinite(num)) {
		return(num);
	}
	if (num > (ldbl_t)LONG_MAX)
	{
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
		result = LONG_MAX;
	}
	else
	{
		if (num < 0.0L)
		{
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
					if (fracpart >= 0.5L) {
						result += 1L;
					}
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
		}
	}
	if (result != num) 
	{
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERREXCEPT) {
			MISRA_EXCEPTION_RULE_20_5()
			feraiseexcept(FE_INVALID);
		}	
	}	
	return(ldbl_t)result;
}
#endif	/* _EWL_C99 */
