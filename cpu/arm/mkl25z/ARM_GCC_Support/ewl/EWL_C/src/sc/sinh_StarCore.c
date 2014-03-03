/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

/*
 *	sinh returns the hyperbolic sine of its double-precision argument.
 *	A series is used for arguments smaller in magnitude than 1.
 *	The exponential function is used for arguments
 *	greater in magnitude than 1.
 *
 *	cosh returns the hyperbolic cosine of its double-precision argument.
 *	cosh is computed from the exponential function for
 *	all arguments.
 *
 *	Returns ERANGE error and value HUGE (or -HUGE for sinh of
 *	negative argument) if the correct result would overflow.
 *
 *	Algorithm and coefficients from Cody and Waite (1980).
 */

#include <float.h>
#include <math.h>
#include <sc/trans.h>
#include <errno.h>

#ifdef _CW_X_SLLD_
#include <prototype.h>
#endif

#if defined(_REENTRANT) && _REENTRANT
#include "context_local_data.h"
#endif

#define X_MAX	(LN_MAXDOUBLE + M_LN2)

#ifdef _CW_X_SLLD_

#define LNV		LL_to_Db(0x3fe62e6000000000LL)
#define V2M1	LL_to_Db(0x3eed0112eb0202d6LL)

#else

#define LNV		0.6931610107421875
#define V2M1	0.13830277879601902638e-4

#endif /* _CW_X_SLLD_ */

static f64_t sinh_exc( f64_t x, f64_t y, int_t sinhflag)
{
	int_t neg = ((x < 0.0) && sinhflag); /* sinh of negative argument */

	/* result is still representable */
	if ( y < X_MAX ) {
		x = exp(y - LNV);
		x += V2M1 * x;
		return (neg ? -x : x);
	}

#if _EWL_C99
	MISRA_EXCEPTION_RULE_13_7()
	if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = ERANGE;
	}
#else
	MISRA_EXCEPTION_RULE_20_5()
	errno = ERANGE;
#endif
	return ( neg ? -HUGE : HUGE );
}


f64_t sinh( f64_t x )
{
	f64_t y = fabs(x);

	if ( y <= 1.0 ) {
#ifdef _CW_X_SLLD_
		static f64_t p[4],q[4];

		p[0] = LL_to_Db(0xbfe944e7b86fc81bLL);
		p[1] = LL_to_Db(0xc06478416385be4aLL);
		p[2] = LL_to_Db(0xc0c695c2b6941490LL);
		p[3] = LL_to_Db(0xc115791356533419LL);

		q[0] = LL_to_Db(0x3ff0000000000000LL);
		q[1] = LL_to_Db(0xc0715bc381c97ff2LL);
		q[2] = LL_to_Db(0x40e1a85723b65ec7LL);
		q[3] = LL_to_Db(0xc1401ace80be6713LL);
#else
		static f64_t p[] = {
			-0.78966127417357099479e0,
			-0.16375798202630751372e3,
			-0.11563521196851768270e5,
			-0.35181283430177117881e6,
		}, q[] = {
			 1.0,
			-0.27773523119650701667e3,
			 0.36162723109421836460e5,
			-0.21108770058106271242e7,
		};
#endif
		/* for efficiency and to prevent underflow */
		if ( y < X_EPS ) {
			return (x);
		}

		y = x * x;
		return (x + (((x * y) * _POLY3(y, p))/_POLY3(y, q)));
	}

	/* exp(x) would overflow */
	if ( y > LN_MAXDOUBLE ) {
		return (sinh_exc(x, y, 1));
	}

	x = exp(x);
	return (0.5 * (x - (1.0/x)));
}


f64_t cosh( f64_t x )
{
	f64_t y = fabs(x);

	/* exp(x) would overflow */
	if ( y > LN_MAXDOUBLE ) {
		return (sinh_exc(x, y, 0));
	}

	x = exp(y);
	return (0.5 * (x + (1.0/x)));
}
