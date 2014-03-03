/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 15:08:18 $
 * $Revision: 1.8 $
 */

/*
 *	Routines
 *	--------
 *		memcpy
 *		memcpy_s
 *		memmove
 *		memmove_s
 *		memset
 *		memchr
 *		memcmp
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <mem_funcs.h>

#if defined(__INTEL__)

	#include <x86/mem.x86.h>

#else  /* all others. . */

#if __dest_os != __starcore

#if !_EMBEDDED_WARRIOR_MEMFUNCS

#if !defined(__PPC_EABI__)

void * (memcpy)(register void * _EWL_RESTRICT dst, register const void * _EWL_RESTRICT src, register size_t n)
{
		const char_t * p=	(char_t *)src;
		char_t  * q		=	(char_t *)dst;

		#if !defined(__COLDFIRE__) \
		    && !defined(__arm) \
		    && !defined(__m56800__) \
		    && !defined(__m56800E__)

			if (n >= _EWL_LONG_COPY_MIN) {
				if ((((int_t) dst ^ (int_t) src)) & 3) {
					__copy_longs_unaligned(dst, src, n);
				} else {
					__copy_longs_aligned(dst, src, n);
				}

				return(dst);
			}
		#endif

		#if !defined(__POWERPC__)

			/*for (p = src, q = dst, n++; --n;)*/
			for(n++;--n;) {
				*q++ = *p++;
			}

		#else

			for (p = (const char_t *) src - 1, q = (char_t) dst - 1, n++; --n;) {
				*++q = *++p;
			}

		#endif

	return(dst);
}

#endif /*  (!__PPC_EABI__) */

void * (memmove)(void * dst, const void * src, size_t n)
{
	const	char_t * p;
			char_t * q;
			MISRA_EXCEPTION_RULE_11_3()
			int_t	rev = ((uint32_t) src < (uint32_t) dst);

		#if !defined(__MIPS__) \
		    && !defined(__SH__) \
		    && !defined(__MCORE__) \
		    && !defined(__COLDFIRE__) \
		    && !defined(__arm) \
		    && !defined(__m56800__) \
		    && !defined(__m56800E__)

			if (n >= (size_t)_EWL_LONG_COPY_MIN) {
				MISRA_EXCEPTION_RULE_11_3()
				MISRA_EXCEPTION_RULE_12_7()
				if ((((int_t) dst ^ (int_t) src)) & 3) {
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
				return(dst);
			}

		#endif

		if (!rev) {

			#if !defined(__POWERPC__)

				p = src; q = dst;
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

	return(dst);
}

#else

void * (memcpy) (void * dst, const void * src, size_t len)
{
	__copy_mem(dst, src, len);
	return dst;
}

void *(memmove)(void * dst, const void * src, size_t n)
{
	__move_mem(dst, src, n);
	return dst;
}

#endif /* _EMBEDDED_WARRIOR_MEMFUNCS */

#if !defined(__PPC_EABI__) && (__dest_os != __starcore)

void * memset(void * dst, int_t val, size_t n)
{
	__fill_mem(dst, val, (uint32_t)n);

	return(dst);
}

#endif /* !defined(__PPC_EABI__) */

#endif /* dest_os == __starcore */

void * memchr(const void * src, int_t val, size_t n)
{
const uchar_t *	p;
uint32_t	v = ((uint32_t)val & 0xffu);

	++n;

#if !defined(__POWERPC__)

	for (p = (uchar_t *) src; --n;) {
		if ((uint32_t)*p++ == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) --p);
		}
	}

#else

	for (p = (uchar_t *) src - 1; --n;) {
		MISRA_EXCEPTION_RULE_10_3()
		if ((uint32_t)(*++p & 0xffu) == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) p);
		}
	}

#endif

	return(NULL);
}

void * __memrchr(const void * src, int_t val, size_t n)
{
	const uchar_t *	p;

#if !defined(__POWERPC__)

	uchar_t	v = (uchar_t)val;

	p = (uchar_t *)src + n;
	for (n++; --n;) {
		if (*--p == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) p);
		}
	}

#else

	uint32_t	v = ((uint32_t)val & 0xffu);

	p = (uchar_t *) src + n;
	for (n++; --n;) {
		if (*--p == v) {
			MISRA_EXCEPTION_RULE_14_7()
			return((void *) p);
		}
	}

#endif

	return(NULL);
}

int_t memcmp(const void * src1, const void * src2, size_t n)
{
	const uchar_t * p1, * p2;

	#if !defined(__POWERPC__)

		p1 = (const uchar_t *) src1;
		p2 = (const uchar_t *) src2;
		for (n++; --n;) {
			MISRA_EXCEPTION_RULE_1_2b()
			if (*p1++ != *p2++) {
				if (*--p1 < *--p2) {
					MISRA_EXCEPTION_RULE_14_7()
					return -1;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return 1;
				}
			}
		}

	#else

		p1 = (const uchar_t *) src1 - 1;
		p2 = (const uchar_t *) src2 - 1;
		for (n++; --n;) {
			MISRA_EXCEPTION_RULE_1_2b()
			if (*++p1 != *++p2) {
				if (*p1 < *p2) {
					MISRA_EXCEPTION_RULE_14_7()
					return -1;
				} else {
					MISRA_EXCEPTION_RULE_14_7()
					return 1;
				}
			}
		}

	#endif

	return(0);
}

#endif /* __INTEL__ */

errno_t (memcpy_s)(register void * _EWL_RESTRICT dst, register rsize_t dst_max, register const void * _EWL_RESTRICT src, register rsize_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX)))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n)
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	(memcpy)(dst, src, n);
	return ENOERR;
}

errno_t (memmove_s)(void * dst, rsize_t dst_max, const void * src, rsize_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > RSIZE_MAX) || (n > RSIZE_MAX)))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n)
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	(memmove)(dst, src, n);
	return ENOERR;
}
