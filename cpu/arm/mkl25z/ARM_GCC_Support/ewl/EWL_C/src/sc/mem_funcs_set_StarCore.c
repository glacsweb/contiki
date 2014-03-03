/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <stddef.h>
#include <string.h>
#include <mem_funcs.h>

#include <ewl_misra_types.h>

#define deref_auto_inc(p)	*(p)++
#define cpd	((uchar_t * restrict) dst)
#define lpd	((uint32_t * restrict) dst)

/*lint -e950 no ANSI checks */
/*lint -e52, -e63, -e826, -e923 ptr casts */

#if _SC3850_ || _SC3450_ || _MSC8156_

#include <prototype.h>

void __fill_mem_unaligned(void* dst, int_t val, uint32_t n);
void __fill_256_aligned(__attribute__((aligned(8)))void* dst, int_t val);

void *memset(void * dst, int_t val, size_t n)
{
	void *tmp=dst;

	if(!n) {
		return dst;
	}

	if((uint32_t)cpd%64u) {
		uint32_t unaligned=64u-((uint32_t)cpd%64u);
		if(unaligned>n) {
			unaligned=n;
		}

		#pragma inline_call __fill_mem_unaligned
		__fill_mem_unaligned(cpd,val,unaligned);

		cpd+=unaligned;
		n-=(size_t)unaligned;
	}

	while (n>=256u) {
		_dmalloc(cpd);
		_dmalloc(cpd+64);
		_dmalloc(cpd+128);
		_dmalloc(cpd+192);

		{
		__attribute__((aligned(8)))uint32_t* x = (__attribute__((aligned(8)))uint32_t* )cpd;

		#pragma inline_call __fill_256_aligned
		__fill_256_aligned(x,val);
		}
		cpd+=256;
		n-=256u;
	}

	if(n) {
		#pragma inline_call __fill_mem_unaligned
		__fill_mem_unaligned(cpd,val,(uint32_t)n);
	}

	return tmp;
}

void __fill_256_aligned(__attribute__((aligned(8)))void* restrict dst, int_t val)
{
	uint32_t v = (uchar_t) val;
	__attribute__((aligned(8)))uint32_t* x = dst;

	int_t i;
	if (v) {
		v |= (v << 24) | (v << 16) | (v <<  8);
	}
	for (i=0; i< 64; i++) {
		x[i] = v;
	}
}

void __fill_mem_unaligned(void* dst, int_t val, uint32_t n)
{
	do {
		deref_auto_inc(cpd) = (uchar_t)val;
	} while (--n);
}

#else

void *memset(void * dst, int_t val, size_t n)
{
#pragma inline_call __fill_mem
	__fill_mem(dst, val, n);
	return dst;
}

#endif

/* local copy for inlining */
void _EWL_CDECL __fill_mem(void * dst, int_t val, uint32_t n)
{
	uint32_t v = (uchar_t) val;
	uint32_t i;

	if (n >= 32u) {
		MISRA_EXCEPTION_RULE_10_3()
		i = ((uint32_t)-(int32_t) dst) & 3u;
		if (i) {
			n -= i;
			do {
				deref_auto_inc(cpd) = (uchar_t)v;
			} while (--i);
		}

		if (v) {
			v |= (v << 24) | (v << 16) | (v << 8);
		}

		for (i=n>>2; i; i--) {
			deref_auto_inc(lpd) = v;
		}

		n &= 3u;
	}

	if (n) {
		do {
			deref_auto_inc(cpd) = (uchar_t)v;
		} while (--n);
	}

	return;
}

/*lint -e960 MISRA EXCEPTION RULE 19.6 undef for build */
#undef deref_auto_inc
#undef cpd
#undef lpd
/*lint +e960 MISRA EXCEPTION RULE 19.6 undef for build */
