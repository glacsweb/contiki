/* EWL
 * Copyright (c)1995-2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/04 09:09:03 $
 * $Revision: 1.10 $
 */

/**
 * \file arith.c
 */

/*
 *	Routines
 *	--------
 *		abs
 *		div
 *
 *		labs
 *		ldiv
 *
 *		__ewl_add
 *		__ewl_ladd
 *
 *		__ewl_mul
 *		__ewl_lmul
 *
 *		__ewl_div
 *		__ewl_ldiv
 *
 *		__ewl_mod
 *		__ewl_lmod
 *
 *	Notes
 *	-----
 *
 *		__ewl_add and __ewl_ladd provide int and long addition with overflow checking. If
 *		overflow occurs, a zero value is returned; otherwise, the sum replaces the
 *		value pointed to by the first argument and a nonzero value is returned.
 *
 *		__ewl_mul and __ewl_lmul provide int and long multiplication with overflow checking
 *		in a similar fashion. Note that a result of INT_MIN or LONG_MIN (whose
 *		magnitudes are 1 greater than INT_MAX and LONG_MAX respectively) is
 *		considered an overflow.
 *
 *		__ewl_div and __ewl_ldiv provide int and long division/remainder. The definition of
 *		remainder here is different from that of div and ldiv and provides for
 *		more mmathematically consistent modular arithmetic (see Knuth Vol. 1, pp.
 *		37-38).
 *
 *		__ewl_mod and __ewl_lmod return the same remainder as __ewl_div and __ewl_ldiv do, but
 *		without the quotient - just a single scalar return value.
 *
 *
 */
#include <ansi_parms.h>
#include "arith.h"
#include <limits.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

/*lint -e123 std macros abs etc... also need to nbe functions */

int_t _EWL_CDECL (abs)(int_t n)
{
	if (n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return (-n);
	} 
	return n;
}

div_t _EWL_CDECL div(int_t numerator, int_t denominator)
{
	int_t		n_sign, d_sign;
	div_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0) {
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0) {
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}

int32_t _EWL_CDECL (labs)(int32_t n)
{
	if (n < 0L)
	{
		MISRA_EXCEPTION_RULE_14_7()	
		return (-n);
	}
	return n;
}

#if (_EWL_C99 || defined( __cplusplus))
#if _EWL_LONGLONG
int64_t _EWL_CDECL (llabs)(int64_t n)
{
	if (n < 0LL)
	{
		MISRA_EXCEPTION_RULE_14_7()	
		return (-n);
	}
	return n;
}
#endif	/* _EWL_LONGLONG	*/
#endif  /* _EWL_C99 */


ldiv_t _EWL_CDECL ldiv(int32_t numerator, int32_t denominator)
{
	int32_t		n_sign, d_sign;
	ldiv_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0L)
	{
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0L)
	{
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}


#if (_EWL_C99 || defined(__cplusplus))
#if _EWL_LONGLONG
lldiv_t _EWL_CDECL lldiv(int64_t numerator, int64_t denominator)
{
	int64_t		n_sign, d_sign;
	lldiv_t	value;

	n_sign = 1;
	d_sign = 1;

	if (numerator < 0LL)
	{
		numerator = -numerator;
		n_sign    = -1;
	}

	if (denominator < 0LL)
	{
		denominator = -denominator;
		d_sign      = -1;
	}

	value.quot = (numerator / denominator) * (n_sign * d_sign);
	value.rem  = (numerator * n_sign) - (value.quot * denominator * d_sign);

	return(value);
}
#endif	/* #if _EWL_LONGLONG	*/
#endif /* _EWL_C99 */

int_t _EWL_CDECL __ewl_add(int_t * x, int_t y)
{
	int_t _x = *x;

	if (y < 0) {
		if ((_x < 0) && (y < (INT_MIN - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	} else {
		if ((_x > 0) && (y > (INT_MAX - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	}

	*x = _x + y;

	return(1);
}

int_t _EWL_CDECL __ewl_ladd(int32_t * x, int32_t y)
{
	int32_t _x = *x;

	if (y < 0L) {
		if ((_x < 0L) && (y < (LONG_MIN - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	} else {
		if ((_x > 0L) && (y > (LONG_MAX - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	}

	*x = _x + y;

	return(1);
}

#if (_EWL_LONGLONG && _EWL_C99)
int_t _EWL_CDECL __lladd(int64_t * x, int64_t y)
{
	int64_t _x = *x;

	if (y < 0LL) {
		if ((_x < 0LL) && (y < (LLONG_MIN - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	} else {
		if ((_x > 0LL) && (y > (LLONG_MAX - _x))) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	}

	*x = _x + y;

	return(1);
}
#endif	/* #if _EWL_LONGLONG	*/

int_t _EWL_CDECL __ewl_mul(int_t * x, int_t y)
{
	int_t	_x = *x;
	int_t	sign;

	MISRA_EXCEPTION_RULE_12_7b()
	sign = ((_x < 0) ^ (y < 0)) ? -1 : 1;

	if (_x < 0) {
		_x = -_x;
	}
	if ( y < 0) {
		y = - y;
	}
	if (_x > (INT_MAX / y)) {
		MISRA_EXCEPTION_RULE_14_7()		
		return(0);
	}

	*x = _x * y * sign;

	return(1);
}

int_t _EWL_CDECL __ewl_lmul(int32_t * x, int32_t y)
{
	int32_t	_x = *x;
	int_t		sign;

	MISRA_EXCEPTION_RULE_12_7b()
	sign = ((_x < 0L) ^ (y < 0L)) ? -1 : 1;

	if (_x < 0L) {_x = -_x;}
	if ( y < 0L) {y = - y;}

	if (_x > (LONG_MAX / y)) {
		MISRA_EXCEPTION_RULE_14_7()		
		return(0);
	}

	*x = _x * y * (int32_t)sign;

	return(1);
}


#if (_EWL_LONGLONG && _EWL_C99)
int_t _EWL_CDECL __llmul(int64_t * x, int64_t y)
{
	int64_t	_x = *x;
	int_t		sign;

	MISRA_EXCEPTION_RULE_12_7b()
	sign = ((_x < 0LL) ^ (y < 0LL)) ? -1 : 1;

	if (_x < 0LL) {_x = -_x;}
	if ( y < 0LL) {y = - y;}

	if (_x > (LLONG_MAX / y)) {
		MISRA_EXCEPTION_RULE_14_7()		
		return(0);
	}

	*x = _x * y * (int64_t)sign;

	return(1);
}
#endif	/* #if _EWL_LONGLONG	*/


div_t _EWL_CDECL __ewl_div(int_t x, int_t y)
{
	int_t	q, r, x_sign, y_sign, q_sign;
	div_t	result;

	x_sign = 1;
	y_sign = 1;

	if (x < 0) {
		x      = -x;
		x_sign = -1;
	}

	if (y < 0) {
		y      = -y;
		y_sign = -1;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0)) {
		q -= 1;
		r += y * y_sign;
	}

	result.quot = q;
	result.rem  = r;

	return(result);
}

ldiv_t _EWL_CDECL __ewl_ldiv(int32_t x, int32_t y)
{
	int32_t		q, r, x_sign, y_sign, q_sign;
	ldiv_t	result;

	x_sign = 1;
	y_sign = 1;

	if (x < 0L)
	{
		x      = -x;
		x_sign = -1;
	}

	if (y < 0L)
	{
		y      = -y;
		y_sign = -1;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0L))
	{
		q -= 1L;
		r += y * y_sign;
	}

	result.quot = q;
	result.rem  = r;

	return(result);
}


#if _EWL_C99
#if _EWL_LONGLONG
lldiv_t _EWL_CDECL __lldiv(int64_t x, int64_t y)
{
	int64_t		q, r, x_sign, y_sign, q_sign;
	lldiv_t	result;

	x_sign = 1;
	y_sign = 1;

	if (x < 0LL)
	{
		x      = -x;
		x_sign = -1LL;
	}

	if (y < 0LL)
	{
		y      = -y;
		y_sign = -1LL;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0LL))
	{
		q -= 1LL;
		r += y * y_sign;
	}

	result.quot = q;
	result.rem  = r;

	return(result);
}
#endif	/* #if _EWL_LONGLONG	*/
#endif  /* _EWL_C99 */

int_t _EWL_CDECL __ewl_mod(int_t x, int_t y)
{
	int_t		q, r, x_sign, y_sign, q_sign;

	x_sign = 1;
	y_sign = 1;

	if (x < 0) {
		x      = -x;
		x_sign = -1;
	}

	if (y < 0) {
		y      = -y;
		y_sign = -1;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0))
	{
		q -= 1;
		r += y * y_sign;
	}

	return(r);
}

int32_t _EWL_CDECL __ewl_lmod(int32_t x, int32_t y)
{
	int32_t		q, r, x_sign, y_sign, q_sign;

	x_sign = 1;
	y_sign = 1;

	if (x < 0L)
	{
		x      = -x;
		x_sign = -1;
	}

	if (y < 0L)
	{
		y      = -y;
		y_sign = -1;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0L))
	{
		q -= 1L;
		r += y * y_sign;
	}

	return(r);
}

#if _EWL_LONGLONG
int64_t _EWL_CDECL __llmod(int64_t x, int64_t y)
{
	int64_t		q, r, x_sign, y_sign, q_sign;

	x_sign = 1;
	y_sign = 1;

	if (x < 0LL) {
		x      = -x;
		x_sign = -1LL;
	}

	if (y < 0LL) {
		y      = -y;
		y_sign = -1LL;
	}

	q_sign = x_sign * y_sign;

	q = (x / y) * q_sign;
	r = (x * x_sign) - (q * y * y_sign);

	if (r && (q_sign < 0LL)) {
		q -= 1LL;
		r += y * y_sign;
	}

	return(r);
}
#endif	/* #if _EWL_LONGLONG	*/
