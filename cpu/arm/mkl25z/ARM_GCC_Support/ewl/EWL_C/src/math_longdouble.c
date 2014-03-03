/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:10 $
 * $Revision: 1.15 $
 */

#include <ansi_parms.h>
#include <fenv.h>


#if _GCCPORT_
	#include <math_longdouble_gcc.c>
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
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

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

#if !_EWL_USES_SUN_MATH_LIB

int_t ilogbl(ldbl_t x)
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
		return (FP_ILOGB0);
	}
	if (isinf(x)) {	/*	was: isinlf(x) */
		return (INT_MAX);
	}
	if (isnan(x)) { /*	was: isnanl(x) */
		return (FP_ILOGBNAN);
	}
	return ((int_t)logbl(x));
}
#endif /* _EWL_USES_SUN_MATH_LIB */

#if !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL erfl(ldbl_t x) _EWL_CANT_THROW
{
	return((ldbl_t)erf((f64_t)x));
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL erfcl(ldbl_t x) _EWL_CANT_THROW
{
	#if _EWL_C99
		if(x >= HUGE_VAL) {		/* range error may occur */
			MISRA_EXCEPTION_RULE_20_5()
			errno=ERANGE;
		}	/* x too large */
	#endif
	return((ldbl_t)(1.0 - erf((f64_t)x)));
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL exp2l(ldbl_t x) _EWL_CANT_THROW
{
	ldbl_t x_ln2 = x * 0.6931471805599453094172321;

	/* exp sets the range error */
	x_ln2 = expl(x_ln2);
	return x_ln2;
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL lgammal(ldbl_t x) _EWL_CANT_THROW
{
	ldbl_t z = logl(tgammal(x));	
	
	#if _EWL_C99
		MISRA_EXCEPTION_RULE_13_7()
		if ((uint_t)math_errhandling & (uint_t)MATH_ERRNO) {		
			if (isfinite(x)) {
				MISRA_EXCEPTION_RULE_13_3()
				if ((floor(x) == x) && (x<=0.0)) {		/* range error may occur */
					MISRA_EXCEPTION_RULE_20_5()
					errno=ERANGE;
				}    /* lgamma pole; x negative integer or zero */
				else if (x >= HUGE_VAL) {		/* range error may occur */
					MISRA_EXCEPTION_RULE_20_5()
					errno=ERANGE;
				}	/* lgamma overflow; x too large */
			}
		}
	#endif	
	return(z);
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL nanl(const char_t * str) _EWL_CANT_THROW
{
	#pragma unused(str)
	MISRA_QUIET_UNUSED_ARGS()
/*extern  int32_t __extended_nan[];
ldbl_t NaN = * (ldbl_t *) __extended_nan; */
	return (ldbl_t)NAN;
}

_EWL_IMP_EXP_C ldbl_t _EWL_MATH_CDECL tgammal(ldbl_t x) _EWL_CANT_THROW
{
	ldbl_t coefs[] = {
		+1.0000000000000000L,
		+0.5772156649915329L,
		-0.6558780715202538L,
		-0.0420026350340952L,
		+0.1665386113822915L,
		-0.0421977345555443L,
		-0.0096219715278770L,
		+0.0072189432466630L,
		-0.0011651675918591L,
		-0.0002152416741149L,
		+0.0001280502823882L,
		-0.0000201348547807L,
		-0.0000012504934821L,
		+0.0000011330272320L,
		-0.0000002056338417L,
		+0.0000000061160960L,
		+0.0000000050020075L,
		-0.0000000011812746L,
		+0.0000000001043427L,
		+0.0000000000077823L,
		-0.0000000000036968L,
		+0.0000000000005100L,
		-0.0000000000000206L,
		-0.0000000000000054L,
		+0.0000000000000014L,
		+0.0000000000000001L
	};

	ldbl_t argx = x;
	ldbl_t dv = 0.0L;
	uint_t  index;

#if _EWL_C99
		if (isfinite(x))
		{
			MISRA_EXCEPTION_RULE_13_3()
			if((floorl(x) == x) && (x <= 0.0L)) {			/* domain error may occur - negative integer OR zero */
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
		if (x == 0.0L) {
			/* raise divide by zero exception */
			if (signbit(x)) {
				MISRA_EXCEPTION_RULE_14_7()
				return -INFINITY;
			} else {
				MISRA_EXCEPTION_RULE_14_7()
				return INFINITY;
			}
		}
		if (x < 0.0L) {
			ldbl_t fracpart, intpart;
			
			fracpart = modfl(x, &intpart);
			if (fracpart == 0.0L) {
				/* raised invalid exception above */
				MISRA_EXCEPTION_RULE_14_7()
				return NAN;
			}
		}
		MISRA_EXCEPTION_RULE_13_3()
		if (x == -INFINITY) {
			/* raise invalid exception */
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
			MISRA_EXCEPTION_RULE_14_7()
			return NAN;
		}
		MISRA_EXCEPTION_RULE_13_3()
		if (x == INFINITY) {
			MISRA_EXCEPTION_RULE_20_5()
			errno = ERANGE;
			MISRA_EXCEPTION_RULE_14_7()
			return INFINITY;
		}
#endif /* _EWL_IEC_559_ADDITIONS */
#endif /* _EWL_C99 */	

	for (index = 0u; index < (sizeof(coefs)/sizeof(ldbl_t)); index++) {
		dv += argx * coefs[index];
		argx *= x;
	}

	return(1.0L/dv);
}

#endif /* !_EWL_ONLY_SINGLE_PRECISION_MATH_LIB */
#endif /* _EWL_FLOATING_POINT */
#endif /* _EWL_C99 */

#endif
