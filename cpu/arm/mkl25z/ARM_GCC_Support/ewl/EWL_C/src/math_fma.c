/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:10 $
 * $Revision: 1.12 $
 */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <math_fma_gcc.c>
#else

#if _EWL_C99 || _EWL_C99_MATH_LEAK
#if _EWL_FLOATING_POINT
#if _EWL_LONGLONG

#include <ewl_misra_types.h>
#include <math.h>
#include <float.h>
#include <fenv.h>
#include <string.h>
#include <limits.h>

#if !__has_intrinsic(__builtin_fma) && !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL fma(f64_t x, f64_t y, f64_t z) _EWL_CANT_THROW
{
	/* range error may occur - not specified */
	uint64_t p_hi, p_lo;
	int32_t shift;
	uint32_t xp;
	uint32_t sp, x_lo, y_lo, z_lo;
	int32_t ep;

	uint32_t x_hi = __UHI(x);
	uint32_t y_hi = __UHI(y);
	uint32_t z_hi = __UHI(z);

	/* extract exponent */
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ex = (int32_t)(x_hi & 0x7FF00000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ey = (int32_t)(y_hi & 0x7FF00000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ez = (int32_t)(z_hi & 0x7FF00000UL);

	/* extract sign bits */
	uint32_t sx = x_hi & 0x80000000UL;
	uint32_t sy = y_hi & 0x80000000UL;
	uint32_t sz = z_hi & 0x80000000UL;
#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	int_t x_inf, y_inf, z_inf, z_nan;
#endif /* #if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	f64_t fabs_x;
	f64_t fabs_y;
	f64_t fabs_z;
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

	
#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL

	MISRA_EXCEPTION_RULE_12_4()
	if (isnan(x) || isnan(y)) {
		MISRA_EXCEPTION_RULE_10_2()
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}
	x_inf = isinf(x);
	y_inf = isinf(y);
	z_inf = isinf(z);
	z_nan = isnan(z);

	if ((x_inf && (y == 0.0)) || (y_inf && (x == 0.0))) {
		if (z_nan) {
			/* optionally raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		} else {
			/* raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		}
	}
	if (z_nan) {
		MISRA_EXCEPTION_RULE_10_2()
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}

	if (x_inf || y_inf) {
		int_t msign = 0; /* positive */

		if (z_inf) {
			if (sx) {
				if (sy) {
					/* negative * negative == positive */
					msign = 0;
				} else {
					/* negative * positive == negative */
					msign = 1;
				}
			} else {
				if (sy) {
					/* positive * negative == negative */
					msign = 1;
				} else {
					/* positive * positive == positive */
					msign = 0;
				}
			}
			if (sz) {
				if (msign) {
					/* negative + negative == negative */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return -INFINITY;
				} else {
					/* positive + negative == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				}
			} else {
				if (msign) {
					/* negative + positive == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				} else {
					/* positive + positive == positive */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return INFINITY;
				}
			}
		}
	}
#endif /* _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	if ((x_inf || y_inf) && !z_inf) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return INFINITY;
		} else {
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return -INFINITY;
		}
	}
	/* adding infinity to anything except a NaN equals infinity */
	MISRA_EXCEPTION_RULE_13_3()
	MISRA_EXCEPTION_RULE_10_2()
	if (((z == INFINITY) || (z == -INFINITY))) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* handle some special cases with +/- zeros */
	if ((x == 0.0) && (y == 0.0)) {
		if ((z == 0.0) && (sx == sy)) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	if (((x == 0.0) || (y == 0.0)) && (z == 0.0)) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	/* handle some special cases when either (x || y) == +/- 1.0 */
	fabs_x = fabs(x);
	fabs_y = fabs(y);
	fabs_z = fabs(z);
	MISRA_EXCEPTION_RULE_13_3()
	if (fabs_x == 1.0) {
		if (fabs_y == fabs_z) {
			if (!sx) {
				if (sy != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0;
				}
			} else {
				if (sy == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0;
				}
			}
		}
	}
	if (fabs_y == 1.0) {
		MISRA_EXCEPTION_RULE_13_3()
		if (fabs_x == fabs_z) {
			if (!sy) {
				if (sx != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0;
				}
			} else {
				if (sx == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0;
				}
			}
		}
	}
	if ((x == 0.0) || (y == 0.0)) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* if z is 0.0, it gets handled below */
#else
	/* test for cases we don't need extended precision */
	if ((x == 0.0) || (ex == 0x7FF00000L) ||
	    (y == 0.0) || (ey == 0x7FF00000L) ||
	    (z == 0.0) || (ez == 0x7FF00000L)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x * y) + z;
	}
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
	/* all of x, y and z are now either FP_NORMAL or FP_SUBNORMAL
	   represent each x as:  sx = sign, ex = biased exponent, x_hi:x_lo 53 bit mantissa
	   shift exponents down, they are still biased */
	MISRA_EXCEPTION_RULE_12_7()
	{
	ex >>= 20;
	ey >>= 20;
	ez >>= 20;
	}
	/* mask off mantissas.  Explicitly store implicit leading bit.  And correct 0 exponent */
	x_hi &= 0x000FFFFFUL;
	if (ex == 0L) {
		ex = 1;
	} else {
		x_hi |= 0x00100000UL;
	}
	y_hi &= 0x000FFFFFUL;
	if (ey == 0L) {
		ey = 1;
	} else {
		y_hi |= 0x00100000UL;
	}
	z_hi &= 0x000FFFFFUL;
	if (ez == 0L) {
		ez = 1;
	} else {
		z_hi |= 0x00100000UL;
	}
	/* extract least significant 32 bits */
	x_lo = __ULO(x);
	y_lo = __ULO(y);
	z_lo = __ULO(z);

	/* Perform p = x * y
	   p will be 118 bits stored in 2 uint64_t's, p_hi:p_lo (only 106 bits are actually generated)
	   compute least significant 32 bits of p_lo.  Use p_hi as an accumulator. */
	p_hi = (uint64_t)x_lo * (uint64_t)y_lo;
	p_lo = p_hi & 0xFFFFFFFFULL;
	p_hi >>= 32;
	/* compute most significant 32 bits of p_lo. */
	p_hi += (uint64_t)x_hi * (uint64_t)y_lo;
	p_hi += (uint64_t)x_lo * (uint64_t)y_hi;
	p_lo |= p_hi << 32;
	p_hi >>= 32;
	/* compute remaining 42 bits */
	p_hi += (uint64_t)x_hi * (uint64_t)y_hi;
	/* shift p_hi:p_lo left by 12 bits to "normalize" it */
	p_hi = (p_hi << 12) | (p_lo >> 52);
	p_lo <<= 12;
	/* compute exponent ep */
	ep = (ex + ey) - 0x3FFL;
	/* compute sign sp */
	sp = sx ^ sy;

	/* Perform p + z */
	shift = ez - ep;
	/* -106 < shift < 55
	   if shift is too low, return p
	   if shift is too high, return z
	   otherwise do addition */
	if (shift >= 55L) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;  /* p is so small it cannot effect z */
	}
	/* sticky guard bit for p */
	xp = 0U;
	if (shift > -106L) {  /* if z is big enough so that it can effect p */
		uint64_t q_hi, q_lo;
		uint32_t xq;
		/* put z into extended precision q_hi:q_lo */
		q_hi = (uint64_t)z_hi << 32;
		q_hi |= (uint64_t)z_lo;
		q_lo = 0U;
		/* sticky guard bit for q */
		xq = 0U;
		/* make ez == ep */
		if (shift > 0L) {
			/* shift p right by shift bits, and capture lost bits in xp
			   0 < shift < 55 */
			MISRA_EXCEPTION_RULE_10_3()
			xp = (uint32_t) (((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_lo) != 0ULL);
			p_lo = (p_hi << (64L - shift)) | (p_lo >> shift);
			p_hi >>= shift;
			ep = ez;
		} else {
			if (shift < 0L) {
				shift = -shift;
				/* shift q right by shift bits, and capture lost bits in xq
				   0 < shift < 106
				   q_lo is known to be 0 */
				if (shift < 64L) {
					q_lo = q_hi << (64L - shift);
					q_hi >>= shift;
				} else {
					MISRA_EXCEPTION_RULE_10_3()
					xq = (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (128L-shift)) & q_hi) != 0ULL);
					q_lo = q_hi >> (shift - 64L);
					q_hi = 0U;
				}
				ez = ep;
			}
		}
		/* Now ez == ep */
		if (sp ^ sz) { /* subtract */
			int32_t low_sign = 0;
			int32_t high_sign = 0;
			if (p_lo < q_lo) {
				low_sign = -1;
			} else {
				if (p_lo > q_lo) {
					low_sign = 1;
				}
			}
			if (p_hi < q_hi) {
				high_sign = -1;
			} else {
				if (p_hi > q_hi) {
					high_sign = 1;
				}
			}
			if ((high_sign < 0L) ||
				((high_sign == 0L) && (low_sign < 0L)) ||
				((high_sign == 0L) && (low_sign == 0L) && (xp < xq))) {
				/* p = q - p */
				if (low_sign > 0L) {
					--q_hi;
				}
				p_lo = q_lo - p_lo;
				p_hi = q_hi - p_hi;
				sp = sz;
			} else {
				/* p = p - q */
				if (low_sign < 0L) {
					--p_hi;
				}
				p_lo -= q_lo;
				p_hi -= q_hi;
			}
		} else { /* addition */
			/* p += q */
			uint32_t carry = 0U;
			/* lo */
			if (q_lo > (0xFFFFFFFFFFFFFFFFULL - p_lo)) {
				carry = 1U;
			}
			p_lo += q_lo;
			/* hi */
			p_hi += q_hi + (uint64_t)carry;
		}
		/* x bit */
		xp |= xq;
	}
	/* normalize p */
	shift = 0;
	if (p_hi != 0ULL) {
		shift = (int32_t)__ewl_count_leading_zero64(p_hi) - 11L;
	} else if (p_lo != 0ULL) {
		shift = 53L + (int32_t)__ewl_count_leading_zero64(p_lo);
	} else { /* p == 0 */
		__ULO(x) = 0U;
		__UHI(x) = sp;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (shift > 0L) {
		/* shift left by shift bits
		   0 < shift < 116 */
		if (shift < 64L) {
			p_hi = (p_hi << shift) | (p_lo >> (64L - shift));
			p_lo <<= shift;
		} else {
			p_hi = p_lo << (shift - 64L);
			p_lo = 0UL;
		}
		ep -= shift;
	} else {
		if (shift < 0L) {
			shift = -shift;
			/* shift right by shift bits
			   0 < shift < 11 */
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_lo) != 0ULL);
			p_lo = (p_hi << (64L - shift)) | (p_lo >> shift);
			p_hi >>= shift;
			ep += shift;
		}
	}
	if (ep < 1L) { /* denormal or 0 */
		/* shift right by 1 - ep */
		shift = 1L - ep;
		if (shift < 64L) {
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_lo) != 0ULL);
			p_lo = (p_hi << (64L - shift)) | (p_lo >> shift);
			p_hi >>= shift;
			ep = 0L;
		} else {  /* return 0 */
			__ULO(x) = 0U;
			__UHI(x) = sp;
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	if (ep >= 0x7FFL) {
		/* return +/- inf */
		__ULO(x) = 0UL;
		__UHI(x) = sp | 0x7FF00000UL;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	/* put exponent and sign into p_hi */
	p_hi &= 0x000FFFFFFFFFFFFFULL;
	p_hi |= ((uint64_t)sp << 32) | ((uint64_t)(uint32_t)ep << 52);
	/* round p to 53 bits */
	switch (FLT_ROUNDS) {
	case 1:  /* to nearest (round to even on tie) */
		if ((p_lo & 0x8000000000000000ULL) && ((p_hi & 1ULL) ||
		    (p_lo & 0x7FFFFFFFFFFFFFFFULL) || xp)) {
			++p_hi;
		}
		break;
	case 2:  /* towards inf */
		if ((p_lo || xp) && !sp) {
			++p_hi;
		}
		break;
	case 3:  /* towards -inf */
		if ((p_lo || xp) && sp) {
			++p_hi;
		}
		break;
	case 0:  /* towards 0 */
	default: /* indeterminant */
		break;
	}
	__ULO(x) = (uint32_t)p_hi;
	__UHI(x) = (uint32_t)(p_hi >> 32);
	return x;
}
#endif /* !__has_intrinsic(__builtin_fma)  && !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */


#if !__has_intrinsic(__builtin_fmaf)
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fmaf(f32_t x, f32_t y, f32_t z) _EWL_CANT_THROW
{
	/* range error may occur - not specified */
	uint64_t p_i;
	int32_t shift;
	uint32_t xp;
	uint32_t sp;
	int32_t ep;
	/* get as unsigned integral */
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t x_i = *(uint32_t*)&x;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t y_i = *(uint32_t*)&y;
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	uint32_t z_i = *(uint32_t*)&z;
	/* extract exponent */
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ex = (int32_t)(x_i & 0x7F800000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ey = (int32_t)(y_i & 0x7F800000UL);
	MISRA_EXCEPTION_RULE_10_3()
	int32_t ez = (int32_t)(z_i & 0x7F800000UL);
	/* extract sign bits */
	uint32_t sx = x_i & 0x80000000UL;
	uint32_t sy = y_i & 0x80000000UL;
	uint32_t sz = z_i & 0x80000000UL;
#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	int_t x_inf, y_inf, z_inf, z_nan;
#endif /* #if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	f64_t fabs_x;
	f64_t fabs_y;
	f64_t fabs_z;
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

#if _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL

	MISRA_EXCEPTION_RULE_12_4()
	if (isnan(x) || isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}
	x_inf = isinf(x);
	y_inf = isinf(y);
	z_inf = isinf(z);
	z_nan = isnan(z);

	if ((x_inf && (y == 0.0F)) || (y_inf && (x == 0.0F))) {
		if (z_nan) {
			/* optionally raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		} else {
			/* raise invalid exception */
			MISRA_EXCEPTION_RULE_10_2()
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		}
	}
	if (z_nan) {
		MISRA_EXCEPTION_RULE_10_2()
		MISRA_EXCEPTION_RULE_14_7()
		return NAN;
	}

	if (x_inf || y_inf) {
		int_t msign = 0; /* positive */

		if (z_inf) {
			if (sx) {
				if (sy) {
					/* negative * negative == positive */
					msign = 0;
				} else {
					/* negative * positive == negative */
					msign = 1;
				}
			} else {
				if (sy) {
					/* positive * negative == negative */
					msign = 1;
				} else {
					/* positive * positive == positive */
					msign = 0;
				}
			}
			if (sz) {
				if (msign) {
					/* negative + negative == negative */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return -INFINITY;
				} else {
					/* positive + negative == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				}
			} else {
				if (msign) {
					/* negative + positive == negative */
					/* raise invalid exception */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return NAN;
				} else {
					/* positive + positive == positive */
					MISRA_EXCEPTION_RULE_10_2()
					MISRA_EXCEPTION_RULE_14_7()
					return INFINITY;
				}
			}
		}
	}
#endif /* _EWL_IEC_559_ADDITIONS || _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */

#if _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	if ((x_inf || y_inf) && !z_inf) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_14_7()
			return INFINITY;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return -INFINITY;
		}
	}
	/* adding infinity to anything except a NaN equals infinity */
	MISRA_EXCEPTION_RULE_13_3()
	if (((z == INFINITY) || (z == -INFINITY))) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* handle some special cases with +/- zeros */
	if ((x == 0.0F) && (y == 0.0F)) {
		if ((z == 0.0F) && (sx == sy)) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0F;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	if (((x == 0.0F) || (y == 0.0F)) && (z == 0.0F)) {
		if (sx == sy) {
			MISRA_EXCEPTION_RULE_14_7()
			return 0.0F;
		} else {
			MISRA_EXCEPTION_RULE_14_7()
			return z;
		}
	}
	/* handle some special cases when either (x || y) == +/- 1.0 */
	fabs_x = fabsf(x);
	fabs_y = fabsf(y);
	fabs_z = fabsf(z);
	if (fabs_x == 1.0F) {
		MISRA_EXCEPTION_RULE_13_3()
		if (fabs_y == fabs_z) {
			if (!sx) {
				if (sy != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			} else {
				if (sy == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			}
		}
	}
	if (fabs_y == 1.0F) {
		MISRA_EXCEPTION_RULE_13_3()
		if (fabs_x == fabs_z) {
			if (!sy) {
				if (sx != sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			} else {
				if (sx == sz) {
					MISRA_EXCEPTION_RULE_14_7()
					return 0.0F;
				}
			}
		}
	}
	if ((x == 0.0F) || (y == 0.0F)) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;
	}
	/* if z is 0.0, it gets handled below */
#else
	/* test for cases we don't need extended precision */
	if ((x == 0.0F) || (ex == 0x7F800000L) ||
	    (y == 0.0F) || (ey == 0x7F800000L) ||
	    (z == 0.0F) || (ez == 0x7F800000L)) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x * y) + z;
	}
#endif /* _EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL */
	/* all of x, y and z are now either FP_NORMAL or FP_SUBNORMAL
	   represent each x as:  sx = sign, ex = biased exponent, x_i 24 bit mantissa
	   shift exponents down, they are still biased */
	MISRA_EXCEPTION_RULE_12_7()
	{
	ex >>= 23;
	ey >>= 23;
	ez >>= 23;
	}
	/* mask off mantissas.  Explicitly store implicit leading bit.  And correct 0 exponent */
	x_i &= 0x007FFFFFUL;
	if (ex == 0L) {
		ex = 1;
	} else {
		x_i |= 0x00800000UL;
	}
	y_i &= 0x007FFFFFUL;
	if (ey == 0L) {
		ey = 1;
	} else {
		y_i |= 0x00800000UL;
	}
	z_i &= 0x007FFFFFUL;
	if (ez == 0L) {
		ez = 1;
	} else {
		z_i |= 0x00800000UL;
	}

	/* Perform p = x * y
	   p will be 57 bits stored in a uint64_t, p_i (only 48 bits are actually generated) */
	p_i = (uint64_t)x_i * (uint64_t)y_i;
	/* shift p_i left by 9 bits to "normalize" it */
	p_i <<= 9;
	/* compute exponent ep */
	ep = (ex + ey) - 0x7FL;
	/* compute sign sp */
	sp = sx ^ sy;

	/* Perform p + z */
	shift = ez - ep;
	/* -48 < shift < 26
	   if shift is too low, return p
	   if shift is too high, return z
	   otherwise do addition */
	if (shift >= 26L) {
		MISRA_EXCEPTION_RULE_14_7()
		return z;  /* p is so small it cannot effect z */
	}
	/* sticky guard bit for p */
	xp = 0U;
	if (shift > -48L) { /* if z is big enough so that it can effect p */
		/* put z into extended precision q_i */
		uint64_t q_i = (uint64_t)z_i << 32;
		/* sticky guard bit for q */
		uint32_t xq = 0U;
		/* make ez == ep */
		if (shift > 0L) {
			/* shift p right by shift bits, and capture lost bits in xp
			   0 < shift < 26 */
			MISRA_EXCEPTION_RULE_10_3()
			xp = (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep = ez;
		} else {
			if (shift < 0L) {
				shift = -shift;
				/* shift q right by shift bits, and capture lost bits in xq
				   0 < shift < 48 */
				MISRA_EXCEPTION_RULE_10_3()
				xq = (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & q_i) != 0ULL);
				q_i >>= shift;
				ez = ep;
			}
		}
		/* Now ez == ep */
		if (sp ^ sz) { /* subtract */
			if ((p_i < q_i) || ((p_i == q_i) && (xp < xq))) {
				/* p = q - p */
				p_i = q_i - p_i;
				sp = sz;
			} else {
				/* p = p - q */
				p_i -= q_i;
			}
		} else { /* addition */
			/* p += q */
			p_i += q_i;
		}
		/* x bit */
		xp |= xq;
	}
	/* normalize p */
	shift = 0;
	if (p_i != 0ULL) {
		shift = (int32_t)__ewl_count_leading_zero64(p_i) - 8L;
	} else { /* p == 0 */
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		*(uint32_t*)&x = sp;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	if (shift > 0L) {
		/* shift left by shift bits
		   0 < shift < 55 */
		p_i <<= shift;
		ep -= shift;
	} else {
		if (shift < 0L) {
			shift = -shift;
			/* shift right by shift bits
			   0 < shift < 8 */
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep += shift;
		}
	}
	if (ep < 1L) { /* denormal or 0 */
		/* shift right by 1 - ep */
		shift = 1L - ep;
		if (shift < 64L) {
			MISRA_EXCEPTION_RULE_10_3()
			xp |= (uint32_t)(((0xFFFFFFFFFFFFFFFFULL >> (64L-shift)) & p_i) != 0ULL);
			p_i >>= shift;
			ep = 0;
		} else { /* return 0 */
			MISRA_EXCEPTION_RULE_1_2c()
			MISRA_EXCEPTION_RULE_11_4()
			*(uint32_t*)&x = sp;
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
	if (ep >= 0xFFL) {
		/* return +/- inf */
		MISRA_EXCEPTION_RULE_1_2c()
		MISRA_EXCEPTION_RULE_11_4()
		*(uint32_t*)&x = sp | 0x7F800000UL;
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	/* put exponent and sign into p_i */
	p_i &= 0x007FFFFFFFFFFFFFULL;
	p_i |= ((uint64_t)sp << 32) | ((uint64_t)(uint32_t)ep << 55);
	/* round p to 24 bits */
	switch (FLT_ROUNDS) {
	case 1:  /* to nearest (round to even on tie) */
		if ((p_i & 0x0000000080000000ULL) && ((p_i & 0x0000000100000000ULL) ||
		    (p_i & 0x000000007FFFFFFFULL) || xp)) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 2:  /* towards inf */
		if (((p_i & 0x00000000FFFFFFFFULL) || xp) && !sp) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 3:  /* towards -inf */
		if (((p_i & 0x00000000FFFFFFFFULL) || xp) && sp) {
			p_i += 0x0000000100000000ULL;
		}
		break;
	case 0:  /* towards 0 */
	default: /* indeterminant */
		break;
	}
	MISRA_EXCEPTION_RULE_1_2c()
	MISRA_EXCEPTION_RULE_11_4()
	*(uint32_t*)&x = (uint32_t)(p_i >> 32);
	return x;
}
#endif /* !__has_intrinsic(__builtin_fmaf) */

#endif

#endif /* _EWL_FLOATING_POINT */

#endif /* _EWL_C99 */
#endif
