/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/28 07:52:10 $
 * $Revision: 1.10 $
 */

/*
 *	Routines
 *	--------
 *		wcslen
 *		wcsnlen_s
 *
 *		wcscpy
 *		wcscpy_s
 *		wcscat
 *		wcscmp
 *
 *		wcsncpy
 *		wcsncpy_s
 *		wcsncat
 *		wcsncat_s
 *		wcsncmp
 *
 *		wcscoll
 *		wcsxfrm
 *
 *		wcschr
 *		wcsrchr
 *		wcsstr
 *
 *		wcsspn
 *		wcscspn
 *		wcspbrk
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#if _EWL_WIDE_CHAR

#include <ewl_misra_types.h>
#include <errno.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

/*
	This routine returns the length of the wide character string str
*/
size_t _EWL_CDECL (wcslen)(const wchar_t * str)
{
	size_t	len = (size_t)-1;

#if !defined(__POWERPC__)

	do {
		len++;
	} while (*str++);

#else

	wchar_t * p = (wchar_t *) str - 1;

	do {
		len++;
	} while (*++p);

#endif

	return(len);
}

size_t _EWL_CDECL (wcsnlen_s)(const wchar_t * str, size_t maxsize)
{
	wchar_t *end;
	wchar_t *p;
	size_t	len = (size_t)-1;

	if ((maxsize == 0U) || (str == NULL)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}

	end = (wchar_t *) &(str[maxsize - 1U]);
	p = (wchar_t *) str - 1;

	MISRA_EXCEPTION_RULE_12_4()
	MISRA_EXCEPTION_RULE_17_3()
	do {
		len++;
	} while ((end > p) && *++p);

	return len;
}

wchar_t * _EWL_CDECL (wcscpy)(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src)
{
#if !defined(__POWERPC__)

	const	wchar_t * p = src;
				wchar_t * q = dst;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*q++ = *p++)) {}

#else

	const	wchar_t * p = (wchar_t *) src - 1;
			wchar_t * q = (wchar_t *) dst - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*++q = *++p)) {}

#endif

	return(dst);
}

errno_t _EWL_CDECL (wcscpy_s)(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src)
{
	if ((dst == NULL) || (src == NULL) || (dst_max == 0U) || (dst_max > (rsize_t)RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > 0U) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= wcsnlen_s(src, dst_max))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcscpy(dst, src);
	return ENOERR;
}


wchar_t * _EWL_CDECL wcsncpy(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src, size_t n)
{
#if !defined(__POWERPC__)

	const	wchar_t * p = src;
			wchar_t * q = dst;

	n++;

	while (--n) {
		MISRA_EXCEPTION_RULE_13_1()
		if (!(*q++ = *p++)) {
			while (--n) {
				*q++ = (wchar_t)0;
			}
			break;
		}
	}

#else

	const	wchar_t * p		= (const wchar_t *) src - 1;
			wchar_t * q		= (wchar_t *) dst - 1;

	n++;

	while (--n) {
		MISRA_EXCEPTION_RULE_13_1()
		if (!(*++q = *++p)) {
			while (--n) {
				*++q = (wchar_t)0;
			}
			break;
		}
	}

#endif

	return(dst);
}

errno_t _EWL_CDECL wcsncpy_s(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;

	if ((dst_max == 0U) || (dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > (rsize_t)0) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= n) {
		for (i = 0u; i < dst_max; i++) {
			if (src[i] == (wchar_t)0) {
				MISRA_EXCEPTION_RULE_1_2a()
				(void)wcscpy(dst, src);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcsncpy(dst, src, n);
	dst[n] = (wchar_t)0;
	return ENOERR;
}


wchar_t * _EWL_CDECL wcscat(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src)
{
#if !defined(__POWERPC__)

	const	wchar_t * p = src;
			wchar_t * q = dst;

	while (*q++) {}
	q--;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*q++ = *p++)) {}

#else

	const	wchar_t * p = (wchar_t *) src - 1;
			wchar_t * q = (wchar_t *) dst - 1;

	while (*++q) {}
	q--;

	MISRA_EXCEPTION_RULE_13_1()
	while ((*++q = *++p)) {}

#endif

	return(dst);
}

errno_t (wcscat_s)(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src)
{
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max == (rsize_t)0) || (dst_max > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > (rsize_t)0) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - wcsnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max <= wcsnlen_s(src, dst_max)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcscat(dst, src);
	return ENOERR;
}


wchar_t * _EWL_CDECL wcsncat(wchar_t * _EWL_RESTRICT dst, const wchar_t * _EWL_RESTRICT src, size_t n)
{
#if !defined(__POWERPC__)

	const	wchar_t * p = src;
			wchar_t * q = dst;

	while (*q++) {}

	q--; n++;

	while (--n) {
		MISRA_EXCEPTION_RULE_13_1()
		if (!(*q++ = *p++)) {
			q--;
			break;
		}
	}

	*q = (wchar_t)0;

#else

	const	wchar_t * p = (wchar_t *) src - 1;
			wchar_t * q = (wchar_t *) dst - 1;

	while (*++q) {}

	q--; n++;

	while (--n) {
		MISRA_EXCEPTION_RULE_13_1()
		if (!(*++q = *++p)) {
			q--;
			break;
		}
	}

	*++q = (wchar_t)0;

#endif

	return(dst);
}

errno_t _EWL_CDECL wcsncat_s(wchar_t * _EWL_RESTRICT dst, rsize_t dst_max, const wchar_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		if ((dst != NULL) && (dst_max > 0U) && (dst_max <= (rsize_t)RSIZE_MAX)) {
			dst[0] = (wchar_t)0;
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - wcsnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max < n) {
		for (i = 0u; i < dst_cat_max; i++) {
			if (src[i] == (wchar_t)0) {
				MISRA_EXCEPTION_RULE_1_2a()
				(void)wcsncat(dst, src, n);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = (wchar_t)0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	MISRA_EXCEPTION_RULE_1_2a()
	(void)wcsncat(dst, src, n);
	if (dst_cat_max != n) {
		dst[(dst_max-dst_cat_max)+n] = (wchar_t)0;
	}
	return ENOERR;
}


int_t _EWL_CDECL wcscmp(const wchar_t * str1, const wchar_t * str2)
{
#if !defined(__POWERPC__)

	const	wchar_t * p1 = (wchar_t *) str1;
	const	wchar_t * p2 = (wchar_t *) str2;
				wchar_t		c1, c2;

	MISRA_EXCEPTION_RULE_1_2b()
	while ((c1 = *p1++) == (c2 = *p2++)) {
		if (!c1) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	}

#else

	const	wchar_t * p1 = (wchar_t *) str1 - 1;
	const	wchar_t * p2 = (wchar_t *) str2 - 1;
				wchar_t		c1, c2;

	MISRA_EXCEPTION_RULE_1_2b()
	while ((c1 = *++p1) == (c2 = *++p2)) {
		if (!c1) {
			MISRA_EXCEPTION_RULE_14_7()
			return(0);
		}
	}

#endif

	return(int_t)c1 - (int_t)c2;
}


int_t _EWL_CDECL wcsncmp(const wchar_t * str1, const wchar_t * str2, size_t n)
{
#if !defined(__POWERPC__)

	const	wchar_t * p1 = (wchar_t *) str1;
	const	wchar_t * p2 = (wchar_t *) str2;
			wchar_t	c1, c2;

	n++;
	while (--n) {
		MISRA_EXCEPTION_RULE_1_2b()
		if ((c1 = *p1++) != (c2 = *p2++)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(int_t)c1 - (int_t)c2;
		} else { if (!c1) {
			break;
		}}
	}

#else

	const	wchar_t * p1 = (wchar_t *) str1 - 1;
	const	wchar_t * p2 = (wchar_t *) str2 - 1;
			wchar_t	c1, c2;

	n++;
	while (--n) {
		MISRA_EXCEPTION_RULE_1_2b()
		if ((c1 = *++p1) != (c2 = *++p2)) {
			MISRA_EXCEPTION_RULE_14_7()
			return(int_t)c1 - (int_t)c2;
		} else { if (!c1) {
			break;
		}}
	}

#endif

	return(0);
}


wchar_t * _EWL_CDECL wcschr(const wchar_t * str, const wchar_t chr)
{
#if !defined(__POWERPC__)

	const wchar_t * p = str;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while((ch = *p++)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) (p - 1));
		}
	}
	return(c ? 0 : (wchar_t *) (p - 1));

#else

	const wchar_t * p = str - 1;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while ((ch = *++p)) {
		if (ch == c) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) p);
		}
	}
	return(c ? 0 : (wchar_t *) p);

#endif
}


int_t _EWL_CDECL wcscoll(const wchar_t *str1, const wchar_t * str2)
{
	return(wcscmp(str1, str2));
}

size_t _EWL_CDECL wcsxfrm(wchar_t * _EWL_RESTRICT str1, const wchar_t * _EWL_RESTRICT str2, size_t n)
{
	size_t len = wcslen(str2);
	if (n > 0U) {
		MISRA_EXCEPTION_RULE_1_2a()
		(void)wcsncpy(str1, str2, n-1U);
		if (n < len) {
			str1[n-1U] = L'\0';
		}
	}
	return(len);
}


wchar_t * _EWL_CDECL wcsrchr(const wchar_t * str, wchar_t chr)
{
#if !defined(__POWERPC__)

	const wchar_t * p = str;
	const wchar_t * q = 0;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while ((ch = *p++) != 0) {
		if (ch == c) {
			q = p - 1;
		}
	}

	if (q) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) q);
	}

	return(c ? 0 : (wchar_t *) (p - 1));

#else

	const wchar_t * p = str - 1;
	const wchar_t * q = 0;
	      wchar_t   c = chr;
	      wchar_t   ch;

	MISRA_EXCEPTION_RULE_13_1()
	while ((ch = *++p) != 0) {
		if (ch == c) {
			q = p;
		}
	}

	if (q) {
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) q);
	}

	return(c ? 0 : (wchar_t *) p);

#endif
}

static int_t tst_char_map(const wchar_t *cm, wchar_t c)
{
	wchar_t	xc, uc = c;

#if !defined(__POWERPC__)

	const wchar_t *str = cm;

    MISRA_EXCEPTION_RULE_13_1()
    while ((xc = *str++)) {
         if (xc == uc) {
			MISRA_EXCEPTION_RULE_14_7()
         	return 1;
         }
    }
	return 0;

#else

	const wchar_t *str = cm - 1;

    MISRA_EXCEPTION_RULE_13_1()
    while ((xc = *++str)) {
         if (xc == uc) {
			MISRA_EXCEPTION_RULE_14_7()
         	return 1;
         }
    }
	return 0;

#endif
}


wchar_t * _EWL_CDECL wcspbrk(const wchar_t * str, const wchar_t * set)
{
	const 	wchar_t *	p;
	      	wchar_t		c;

#if !defined(__POWERPC__)

	p = (wchar_t *) str;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(set, c)) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) (p - 1));
		}
	}
	return(NULL);

#else

	p = (wchar_t *) str - 1;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(set, c)) {
			MISRA_EXCEPTION_RULE_14_7()
			return((wchar_t *) p);
		}
	}
	return(NULL);

#endif
}


size_t _EWL_CDECL wcsspn(const wchar_t * str, const wchar_t * set)
{
	const wchar_t *	p;
	      wchar_t	c;

#if !defined(__POWERPC__)

	p = (wchar_t *) str;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}
	MISRA_EXCEPTION_RULE_17_3()
	return ((p - (wchar_t *) str) - 1);

#else

	p = (wchar_t *) str - 1;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}
	MISRA_EXCEPTION_RULE_17_3()
	return(p - (wchar_t *) str);

#endif
}

size_t _EWL_CDECL wcscspn(const wchar_t * str, const wchar_t * set)
{
	const wchar_t *	p;
	      wchar_t	c;

#if !defined(__POWERPC__)

	p = (wchar_t *) str;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}
	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_17_3()
	return (size_t)((p - str) - 1);

#else

	p = (wchar_t *) str - 1;
	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}
	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_17_3()
	return (size_t)(p - str);

#endif
}


/* Code deleted here mm 010316  The version of wcstok for Windows made use of the thread-local
   data belonging to strtok, which breaks the requirement that strtok behave as if no other library
   function calls it.  Further, the Standards committe learnt from its experience with strtok that
   requiring a state that the library had to look after was a bad thing and so they gave wcstok an
   extra parameter so that thread-local storage is no longer needed for it.                         */

wchar_t * _EWL_CDECL wcstok(wchar_t * _EWL_RESTRICT str, const wchar_t * _EWL_RESTRICT set, wchar_t ** _EWL_RESTRICT ptr)
{
	wchar_t 		* p, * q;
	static wchar_t 	* n	= L"";
	wchar_t 		* s;
	wchar_t			c;

	if (str) {
		s = str;
	} else if (*ptr) {
		s = *ptr;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

#if !defined(__POWERPC__)

	p = s;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = NULL;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		s = n;
	} else {
		s    = p;
		*--p = (wchar_t)0;
	}

#else   /* __POWERPC__ */

	p = s - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (!tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = NULL;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(set, c)) {
			break;
		}
	}

	if (!c) {
		s = n;
	} else {
		s  = p + 1;
		*p = L'\0';
	}

#endif   /* __POWERPC__ */

	*ptr = s;
/*	was (q cannot be NULL here): 
	if (q == NULL) {
		*ptr = NULL;
	} else {
		*ptr = s;
	}
*/
	return q;
}

wchar_t * _EWL_CDECL wcsstr(const wchar_t * str, const wchar_t * pat)
{
#if !defined(__POWERPC__)

	wchar_t * s1 = (wchar_t *) str;
	wchar_t * p1 = (wchar_t *) pat;
	wchar_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_13_1()
	if (!(firstc = *p1++)) {		/* if pat is an empty string we return str */
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while((c1 = *s1++)) {
		if (c1 == firstc) {
			const wchar_t * s2 = s1;
			const wchar_t * p2 = p1;

			MISRA_EXCEPTION_RULE_1_2b()
			while (((c1 = *s2++) == (c2 = *p2++)) && c1) {}
			
			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return(s1 - 1);
			}
		}
	}

	return(NULL);

#else

	wchar_t * s1 = (wchar_t *) str-1;
	wchar_t * p1 = (wchar_t *) pat-1;
	wchar_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_13_1()
	if (!(firstc = *++p1))	{	/* if pat is an empty string we return str */
		MISRA_EXCEPTION_RULE_14_7()
		return((wchar_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while ((c1 = *++s1)) {
		if (c1 == firstc)
		{
			const wchar_t * s2 = s1-1;
			const wchar_t * p2 = p1-1;

			MISRA_EXCEPTION_RULE_13_1()
			MISRA_EXCEPTION_RULE_1_2b()
			while (((c1 = *++s2) == (c2 = *++p2)) && c1) {}

			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return((wchar_t *) s1);
			}
		}
	}

	return(NULL);

#endif
}

#pragma warn_possunwant reset

#endif /* #if _EWL_WIDE_CHAR */
