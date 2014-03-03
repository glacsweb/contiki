/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <stddef.h>
#include <string.h>
#include <mem_funcs.h>

#include <ewl_misra_types.h>

/*lint -e52, -e63, -e950, -e826, -e923 ptr cast lvalues */

#define deref_auto_inc(p)	*(p)++
#define cps	((uchar_t * restrict) src)
#define cpd	((uchar_t * restrict) dst)
#define lps	((uint32_t * restrict) src)
#define lpd	((uint32_t * restrict) dst)

#if _SC3850_ || _SC3450_ || _MSC8156_

#include <prototype.h>

void __copy_mem_dst_unaligned(void *restrict dst, const void *restrict src, uint32_t n);
void __copy_mem_dst_aligned(void *restrict dst, const void *restrict src, uint32_t n);
/*
void *memcpy(void *restrict dst, const void *restrict src, size_t n)
{
    char *p=(char*)dst+64-(int)((char*)dst)%64;
    int max=n-(int)((char*)dst)%64;
    while(max>=64){
      _dmalloc(p);
      p+=64;
      max-=64;
    }
#pragma inline_call __copy_mem
	__copy_mem(dst, src, n);
	return dst;
}*/

void *memcpy(void *restrict dst, const void *restrict src, size_t n)
{
	void* temp = dst;

	if (!n) {
		return dst;
	}
	MISRA_EXCEPTION_RULE_11_3()
	if ((uint32_t)cpd%64u) {
		uint32_t unaligned=64u-((uint32_t)cpd%64u);
		if(unaligned>n) {
			unaligned=n;
		}
#pragma inline_call __copy_mem_dst_unaligned
		__copy_mem_dst_unaligned(cpd,cps,unaligned);
		cpd+=unaligned;
		cps+=unaligned;
		n-=(size_t)unaligned;
	}

	while (n>=256u) {
		_dmalloc(cpd);
		_dmalloc(cpd+64);
		_dmalloc(cpd+128);
		_dmalloc(cpd+192);
#pragma inline_call __copy_mem_dst_aligned
		__copy_mem_dst_aligned(cpd, cps, 256u);
		cpd+=256;
		cps+=256;
		n-=256u;
	}

	if (n) {
#pragma inline_call __copy_mem_dst_unaligned
		__copy_mem_dst_unaligned(cpd,cps,(uint32_t)n);
	}

	return temp;
}

void __copy_mem_dst_unaligned(void *restrict dst, const void *restrict src, uint32_t n)
{
	do {
		deref_auto_inc(cpd) = deref_auto_inc(cps);
	} while (--n);
}

void __copy_mem_dst_aligned(void *restrict dst, const void *restrict src, uint32_t n)
{
	int_t i;
	MISRA_EXCEPTION_RULE_11_3()
	if ((((uint_t) dst ^ (uint_t) src)) & 3u) {
		uint_t	src_offset, left_shift, right_shift;
		uint32_t	v1, v2;

		src_offset = ((uint_t) cps) & 3u;
		left_shift  = src_offset << 3;
		right_shift = 32u - left_shift;
		cps -= src_offset;
		lpd = (uint32_t *) cpd;
		lps = (uint32_t *) cps;
		MISRA_EXCEPTION_RULE_10_3()
		i = (int_t)(n >> 3);

		v1 = deref_auto_inc(lps);

		do {
			#ifdef BIG_ENDIAN
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
	} else {
		MISRA_EXCEPTION_RULE_10_3()
		for (i=(int_t)(n>>2); i; i--) {
			deref_auto_inc(lpd) = deref_auto_inc(lps);
		}
	}
	return;
}

#else

void *memcpy(void *restrict dst, const void *restrict src, size_t n)
{
#pragma inline_call __copy_mem
	__copy_mem(dst, src, n);
	return dst;
}

#endif

/* local copy for inlining original from mem_funcs.c */
void _EWL_CDECL __copy_mem(void *restrict dst, const void *restrict src, uint32_t n)
{
	const	char_t *restrict p;
			char_t *restrict q;

/* On SC3900 rely on the compiler to auto-vectorize the code */

#if !defined(_SC3900_)
	if (n >= (uint32_t)__min_bytes_for_long_copy) {
		if ((((uint32_t) dst ^ (uint32_t) src)) & 3u) {
			__copy_longs_unaligned(dst, src, n);
		} else {
			__copy_longs_aligned(dst, src, n);
		}

		return;
	}
#endif

	p = (const char_t *) src;
	q = (char_t *) dst;
	for (n++; --n;) {
		*q++ = *p++;
	}
}

/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef deref_auto_inc
#undef cps
#undef cpd
#undef lps
#undef lpd
/*lint +e960 MISRA EXCEPTION RULE 19.6 undef for build */
