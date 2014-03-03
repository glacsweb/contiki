/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 15:28:50 $
 * $Revision: 1.9 $
 */

/*
 *	Routines
 *	--------
 *		__copy_longs_aligned
 *		__copy_longs_rev_aligned
 *		__copy_longs_unaligned
 *		__copy_longs_rev_unaligned
 *
 *		__fill_mem
 *
 *	Implementation
 *	--------------
 *		Specialized routines to copy memory by transferring longs and using
 *		unrolled loops whenever possible.
 *
 *		There are two pairs of routines.
 *
 *		One pair handles the cases where both the source and destination are
 *		mutually aligned, in which case it suffices to transfer bytes until they
 *		are long-aligned, then transfer longs, then finish up transferring any
 *		excess bytes.
 *
 *		The other pair handles the cases where the source and destination are not
 *		mutually aligned. The handling is similar but requires repeatedly
 *		assembling a destination long from two source longs.
 *
 *		Each pair consists of one routine to transfer bytes from first byte to last
 *		and another to transfer bytes from last byte to first. The latter should be
 *		used whenever the address of the source is less than the address of the
 *		destination in case they overlap each other.
 *
 *		There are also two routines here that effectively duplicate memcpy and
 *		memmove, so that these routines may be used without bringing in the entire
 *		StdLib.
 *
 *		Oh, and let's not forget __fill_mem, which fills memory a long at a time
 *		and in an unrolled loop whenever possible.
 *
 *
 */

#include <mem_funcs.h>

#if _GCCPORT_
	#include <mem_funcs_gcc.c>
#else

#include <ewl_misra_types.h>
MISRA_ALLOW_POINTER_CASTS()
/*lint -e63, -e826 string pointer manipulations */

#pragma ANSI_strict off

#ifdef __COLDFIRE__
#pragma optimize_for_size off
#endif

#if !(defined(__MOTO__) || defined(__MIPS__))
#define cps	((uchar_t *) src)
#define cpd	((uchar_t * _EWL_RESTRICT) dst)
#define lps	((uint32_t *) src)
#define lpd	((uint32_t * _EWL_RESTRICT) dst)
#endif

MISRA_EXCEPTION_RULE_19_7()
#if !defined(__POWERPC__)
#define deref_auto_inc(p)	*(p)++
#else
#define deref_auto_inc(p)	*++(p)
#endif
_MISRA_RESTORE()

#if __dest_os != __starcore

void _EWL_CDECL __copy_mem(void *_EWL_RESTRICT dst, const void * src, uint32_t n)
{
	const char_t * p;
	char_t *_EWL_RESTRICT q;

	if (n >= (uint32_t)_EWL_LONG_COPY_MIN) {
		MISRA_EXCEPTION_RULE_11_3()
		if ((((uint_t) dst ^ (uint_t) src)) & 3u) {
			__copy_longs_unaligned(dst, src, n);
		} else {
			__copy_longs_aligned(dst, src, n);
		}
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

#if !defined(__POWERPC__)
	p = (const char_t *) src;
	q = (char_t *) dst;
	for (n++; --n;)
	{
		*q++ = *p++;
	}
#else

	p = (const char_t *) src - 1;
	q = (char_t *) dst - 1;
	for (n++; --n;)
	{
		*++q = *++p;
	}
#endif
}

void _EWL_CDECL __move_mem(void * dst, const void * src, uint32_t n)
{
	const	char_t * p;
	char_t *q;
	MISRA_EXCEPTION_RULE_11_3()
	int_t	rev = ((uint32_t) src < (uint32_t) dst);

	if (n >= (uint32_t)_EWL_LONG_COPY_MIN) {
		MISRA_EXCEPTION_RULE_11_3()
		if ((((uint_t) dst ^ (uint_t) src)) & 3u) {
			if (!rev) {
				__copy_longs_unaligned(dst, src, n);
			} else {
				__copy_longs_rev_unaligned(dst, src, n);
			}
		} else {
			if (!rev) {
				__copy_longs_aligned(dst, src, n);
			} else {
				__copy_longs_rev_aligned(dst, src, n);
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}

	if (!rev) {
#if !defined(__POWERPC__)
		p = (const char_t *) src;
		q = (char_t *) dst;
		for (n++; --n;) {
			*q++ = *p++;
		}
#else
		p = (const char_t *) src - 1;
		q = (char_t *) dst - 1;
		for (n++; --n;) {
			*++q = *++p;
		}
#endif

	} else {
		p = (const char_t *) src + n;
		q = (char_t *) dst + n;
		for (n++; --n;) {
			*--q = *--p;
		}
	}
}

#endif

void _EWL_CDECL __copy_longs_aligned(void *_EWL_RESTRICT dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__))
	uchar_t *	cps	= ((uchar_t  *) src);
	uchar_t *	cpd	= ((uchar_t  *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif

	uint32_t	i;

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_11_3()
	i = (uint32_t)(- (int32_t) dst) & 3UL;

#if defined(__POWERPC__)
	cps = ((uchar_t *) src) - 1;
	cpd = ((uchar_t *) dst) - 1;
#endif

	if (i) {
		n -= i;
		do { /*lint -e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		}
		while (--i);
	}

#if defined(__POWERPC__)
	lps = ((uint32_t *) (cps + 1)) - 1;
	lpd = ((uint32_t *) (cpd + 1)) - 1;
#endif

#ifdef __STARCORE__
	for (i=n>>2; i; i--) {
		deref_auto_inc(lpd) = deref_auto_inc(lps);
	}
#else
	i = n >> 5;

	if (i) {
		do {
			  /*lint --e{52} lvalue */
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
			deref_auto_inc(lpd) = deref_auto_inc(lps);
		} while (--i);
	}

	i = (n & 31UL) >> 2;

	if (i) {
		do {
		    /*lint --e{52} lvalue */
			deref_auto_inc(lpd) = deref_auto_inc(lps);
		} while (--i);
	}
#endif

#if defined(__POWERPC__)
	cps = ((uchar_t *) (lps + 1)) - 1;
	cpd = ((uchar_t *) (lpd + 1)) - 1;
#endif

	n &= 3UL;

	if (n) {
		do {
		    /*lint --e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--n);
	}

	return;
}


void _EWL_CDECL __copy_longs_rev_aligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif
	uint32_t			i;

   	/*lint --e{63} lvalue */
   	cps = ((uchar_t *) src) + n;
	cpd = ((uchar_t *) dst) + n;

	MISRA_EXCEPTION_RULE_11_3()
	i = ((uint32_t) cpd) & 3UL;

	if (i) {
		n -= i;
		do {
   			/*lint --e{52} lvalue */
			*--cpd = *--cps;
		}while (--i);
	}

#ifdef __STARCORE__
	for (i=n>>2; i; i--) {
			*--lpd = *--lps;
	}
#else
	i = n >> 5;

	if (i) {
		do {
   			/*lint --e{52} lvalue */
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
			*--lpd = *--lps;
		} while (--i);
	}

	i = (n & 31UL) >> 2;

	if (i) {
		do {
   			/*lint --e{52} lvalue */
			*--lpd = *--lps;
		}while (--i);
	}
#endif

	n &= 3UL;

	if (n) {
		do {
   			/*lint --e{52} lvalue */
			*--cpd = *--cps;
		} while (--n);
	}

	return;
}

void _EWL_CDECL __copy_longs_unaligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif


	uint32_t	i, v1, v2;
	uint_t	src_offset, left_shift, right_shift;

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_11_3()
	i = (uint32_t)(- (int32_t) dst) & 3UL;

#if defined(__POWERPC__)
	cps = ((uchar_t *) src) - 1;
	cpd = ((uchar_t *) dst) - 1;
#endif

	if (i) {
		n -= i;
		do {
   			/*lint --e{52} lvalue */
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--i);
	}

	MISRA_EXCEPTION_RULE_11_3()
#if !defined(__POWERPC__)
	src_offset = ((uint_t) cps) & 3U;
#else
	src_offset = ((uint_t) (cps + 1)) & 3U;
#endif

	left_shift  = src_offset << 3;
	right_shift = 32U - left_shift;

   	/*lint --e{63} lvalue */
	cps -= src_offset;

#if defined(__POWERPC__)
	lps = ((uint32_t *) (cps + 1)) - 1;
	lpd = ((uint32_t *) (cpd + 1)) - 1;
#endif

	i = n >> 3;

   	/*lint --e{52} lvalue */
	v1 = deref_auto_inc(lps);

	do {
   		/*lint --e{52} lvalue */
#if !_EWL_LITTLE_ENDIAN
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
		v1                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v2 << left_shift) | (v1 >> right_shift);
#else
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 >> left_shift) | (v2 << right_shift);
		v1                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v2 >> left_shift) | (v1 << right_shift);
#endif
	} while (--i);

	if (n & 4UL) {
#if !_EWL_LITTLE_ENDIAN
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
#else
		v2                  = deref_auto_inc(lps);
		deref_auto_inc(lpd) = (v1 >> left_shift) | (v2 << right_shift);
#endif
	}

#if defined(__POWERPC__)
	cps = ((uchar_t *) (lps + 1)) - 1;
	cpd = ((uchar_t *) (lpd + 1)) - 1;
#endif

	n &= 3UL;

	if (n) {
		cps -= 4U - src_offset;
		do {
			deref_auto_inc(cpd) = deref_auto_inc(cps);
		} while (--n);
	}

	return;
}

void _EWL_CDECL __copy_longs_rev_unaligned(void * dst, const void * src, uint32_t n)
{
#if( defined(__MOTO__)  || defined(__MIPS__))
	uchar_t *	cps	= ((uchar_t *) src);
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lps	= ((uint32_t *) src);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif

	uint32_t	i, v1, v2;
	uint_t	src_offset, left_shift, right_shift;

   	/*lint --e{52} lvalue */
   	/*lint --e{63} lvalue */
	cps = ((uchar_t *) src) + n;
	cpd = ((uchar_t *) dst) + n;

	MISRA_EXCEPTION_RULE_11_3()
	i = ((uint32_t) cpd) & 3UL;

	if (i) {
		n -= i;
		do {
			*--cpd = *--cps;
		} while (--i);
	}

	MISRA_EXCEPTION_RULE_11_3()
	src_offset = ((uint_t) cps) & 3U;

	left_shift  = src_offset << 3U;
	right_shift = 32U - left_shift;

	cps += 4U - src_offset;

	i = n >> 3;

	v1 = *--lps;

	do {
#if !_EWL_LITTLE_ENDIAN
		v2     = *--lps;
		*--lpd = (v2 << left_shift) | (v1 >> right_shift);
		v1     = *--lps;
		*--lpd = (v1 << left_shift) | (v2 >> right_shift);
#else
		v2     = *--lps;
		*--lpd = (v2 >> left_shift) | (v1 << right_shift);
		v1     = *--lps;
		*--lpd = (v1 >> left_shift) | (v2 << right_shift);
#endif
	} while (--i);

	if (n & 4UL)
	{
#if !_EWL_LITTLE_ENDIAN
		v2     = *--lps;
		*--lpd = (v2 << left_shift) | (v1 >> right_shift);
#else
		v2     = *--lps;
		*--lpd = (v2 >> left_shift) | (v1 >> right_shift);
#endif
	}

	n &= 3UL;

	if (n) {
		cps += src_offset;
		do {
			*--cpd = *--cps;
		} while (--n);
	}
}


#if ((!defined(__PPC_EABI__)) && !defined(__m56800__)) && (__dest_os != __starcore)
void _EWL_CDECL __fill_mem(void * dst, int_t val, uint32_t n)
{
#if defined(__MOTO__) || defined(__MIPS__)
	uchar_t *	cpd	= ((uchar_t *) dst);
	uint32_t *	lpd	= ((uint32_t *) dst);
#endif

	uint32_t v = (uchar_t) val;
	uint32_t i;

#if defined(__POWERPC__)
	cpd = ((uchar_t *) dst) - 1;
#endif

	if (n >= 32UL) {
		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_11_3()
#if defined(__POWERPC__)
		i = (~ (uint32_t) dst) & 3UL;
#else
		i = (uint32_t)(-(int32_t) dst) & 3UL;
#endif
		if (i) {
			n -= i;

			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(cpd) = (uchar_t)v;
			} while (--i);
		}

		if (v) {
			v |= (v << 24) | (v << 16) | (v <<  8);
		}

#if defined(__POWERPC__)
		lpd = ((uint32_t *) (cpd + 1)) - 1;
#elif defined(__MOTO__) || defined(__MIPS__)
		lpd = (uint32_t *) cpd;
#endif

#ifdef __STARCORE__
		for (i=n>>2; i; i--) {
			deref_auto_inc(lpd) = v;
		}
#else
		i = n >> 5;

		if (i) {
			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
			}
			while (--i);
		}

		i = (n & 31UL) >> 2;

		if (i) {
			do {
   				/*lint --e{52} lvalue */
				deref_auto_inc(lpd) = v;
			} while (--i);
		}
#endif

#if defined(__POWERPC__)
		cpd = ((uchar_t *) (lpd + 1)) - 1;
#elif defined(__MOTO__) || defined(__MIPS__)
		cpd = (uchar_t *) lpd;
#endif

		n &= 3UL;
	}

	if (n) {
		do {
   			/*lint --e{52} lvalue */
			deref_auto_inc(cpd) = (uchar_t) v;
		} while (--n);
	}
}


#endif /* !__PPC_EABI__ */


#if defined(__m56800__) && (__dest_os != __starcore)
void __fill_mem(void * dst, int val, uint32_t n)
{

	uint32_t			v = (uchar_t) val;
	uint32_t			i;

	if (n >= 32) {
		if (v) {
			v |= v << 16;
		}

		i = n >> 4;
		if (i) {
			do
			{
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
				deref_auto_inc(lpd) = v;
			} while (--i);
		}
		i = (n & 15)>>1;

		if (i) {
			do {
				deref_auto_inc(lpd) = v;
			} while (--i);
		}
		n &= 1;
	}

	if (n) {
		do {
			deref_auto_inc(cpd) = v;
		} while (--n);
	}
}

#endif /* defined(__m56800__) */

#endif

#ifdef __STARCORE__
/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef cps
#undef cpd
#undef lps
#undef lpd
#undef deref_auto_inc
/*lint +e960 MISRA EXCEPTION RULE 19.6 undef for build */
#endif
