/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 15:08:18 $
 * $Revision: 1.8 $
 */

/*
 *	Routines
 *	--------
 *		wmemcpy
 *		wmemcpy_s
 *		wmemmove
 *		wmemmove_s
 *		wmemset
 *		wmemchr
 *		wmemcmp
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

wchar_t * _EWL_CDECL (wmemcpy)(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src, size_t n)
{
	return memcpy(dst, src, n * sizeof(wchar_t));
}

errno_t _EWL_CDECL (wmemcpy_s)(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src, rsize_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max * sizeof(wchar_t));
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	memcpy(dst, src, n * sizeof(wchar_t));
	return ENOERR;
}

wchar_t * _EWL_CDECL (wmemmove)(wchar_t * dst, const wchar_t * src, size_t n)
{
	return memmove(dst, src, n * sizeof(wchar_t));
}

errno_t _EWL_CDECL (wmemmove_s)(wchar_t * dst, size_t dst_max, const wchar_t * src, size_t n)
{
	if (((dst == NULL) || (src == NULL)) || ((dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max < n) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		memset(dst, 0, dst_max * sizeof(wchar_t));
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	memmove(dst, src, n * sizeof(wchar_t));
	return ENOERR;
}

wchar_t * _EWL_CDECL (wmemset)(wchar_t * dst, wchar_t val, size_t n)
{
	wchar_t *save = dst;

	while (n) {
		*dst++ = val;
		n--;
	}

	return(save);
}

wchar_t * _EWL_CDECL (wmemchr)(const wchar_t * src, wchar_t val, size_t n)
{
	while (n) {
		if (*src == val) {
			MISRA_EXCEPTION_RULE_11_4()
			MISRA_EXCEPTION_RULE_14_7()
			return (wchar_t *) src;
		}
		src++;
		n--;
	}

	return(NULL);
}

int_t _EWL_CDECL (wmemcmp)(const wchar_t * src1, const wchar_t * src2, size_t n)
{
	uint_t diff = 0u;

	while (n) {
		MISRA_EXCEPTION_RULE_10_1()
		diff = (*src1 - *src2);
		if (diff) {
			break;
		}
		src1++;
		src2++;
		n--;
	}

	return (int_t)diff;
}

#endif /* #if _EWL_WIDE_CHAR */

