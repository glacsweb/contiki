/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/03/25 13:26:07 $
 * $Revision: 1.4 $
 */

/*
 *	
 *  First written by:  Matthew Fassiotto on June 1, 1998
 *  
 *	Routines
 *	--------
 *		
 *		__num2dec
 *      __dec2num
 *
 * Purpose:  library support routines for converting IEEE 32 and 64 bit floating point
 *           numbers to and from ansi C strings.  these routines are called by atof,strtod, 
 *           printf,scanf et al.
 *
 * Theory:   These routines are INTEL specific and can be used on any intel architecture with a
 *           fpu.  They are highly optimized in that any number will scale in two ops where at most
 *           one op is a divide. They also use the full precision of the fpu(64 significant digits)
 *           to achieve perfectly rounded double values.  This is centered around the fbstp and fbld
 *           instructions.  To keep the codesize small, the tables in the header x80...fordoubles.h
 *           are only large enough for the range of a double.  The tables can be extended for 80 bit
 *           support.           
 */
#include <ansi_parms.h>

#if _EWL_FLOATING_POINT

#include <ansi_fp.h>
#include <math.h>
#include <math_api.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#ifndef _EWL_ACCURATE_BUT_LARGE_ANSI_FP
	#define _EWL_ACCURATE_BUT_LARGE_ANSI_FP 1
#endif /* _EWL_ACCURATE_BUT_LARGE_ANSI_FP */

#if _EWL_ACCURATE_BUT_LARGE_ANSI_FP

#if !_EWL_LONGLONG
	#error _EWL_LONGLONG is required to use _EWL_ACCURATE_BUT_LARGE_ANSI_FP
	/* Either turn on _EWL_LONGLONG, or turn off _EWL_ACCURATE_BUT_LARGE_ANSI_FP */
#endif /* !_EWL_LONGLONG */

#ifndef __CWCC__
	#define _EWL_COMPILE_FOR_SPEED 1
#else
	#if __option(optimize_for_size)
		#define _EWL_COMPILE_FOR_SPEED 0
	#else
		#define _EWL_COMPILE_FOR_SPEED 1
	#endif
#endif /* __CWCC__ */

/*
	_ENCODE_NAN

	Set if you want __dec2num to be able to encode information into a nan
*/

/*
	Set _DOUBLE_BITS to either 64 to support IEEE 64 bit double
	Set _DOUBLE_BITS to either 32 to support IEEE 32 bit double
*/

/*
	Comment out _TEST once ansi_fp.c passes the tests in ansi_fp_test.c

#define _TEST
*/

#ifndef _DOUBLE_BITS
	#if DBL_MANT_DIG == 53		/* 64 bit */
		#define _DOUBLE_BITS 64
	#elif DBL_MANT_DIG == 24	/* 32 bit */
		#define _DOUBLE_BITS 32
	#else
		#error unsupported sizeof double
	#endif
#endif /* _DOUBLE_BITS */

/*
	There is an internal format and external format for the decimal structure.
	The external format is output from num2dec, and input for dec2num.
	The internal format is used everywhere else.
	The external format uses ASCII for the digits, and the decimal point
	is to the right of the least significant digit.
	The internal format uses numerical values for the digits ('\0' is 0)
	and the decimal point is to the right of the most significant digit.
*/

#if _DOUBLE_BITS == 64
	typedef unsigned long long d_int;
#elif _DOUBLE_BITS == 32
	typedef unsigned long d_int;
#else
	#error unsupported sizeof double
#endif

#ifdef _TEST

static
void __test_preconditions()
{
	char c1[sizeof(double)*CHAR_BIT == _DOUBLE_BITS];
	char c2[sizeof(double) == sizeof(d_int)];
	char c3[sizeof(unsigned long)*CHAR_BIT == 32];
	c1;
	c2;
	c3;
}

#endif /* _TEST */

/*
	Count number of consequtive low order 0 bits in double
*/
#ifndef _TEST
static
inline
#endif /* _TEST */
int __ewl_count_trailing_zero_double(double x)
{
#if _DOUBLE_BITS == 64
	_UINT64* l = (_UINT64*)&x;
	return __ewl_count_trailing_zero64(*l | 0x0010000000000000);
#elif _DOUBLE_BITS == 32
	_UINT32* l = (_UINT32*)&x;
	return __ewl_count_trailing_zero32(*l | 0x00800000);
#else
	#error __ewl_count_trailing_zero_double unsupported
#endif
}

/*
	Scan decimal mantissa starting at pos.
	Return -1 if should round down
	Return  1 if should round up
	Round towards even for half way
*/
#ifndef _TEST
static
#endif /* _TEST */
int __must_round(const decimal *d, int pos)
{
	char const *i = d->sig.text + pos;
	if (*i > 5)
		return 1;
	if (*i < 5)
		return -1;
	{    /* if there are any non-zeroes after the 5 round up */
	char const *e = d->sig.text + d->sig.length;
	for (++i; i < e; ++i)
		if (*i != 0)
			return 1;
	}
	/* exactly half, round to even */
	if (d->sig.text[pos-1] & 1)
		return 1;
	return -1;
}

/*
	Round decimal up, using the left most digits digits
*/
#ifndef _TEST
static
#endif /* _TEST */
void __dorounddecup(decimal *d, int digits)
{
	char *b = d->sig.text;
	char *i = b + digits - 1;
	while (1)
	{
		if (*i < 9)
		{
			(*i)++;
			break;
		}
		if (i == b)
		{
			*i = 1;
			d->exp++;
			break;
		}
		*i-- = 0;
	}
}

/*
	Round decimal to specified digits according to value in decimal
*/
#ifndef _TEST
static
#endif /* TEST */
void __rounddec(decimal *d, int digits)
{
	if (digits <= 0 || digits >= d->sig.length)
		return;
	{
	int r = __must_round(d, digits);
	d->sig.length = (unsigned char)digits;
	if (r < 0)
		return;
	}
	__dorounddecup(d, digits);
}

/*
	Convert d_int to decimal
*/
#ifndef _TEST
static
#endif /* _TEST */
void __ull2dec(decimal *result, d_int integer)
{
	/* assumes integer is > 0 */
	result->sgn = 0;
	result->sig.length = 0;
	for (; integer != 0; integer /= 10)
		result->sig.text[result->sig.length++] = (unsigned char)(integer % 10);
	{
	char* i = result->sig.text;
	char* j = result->sig.text + result->sig.length;
	for (; i < --j; ++i)
	{
		unsigned char t = *i;
		*i = *j;
		*j = t;
	}
	}
	result->exp = (short)(result->sig.length - 1);
}

/*
	result = x * y
*/
#ifndef _TEST
static
#endif /* _TEST */
void __timesdec(decimal *result, const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and positive */
	unsigned long accumulator = 0;
	char mantissa[2*SIGDIGLEN];
	int i = x->sig.length + y->sig.length - 1;
	char *ip = mantissa + i + 1;
	char *ep = ip;
	result->sgn = 0;
	for (; i > 0; --i)
	{
		int k = y->sig.length - 1;
		int j = i - k - 1;
		int l;
		int t;
		char const *jp, *kp;
		if (j < 0)
		{
			j = 0;
			k = i - 1;
		}
		jp = x->sig.text + j;
		kp = y->sig.text + k;
		l = k + 1;
		t = x->sig.length - j;
		if (l > t)
			l = t;
		for (; l > 0; --l, ++jp, --kp)
			accumulator += (unsigned long)(*jp) * *kp;
		*--ip = (unsigned char)(accumulator % 10);
		accumulator /= 10;
	}
	result->exp = (short)(x->exp + y->exp);
	if (accumulator > 0)
	{
		*--ip = (unsigned char)accumulator;
		result->exp++;
	}
	for (i = 0; i < SIGDIGLEN && ip < ep; ++i, ++ip)
		result->sig.text[i] = *ip;
	result->sig.length = (unsigned char)i;
	if (ip < ep && *ip >= 5)
	{
		if (*ip == 5)
		{
			char* jp = ip + 1;
			for (; jp < ep; ++jp)
				if (*jp != 0)
					goto round;
			/* exactly half way, round toward even */
			if ((*(ip - 1) & 1) == 0)
				return;
		}
	round:
		__dorounddecup(result, result->sig.length);
	}
}

/*
	The ASCII string is put into d's mantissa (after converting
	to numeric) and the exp is put into d's exponent.  If the string
	is too long to fit in the mantissa, rounding is performed.
*/
#ifndef _TEST
static
#endif /* _TEST */
void __str2dec(decimal *d, const char* s, short exp)
{
	int i;
	d->exp = exp;
	d->sgn = 0;
	for (i = 0; i < SIGDIGLEN && *s != 0;)
		d->sig.text[i++] = (unsigned char)(*s++ - '0');
	d->sig.length = (unsigned char)i;
	if (*s != 0)
	{
		if (*s < 5)
			return;
		if (*s > 5)
			goto round;
		{
		const char* p = s + 1;
		for (; *p != 0; ++p)
			if (*p != '0')
				goto round;
		if ((d->sig.text[i-1] & 1) == 0)
			return;
		}
	round:
		__dorounddecup(d, d->sig.length);
	}
}

/*
	decimal = pow(2, exp)
*/
#ifndef _TEST
static
#endif /* _TEST */
void __two_exp(decimal *result, long exp)
{
	switch (exp)
	{
#if _EWL_COMPILE_FOR_SPEED
	case -64:
		__str2dec(result, "542101086242752217003726400434970855712890625", -20);
		return;
	case -53:
		__str2dec(result, "11102230246251565404236316680908203125", -16);
		return;
	case -32:
		__str2dec(result, "23283064365386962890625", -10);
		return;
	case -16:
		__str2dec(result, "152587890625", -5);
		return;
	case -8:
		__str2dec(result, "390625", -3);
		return;
	case -7:
		__str2dec(result, "78125", -3);
		return;
	case -6:
		__str2dec(result, "15625", -2);
		return;
	case -5:
		__str2dec(result, "3125", -2);
		return;
	case -4:
		__str2dec(result, "625", -2);
		return;
	case -3:
		__str2dec(result, "125", -1);
		return;
	case -2:
		__str2dec(result, "25", -1);
		return;
#endif /* _EWL_COMPILE_FOR_SPEED */
	case -1:
		__str2dec(result, "5", -1);
		return;
	case 0:
		__str2dec(result, "1", 0);
		return;
	case 1:
		__str2dec(result, "2", 0);
		return;
#if _EWL_COMPILE_FOR_SPEED
	case 2:
		__str2dec(result, "4", 0);
		return;
	case 3:
		__str2dec(result, "8", 0);
		return;
	case 4:
		__str2dec(result, "16", 1);
		return;
	case 5:
		__str2dec(result, "32", 1);
		return;
	case 6:
		__str2dec(result, "64", 1);
		return;
	case 7:
		__str2dec(result, "128", 2);
		return;
	case 8:
		__str2dec(result, "256", 2);
		return;
#endif /* _EWL_COMPILE_FOR_SPEED */
	}
	{
	decimal x2, temp;
	__two_exp(&x2, ((long)((0x80000000UL & exp) >> 31) + exp) >> 1); /*__two_exp(&x2, exp/2); */
	__timesdec(result, &x2, &x2);
	if (exp & 1)
	{
		temp = *result;
		if (exp > 0)
			__str2dec(&x2, "2", 0);
		else
			__str2dec(&x2, "5", -1);
		__timesdec(result, &temp, &x2);
	}
	}
}

#ifndef _TEST
static
#endif /* _TEST */
int __equals_dec(const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* First check for x or y to be 0 */
	if (x->sig.text[0] == 0)
	{
		if (y->sig.text[0] == 0)
			return 1;
		return 0;
	}
	if (y->sig.text[0] == 0)
	{
		if (x->sig.text[0] == 0)
			return 1;
		return 0;
	}
	/* x and y are finite and positive */
	if (x->exp == y->exp)
	{
		int i;
		int l = x->sig.length;
		if (l > y->sig.length)
			l = y->sig.length;
		for (i = 0; i < l; ++i)
			if (x->sig.text[i] != y->sig.text[i])
				return 0;
		if (l == x->sig.length)
			x = y;
		for (; i < x->sig.length; ++i)
			if (x->sig.text[i] != 0)
				return 0;
		return 1;
	}
	return 0;
}

#ifndef _TEST
static
#endif /* _TEST */
int __less_dec(const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* First check for x or y to be 0 */
	if (x->sig.text[0] == 0)
	{
		if (y->sig.text[0] != 0)
			return 1;
		return 0;
	}
	if (y->sig.text[0] == 0)
		return 0;
	/* x and y are finite and positive */
	if (x->exp == y->exp)
	{
		int i;
		int l = x->sig.length;
		if (l > y->sig.length)
			l = y->sig.length;
		for (i = 0; i < l; ++i)
		{
			if (x->sig.text[i] < y->sig.text[i])
				return 1;
			if (y->sig.text[i] < x->sig.text[i])
				return 0;
		}
		if (l == x->sig.length)
		{
			for (; i < y->sig.length; ++i)
				if (y->sig.text[i] != 0)
					return 1;
		}
		return 0;
	}
	return x->exp < y->exp;
}

#ifndef _TEST
static
#endif /* _TEST */
void __minus_dec(decimal *z, const decimal *x, const decimal *y)
{
	/* Assume x and y are finite and non-negative */
	/* Also assume y < x */
	int zlen, dexp;
	char *ib, *i, *ie;
	char const *jb, *j, *jn;
	*z = *x;
	if (y->sig.text[0] == 0)
		return;
	zlen = z->sig.length;
	if (zlen < y->sig.length)
		zlen = y->sig.length;
	dexp = z->exp - y->exp;
	zlen += dexp;
	if (zlen > SIGDIGLEN)
		zlen = SIGDIGLEN;
	while (z->sig.length < zlen)
		z->sig.text[z->sig.length++] = 0;
	ib = z->sig.text;
	i = ib + zlen;
	if (y->sig.length + dexp < zlen)
		i = ib + (y->sig.length + dexp);
	jb = y->sig.text;
	j = jb + (i - ib - dexp);
	jn = j;
	while (i > ib && j > jb)
	{
		--i;
		--j;
		if (*i < *j)
		{
			char *k = i - 1;
			while (*k == 0)
				--k;
			while (k != i)
			{
				--*k;
				*++k += 10;
			}
		}
		*i -= *j;
	}
	/* round */
	if (jn - jb < y->sig.length)
	{
		int round_down = 0;
		if (*jn < 5)
			round_down = 1;
		else if (*jn == 5)
		{
			char const *je = y->sig.text + y->sig.length;
			for (j = jn+1; j < je; ++j)
				if (*j != 0)
					goto done;
			i = ib + (jn - jb) + dexp - 1;
			if (*i & 1)
				round_down = 1;
		}
		if (round_down)
		{
			if (*i < 1)
			{
				char *k = i - 1;
				while (*k == 0)
					--k;
				while (k != i)
				{
					--*k;
					*++k += 10;
				}
			}
			*i -= 1;
		}
	}
done:
	/* normalize */
	for (i = ib; *i == 0; ++i)
		{}
	if (i > ib)
	{
		unsigned char dl = (unsigned char)(i - ib);
		z->exp -= dl;
		ie = ib + z->sig.length;
		for (; i < ie; ++i, ++ib)
			*ib = *i;
		z->sig.length -= dl;
	}
	/* trim */
	ib = z->sig.text;
	for (i = ib + z->sig.length; i > ib; )
	{
		--i;
		if (*i != 0)
			break;
	}
	z->sig.length = (unsigned char)(i - ib + 1);
}

#ifndef _TEST
static
#endif /* _TEST */
void __num2dec_internal(decimal *d, double x)
{
	char sgn = (char)(signbit(x) != 0);
	/* Special cases */
	if (x == 0.0)
	{
		d->sgn = sgn;
		d->exp = 0;
		d->sig.length = 1;
		d->sig.text[0] = 0;
		return;
	}
#if _EWL_C99						/*- mm 030218 -*/
	if (!isfinite(x))
	{
		d->sgn = sgn;
		d->exp = 0;
		d->sig.length = 1;
		d->sig.text[0] = (unsigned char)(isnan(x) ? 'N' : 'I');
		return;
	}
#endif /* _EWL_C99 */				/*- mm 030218 -*/
	/* Convert x to decimal */
	if (sgn)
		x = -x;
	{
	int exp;
	double frac = frexp(x, &exp);
	long num_bits_extract = DBL_MANT_DIG - __ewl_count_trailing_zero_double(frac);
	decimal int_d, pow2_d;
	__two_exp(&pow2_d, exp-num_bits_extract);
	__ull2dec(&int_d, (d_int)ldexp(frac, (int)num_bits_extract));
	__timesdec(d, &int_d, &pow2_d);
	d->sgn = sgn;
	}
}

void __num2dec(const decform *f, double x, decimal *d)
{
	short digits = f->digits;
	int i;
	__num2dec_internal(d, x);
	if (d->sig.text[0] > 9)
		return;
	/* Convert to external format */
	if (digits > SIGDIGLEN)
		digits = SIGDIGLEN;
	/* round length to digits */
	__rounddec(d, digits);
	/* pad length with 0 to digits */
	while (d->sig.length < digits)
		d->sig.text[d->sig.length++] = 0;
	/* Move decimal point from the right of digit 1 to the right of the last digit */
	d->exp -= d->sig.length - 1;
	/* Convert to ASCII */
	for (i = 0; i < d->sig.length; ++i)
		d->sig.text[i] += '0';
}

double __dec2num(const decimal *d)
{
	if (d->sig.length <= 0)
		return copysign(0.0, d->sgn == 0 ? 1.0 : -1.0);
	switch (d->sig.text[0])
	{
	case '0':
		return copysign(0.0, d->sgn == 0 ? 1.0 : -1.0);
	case 'I':
		return copysign((double)INFINITY, d->sgn == 0 ? 1.0 : -1.0);
	case 'N':
		{
		double result;
		d_int *ll = (d_int*)&result;
	#if _DOUBLE_BITS == 64
		*ll = 0x7FF0000000000000ULL;
		if (d->sgn)
			*ll |= 0x8000000000000000ULL;
	#ifndef _ENCODE_NAN
		*ll |= 0x0008000000000000ULL;
	#else
		if (d->sig.length == 1)
			*ll |= 0x0008000000000000ULL;
		else
		{
		#if _EWL_LITTLE_ENDIAN
			unsigned char* p = (unsigned char*)&result + 6;
		#else
			unsigned char* p = (unsigned char*)&result + 1;
		#endif /* _EWL_LITTLE_ENDIAN */
			int placed_non_zero = 0;
			int low = 1;
			int i;
			int e = d->sig.length;
			if (e > 14)
				e = 14;
			for (i = 1; i < e; ++i)
			{
				unsigned char c = d->sig.text[i];
				if (isdigit(c))
					c -= '0';
				else
					c = (unsigned char)(tolower(c) - 'a' + 10);
				if (c != 0)
					placed_non_zero = 1;
				if (low)
				#if _EWL_LITTLE_ENDIAN
					*p-- |= c;
				#else
					*p++ |= c;
				#endif /* _EWL_LITTLE_ENDIAN */
				else
					*p = (unsigned char)(c << 4);
				low = !low;
			}
			if (!placed_non_zero)
				*ll |= 0x0008000000000000;
		}
	#endif /* _ENCODE_NAN */
	#elif _DOUBLE_BITS == 32
		*ll = 0x7F800000;
		if (d->sgn)
			*ll |= 0x80000000;
	#ifndef _ENCODE_NAN
		*ll |= 0x00400000;
	#else
		if (d->sig.length == 1)
			*ll |= 0x00400000;
		else
		{
		#if _EWL_LITTLE_ENDIAN
			unsigned char* p = (unsigned char*)&result + 2;
		#else
			unsigned char* p = (unsigned char*)&result + 1;
		#endif /* _EWL_LITTLE_ENDIAN */
			int placed_non_zero = 0;
			int low = 0;
			int i;
			int e = d->sig.length;
			if (e > 7)
				e = 7;
			for (i = 1; i < e; ++i)
			{
				unsigned char c = d->sig.text[i];
				if (isdigit(c))
					c -= '0';
				else
					c = (unsigned char)(tolower(c) - 'a' + 10);
				if (c != 0)
					placed_non_zero = 1;
				if (low)
				#if _EWL_LITTLE_ENDIAN
					*p-- |= c;
				#else
					*p++ |= c;
				#endif /* _EWL_LITTLE_ENDIAN */
				else
					*p |= (unsigned char)(c << 4);
				low = !low;
			}
			if (!placed_non_zero)
				*ll |= 0x00400000;
		}
	#endif /* _ENCODE_NAN */
	#else
		#error __dec2num not supported
	#endif
		return result;
		}
	}
	{
	static double pow_10[8] = {1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8};
	/* Store *d in dec in internal format */
	decimal dec = *d;
	char *i = dec.sig.text;
	char *e = i + dec.sig.length;
	double first_guess;
	int exponent;
	for (; i < e; ++i)
		*i -= '0';
	dec.exp += dec.sig.length - 1;
	exponent = dec.exp;
	{ /* Check for numbers greater than the half-way point between DBL_MAX and INFINITY*/
	decimal max;
#if _DOUBLE_BITS == 64
	__str2dec(&max, "179769313486231580793728714053034151", 308);
#elif _DOUBLE_BITS == 32
	__str2dec(&max, "340282356779733661637539395458142568", 38);
#else
	#error __dec2num not supported
#endif
	if (__less_dec(&max, &dec))
		return copysign((double)INFINITY, d->sgn == 0 ? 1.0 : -1.0);
	}
	/* make first guess for the value of dec */
	i = dec.sig.text;
	first_guess = *i++;
	while (i < e)
	{
		unsigned long ival = 0;
		int j;
		double temp1, temp2;
		int ndig = (int)(e - i) % 8;
		if (ndig == 0)
			ndig = 8;
		for (j = 0; j < ndig; ++j, ++i)
			ival = ival * 10 + *i;
		temp1 = first_guess * pow_10[ndig-1];
		temp2 = temp1 + ival;
		if (ival != 0 && temp1 == temp2)
			break;
		first_guess = temp2;
		exponent -= ndig;
	}
	if (exponent < 0)
		first_guess /= pow(5.0, -exponent);
	else
		first_guess *= pow(5.0, exponent);
	first_guess = ldexp(first_guess, exponent);
	if (isinf(first_guess))
		first_guess = DBL_MAX;
	{
	/* convert first guess back to dec form and see if you get the same result */
	decimal feedback1, feedback2, difflow, diffhigh;
	double next_guess;
	d_int *ull = (d_int*)&next_guess;
	int guessed_low = 0;
	__num2dec_internal(&feedback1, first_guess);
	if (__equals_dec(&feedback1, &dec))
		goto done;
	if (__less_dec(&feedback1, &dec))
		guessed_low = 1;
	next_guess = first_guess;
	while (1)
	{
		if (guessed_low)
		{
			++*ull;
			if (isinf(next_guess))
				goto done;
		}
		else
			--*ull;
		__num2dec_internal(&feedback2, next_guess);
		if (guessed_low && !__less_dec(&feedback2, &dec))
			break;
		else if (!guessed_low && !__less_dec(&dec, &feedback2))
		{
			difflow = feedback1;
			feedback1 = feedback2;
			feedback2 =  difflow;
			{
			double temp = first_guess;
			first_guess = next_guess;
			next_guess = temp;
			}
			break;
		}
		feedback1 = feedback2;
		first_guess = next_guess;
	}
	__minus_dec(&difflow, &dec, &feedback1);
	__minus_dec(&diffhigh, &feedback2, &dec);
	if (__equals_dec(&difflow, &diffhigh))
	{
		/* break tie with round to even */
		if (*(d_int*)&first_guess & 1)
			first_guess = next_guess;
	}
	else if (!__less_dec(&difflow, &diffhigh))
		first_guess = next_guess;
	}
done:
	if (dec.sgn)
		first_guess = -first_guess;
	return first_guess;
	}
}

#else

static const double bit_values[] =
{
	1.0E+001,
	1.0E+002,
	1.0E+004,
	1.0E+008,
	1.0E+016,
	1.0E+032,
	1.0E+064,
	1.0E+128,
	1.0E+256
};

#define max_bits	511				/* (2**n)-1 where n is the number of elements in bit_values */

static const double digit_values[] =
{
	1.0E+01,
	1.0E+02,
	1.0E+03,
	1.0E+04,
	1.0E+05,
	1.0E+06,
	1.0E+07,
	1.0E+08
};

extern _INT32 __double_nan[];		/* hh 971202  added extern, defined in float.c*/
#define Inf	HUGE_VAL				/*  HUGE_VAL is the double encoding of infinity */
#define NaN	(* (double *) __double_nan )

void __num2dec(const decform *f, double x, decimal *d)
{
	int		exp2;
	long	exp10;
	int		digits = f->digits;
	
/*
 *	Don't try to extract any more significant digits than are there.
 */
	
	if (digits > DBL_DIG + 1)
		digits = DBL_DIG + 1;
	
/*
 *	We first deal with the special cases of zeroes, infinities and NaNs
 */
	
	d->sgn = 0;
	d->exp = 0;	

	if (__UHI(x) & 0x80000000UL)
	{
		d->sgn = 1;
		__UHI(x) &= 0x7fffffffUL;
	}

	if (x == 0.0)
	{
		d->sig.text[0] = '0';
		d->sig.length = 1;
		return;
	}
	
	if (!isfinite(x))
	{
		d->sig.text[0] = isnan(x) ? 'N' : 'I';
		d->sig.length = 1;
		return;
	}
	
	d->sig.length = 0;
	
/*
 *	Next, we use frexp() to separate x into a fraction f and an exponent e. The
 *	fraction is a value between 0.5 and 1.0 such that f * (2**e) == x. This is
 *	basically how x is stored internally. All we're interested in is the
 *	exponent to approximate the factor of 10 by which to scale x to get rid of
 *	its exponent.
 */

    if (sizeof(x) == 4)
    	frexpf(x, &exp2);
    else
       	frexp(x, &exp2);
	
/*
 *	We convert the base-2 exponent to a base-10 exponent by multiplying it by
 *	the base-10 logarithm of 2 (approximately .30102999566398). Rather than
 *	invoke the overhead of conversion to floating point and back, we instead
 *	multiply by 301029 and then divide by 1000000. The largest double exponent
 *	is 1024; 301029 * 1024 == 308253696, which can easily be represented by a
 *	long int.
 */
	
	exp10 = (exp2 * 301029) / 1000000;
	
/*
 *	Now we scale x by exp10. The easy way to do this would be simply to
 *	multiply or divide by 10, exp10 times. However, as noted above, the largest
 *	binary exponent is 1024. It's decimal equivalent is 1024 log 2 ≈≈ 308.
 *	That's potentially a lot of multiplies/divides. Alternatively, we can scan
 *	the decimal exponent bitwise and, for each bit 2**i, scale by a constant
 *	10**(2**i) if that bit is nonzero. This results in at most nine multiplies
 *	or divides.
 *
 *	Note that division usually consumes more cycles than multiplication.
 *	Therefore, we avoid doing multiple division operations by multiplying the
 *	scale factors together and then dividing only once.
 */
	
	{
		const double *	p = bit_values;
		int				n = exp10;
		
		if (n < 0)
		{
			for (n = -n; n; p++, n >>= 1)
				if (n & 1)
					x *= *p;
		}
		else if (n > 0)
		{
			double f = 1.0;
			
			for (; n; p++, n >>= 1)
				if (n & 1)
					f *= *p;
			
			x /= f;
		}
	}
	
/*
 *	Now we scale up or down as necessary to ensure that x is >= 0.1 and < 1.0.
 *	This puts all the digits to the right of the decimal point.
 */
	
	while (x >= 1.0)
	{
		x *= 0.1;
		++exp10;
	}
	
	while (x < 0.1)
	{
		x *= 10.0;
		--exp10;
	}
	
/*
 *	Now we pick off decimal digits from left to right up to eight at a time. We
 *	do this by shifting digits to the left of the decimal point and converting
 *	to long int to extract the shifted digits. We then decode the long int in
 *	the usual way.
 */
	
	{
		char *	p = d->sig.text;
		int				i;
		
		while (digits)
		{
			int		n = digits;
			long	long_x;
			
			if (n > 8)
				n = 8;
			
			digits -= n;
			
			exp10 -= n;
			
			d->sig.length += n;
			
			p += n;
			
			x *= digit_values[n-1];
			
			long_x = (long) x;
			
			x -= (double) long_x;
			
			for (i = n + 1; --i;)
			{
				*--p = (long_x % 10) + '0';
				long_x /= 10;
			}
			
			p += n;
		}
	
/*
 *	We fill up any remaining requested digits with zeroes, but no more
 *	than fits in a decimal record.
 */
		
		digits = f->digits;
		
		if (digits > SIGDIGLEN)
			digits = SIGDIGLEN;
		
		digits -= d->sig.length;
		
		if (digits > 0)
		{
			for (i = digits + 1; --i;)
				*p++ = '0';
			
			d->sig.length += digits;
			
			exp10 -= digits;
		}
	}
	
/*
 *	Finally, don't forget to drop off the exponent...
 */
	
	d->exp = exp10;
}

double __dec2num(const decimal *d)
{
	double		x   = 0.0;
	int			sgn = d->sgn;
	long		exp = d->exp;
	int			len = d->sig.length;
	
/*
 *	Handle special cases
 */
	
	{
		char	c = *d->sig.text;
		
		if (len < 1 || c == '0')
			return(0.0);
		
		if (c == 'I')
			return(sgn ? -Inf : Inf);
		
		if (c == 'N')
			return(NaN);
	}
	
/*
 *	Toss insignificant digits
 */
	
	if (len > DBL_DIG + 1)
	{
		exp += len - (DBL_DIG + 1);
		len = DBL_DIG + 1;
	}
	
/*
 *	We're going to accumulate digits eight at a time (enough to fit into a long int).
 *	If the number of digits isn't evenly divisible by eight, the stragglers are on
 *	the left as far as we are concerned.
 */

	{
		int				digits = len % 8;
		double			f;
		const char *	p = d->sig.text;
		
		if (!digits)
			digits = 8;
	
/*
 *	Once we're done we're going to shift the decimal point from the far right to the
 *	immediate right of the most significant digit. We make some adjustments here toward
 *	that end.
 */
		
		exp += len - 1;
		f    = pow(10.0, (len-1));
		
/*
 *	OK, let's do it...
 */
		
		while (digits)
		{
			int		i;
			long	long_x;
			
			for (i = digits + 1, long_x = 0; --i;)
				long_x = (long_x * 10) + (*p++ - '0');
			
			x = (x * 1e+08) + (double) long_x;
			
			if (!(len -= digits))
				break;
			
			digits = 8;
		}
		
		x /= f;
	}
	
/*
 *	We raise 10 to the exp power, making sure we don't overflow along the
 *	way. Then we multiply or divide x by the result, also guarding against
 *	overflow/underflow.
 */
	
	{
		const double *	p = bit_values;
		double			f = 1.0;
		long			n = labs(exp);
		
		if (n > max_bits)
			if (exp < 0)
				return(0.0);
			else
				return(sgn ? -Inf : Inf);
		
		f = 1.0;
		
		for (; n; p++, n >>= 1)
			if (n & 1)
				if (f > DBL_MAX / *p)
					return(sgn ? -Inf : Inf);
				else
					f *= *p;
		
		if (exp < 0)
		{
			if (x < DBL_MIN * f)
				return(0.0);
			else
				x /= f;
		}
		else if (exp > 0)
		{
			if (x > DBL_MAX / f)
				return(sgn ? -Inf : Inf);
			else
				x *= f;
		}
	}
	
	return(sgn ? -x : x);
}

#endif /* _EWL_ACCURATE_BUT_LARGE_ANSI_FP */

#endif /* _EWL_FLOATING_POINT */

/*******************************************************/ 
#if 0
#include <ansi_fp.h>
#include <math.h>
#include <fenv.h>
#include <x80_powers_of_ten_fordoubles.h>

static const long*  small_pow=(long*)&powers_of_ten[0];
static const long*  big_pow=(long*)&big_powers_of_ten[0];

//	the variable "direction" is passed on the stack
static __declspec(naked)  void __cdecl __scalenum(int)
{
    asm
    {
	    cmp 	eax,0
	    jge 	positive_exp
	    xor 	dword ptr [esp+4],0x00000001
	    neg 	eax				; eax guaranteed > 0 
positive_exp: 
	    fld1					; dummy stack element for the case where
								; we skip load_big_pow
	    mov 	ecx,eax			; ecx guaranteed > 0  
	    mov 	edx,eax			; edx will be the sum of eax and ecx in load_big_pow                 
	    cmp 	ecx,32			; 32 is the number of elements in the
								; table containing the small powers
								; of ten. this number will vary according to
								; the optimal table size(varies according to precision
								; of number). for 80 bit numberes the small table has 64 elements
	    jl  	load_small_pow                       
	    and 	eax,0xffffffe0	; this value shifted right 5(divided by 32) = index -1 for big_pow 
	    and 	ecx,0x0000001f	;  0 <= ecx < 32  and note 10^(eax+ecx)= scale factor
load_big_pow:
	    fstp 	st				; we do not need 1 in st anymore
								; checking for really big exponents where we bail here and
								; return inf
	    cmp 	eax,400
	    jl 		not_humungous
	    fld 	tbyte ptr[big_powers_of_ten + 110] 
	    fstp 	st(1)
	    jmp 	finished
not_humungous:                                
	    sub  	edx,eax			; edx=remainder of exp/32(note edx >= eax)
	    shr  	eax,5    
	    dec  	eax				; we overestimate big pow and do the opposite
								; operation with little pow(i.e. div vs. mul)
	    imul 	eax,10
	    mov  	edx, big_pow
	    add 	edx,eax
	    fld 	tbyte ptr[edx]              
load_small_pow:
	    mov 	edx,small_pow
	    imul 	ecx,10
	    add 	edx,ecx
	    fld 	tbyte ptr[edx]	; order is important and must be the same at all times!!
	                           	; st=little, st(1)=bigpower of 10, st(2)=number itself                          
	    fmul                   	; multiply big and small power of 10 to get scale factor
	    cmp 	dword ptr [esp+4],0
	    jne 	divide_res
	    fmul
	    jmp 	finished   
divide_res:
	    fdivp  	st(1),st
finished:    
		ret		4				; remove direction
  }	
/* common code ends here */
}

_EWL_IMP_EXP_C void _EWL_CDECL __num2dec(const  decform * /* f */, double x, decimal *d)
{    
      short NEAREST_FULLPREC=0X037F;
      float seventeen=17.0F;
      short TEMP;
      double TEN_TO_SEVENTEEN=1e17;
      double TEN=10.0;	
      const register unsigned char* tmp=&(d->sig.text[0]);
      const int* exp_tmp=(int*)(&(d->exp));
      int direction;
      
      d->sgn= (char)(signbit(x)>>31); 
	
	switch(fpclassify(x))
    {
        case FP_NAN:
         d->sig.text[0] = 'N';
         d->sig.length=(char)1;
         return;  
         break;
        
        case FP_INFINITE:
         d->sig.text[0] = 'I';
         d->sig.length=(char)1;
         return; 
         break;
        
        case FP_ZERO:
         d->sig.text[0] = '0';
         d->sig.length=(char)1;
         return; 
         break;
        
        case FP_NORMAL:	
        case FP_SUBNORMAL:
        direction=1;
 
        asm
	    {
          fstcw 	TEMP			; backup control word and reload after calculation
	      fldcw 	NEAREST_FULLPREC
	      fld 		x
	      fabs					; the sign of x is in d->sgn already,so sign of number is irrelevant now
	      fld 		st				; need x later to multiply by 10^exp10
	      fxtract				; st=base 2 exponent st(1)=sig
	      fxch
	      fstp 		st(1)			; significand not needed
	      fldlg2				; log10(2)
          fmul					; st=base 10 exponent=exp10
	      fsub 		seventeen		; exp of 10 to  scale number to be btwn. [10e^17,10e18)
	      mov 		eax,d           ; 
	      fistp		decimal.exp[eax]; rounded to nearest exponent produces value in 
	      movsx		eax, decimal.exp[eax]
	      push		direction
          call 		__scalenum
          fld		TEN_TO_SEVENTEEN
          fcomp		st(1)  
          fstsw		ax
          sahf
          jbe 		start_for_real
          fmul 		TEN
          mov		eax,d                
          dec		decimal.exp[eax]
start_for_real:          
          sub		esp,14			; 4 bytes for address of length
          mov		eax, tmp		; sig.text[0]                              
          fbstp		tbyte ptr[esp+4]; will round number to get all integer digits exact
          
          ; EJS 041008: in rare cases (only one known so far: 1e153) this can overflow, 
          ; so hack it if it does
          cmp		byte ptr [esp+4+8], 0xff
          jne		not_overflow
          
          ;	set to 9999 9999 9999 99
          mov		dword ptr[eax], 0x39393939
          mov		dword ptr[eax+4], 0x39393939
          mov		dword ptr[eax+8], 0x39393939
          mov		dword ptr[eax+12], 0x39393939
          mov		word ptr[eax+16], 0x3939
          jmp		done
          
not_overflow:
          mov		ecx,9	  
store_bytes:
          dec   	ecx
          mov   	dl,byte ptr[esp+4+ecx]	; start at most significant byte
          mov   	dh,dl                
          shr   	dl,4
          and   	dx,0x0f0f
          or    	dx,0x3030				; the first 4 bits of an ASCII digit
          mov   	word ptr[eax],dx
          add   	eax,2					; eax is a byte pointer(tmp)    
          jcxz 		done
          jmp 		store_bytes
done:
          add 		esp,14
          fldcw 	TEMP
		}/* end of .asm routine */
	} /* end of switch */
	
    d->sig.length=18;
}

_EWL_IMP_EXP_C double _EWL_CDECL __dec2num(const decimal *d)
{
  unsigned char tmp,p[10]; /* 80 bitpacked decimal array */
                           /* bit 80=sign 79-73=reserved ; bits 1-72 are digits */
  double ret_val=0.0;
  int i ;
  short temp, oldcw;
   unsigned char *p2=p;
   int direction;
   
  for(i=0;i < 9; i++)
  {
  /* shift the bits of the odd elements left 4 then add to preceding even element */
   tmp=d->sig.text[2*i]<<4;
   p[9-(i+1)]=(d->sig.text[2*i+1]&0x0F) | tmp ;
  }
  p[9]=d->sgn;
  i=d->exp - (18 - d->sig.length) ;
 direction =0;
  		
	asm							/*- pmk 010402 -*/
	{
		fstcw 	oldcw				; save FPU_register 				
		mov 	ax, oldcw			; tfr values to usable space
		or 		ax, 0x0300			; gives full precision
		mov 	temp, ax
		fldcw 	temp 
		mov  	edx,p2
		fbld 	tbyte ptr[edx]
		mov 	eax,i				; load base 10 exponent
		push	direction
		call 	__scalenum
		fstp 	ret_val
		fldcw 	oldcw				; return_original precision
	}

	return ret_val;
}  
#endif
/* Change record: 
 * pmk 010402 dec2num function did not save & restore current values of FPU register
 * ejs 010403 added "register" keyword for new compiler, fixed bug accessing var, formatting 
 * ejs 010726 turn off K6 calls for naked assembly function taking args on FP stack
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * ejs 020125 Turn off K6 calls pragmas, add _EWL_IMP_EXP_C
 * cc  020521 Changed the double_t to double
 * ejs 030217 Move "direction" into a local for thread safety
 * ejs 041004 Fix bug dealing with numbers near 1e152 (MTWX14205)
 * ejs 041008 Fix it a better way that doesn't break everything else (MTWX14205)
 */