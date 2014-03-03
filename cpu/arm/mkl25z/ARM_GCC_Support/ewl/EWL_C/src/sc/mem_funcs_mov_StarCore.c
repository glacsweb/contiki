/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 
#include <stddef.h>
#include <string.h>
#include <mem_funcs.h>
#include <ewl_misra_types.h>

/*lint -e923 */

void *memmove(void * dst, const void * src, size_t n)
{
#pragma inline_call __move_mem
	__move_mem(dst, src, (uint32_t)n);
	return dst;
}

void _EWL_CDECL __move_mem(void * dst, const void * src, uint32_t n)
{
	const	char_t * p;
	char_t * q;
	int_t	 rev = ((uint32_t) src < (uint32_t) dst);
	
/* On SC3900 rely on the compiler to auto-vectorize the code */

#if !defined(_SC3900_)
	if (n >= (uint32_t)__min_bytes_for_long_copy) {
		if ((((uint32_t) dst ^ (uint32_t) src)) & 3u) {
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
		return;
	}
#endif

	if (!rev) {
		p = (const char_t *) src;
		q = (char_t *) dst;
		for (n++; --n;) {
			*q++ = *p++;
		}
	} else {
		p = (const char_t *) src + n;
		q = (char_t *) dst + n;
		for (n++; --n;) {
			*--q = *--p;
		}
	}
}
