/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/03 09:08:15 $
 * $Revision: 1.16 $
 */

/*
 *	Routines
 *	--------
 *		strlen
 *		strnlen_s
 *
 *		strcpy
 *		strcpy_s
 *		strcat
 *		strcat_s
 *		strcmp
 *
 *		strncpy
 *		strncpy_s
 *		strncat
 *		strncat_s
 *		strncmp
 *
 *		strcoll
 *		strxfrm
 *
 *		strchr
 *		strrchr
 *		strstr
 *
 *		strspn
 *		strcspn
 *		strpbrk
 *		strtok
 *		strtok_s
 *
 *		strerror
 *		strerror_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale_api.h>
#include <ewl_thread_local_data.h>

MISRA_ALLOW_POINTER_CASTS()

#pragma warn_possunwant off

#ifdef __COLDFIRE__
#pragma optimize_for_size off
#endif

#if !defined(__MACH__)
#if defined(__INTEL__)

	#include <x86/string.x86.h>

#elif (!defined(__MC68K__) || _No_String_Inlines || !defined(__cplusplus))

	#if __dest_os != __n64_os
		#if !defined(__PPC_EABI__)
			size_t (strlen)(const char_t * str)
			{
				size_t len = (size_t)-1; /* overflows to 0 next */

			#if !defined(__POWERPC__)

				do {
					len++;
				} while (*str++);

			#else

				uchar_t * p = (uchar_t *) str - 1;

				do {
					len++;
				} while (*++p);

			#endif

				return(len);
			}
		#endif /* __PPC_EABI__ */
	#endif /* __dest_os != __n64_os */

		#if (defined(__PPC_EABI__) || defined(__MIPS__) || defined(__arm))
			#define K1 0x80808080L
			#define K2 0xfefefeffL
		#endif

		#pragma ANSI_strict off

		char_t * (strcpy)(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src)
		{

		#if (!defined(__POWERPC__) && !defined(__MIPS__) && !defined(__arm))
			const	char_t * p = src;
					char_t * q = dst;

			MISRA_EXCEPTION_RULE_13_1()
			while ((*q++ = *p++)) {}

			return(dst);

		#elif (defined(__PPC_EABI__) || defined(__MIPS__) || defined(__arm))

			#if _GCCPORT_
			int_t *portPtr = NULL;
			#endif

			/*
			 *	strcpy routine designed to minimized the number of
			 *	loads and stores.  We make sure that all the loads and
			 *  stores are properly aligned.
			 *
			 */
			register uchar_t *destb, *fromb;
			register uint_t w, t, align;
			register uint_t k1;
			register uint_t k2;

			fromb = (uchar_t *)src;
			destb = (uchar_t *)dst;

			/*
			 *	If either the destination or the source are not
			 *	aligned on the same boundary, we do a byte copy.
			 *	Otherwise we align them to a word boundary.
			 */
			MISRA_EXCEPTION_RULE_11_3()
			if ((align = ((uint_t)fromb & 3u)) != ((uint_t)destb & 3u)) {
				MISRA_EXCEPTION_RULE_14_4()
				goto bytecopy;
			}

			if (align) {
				MISRA_EXCEPTION_RULE_10_1()
				if ((*destb = *fromb) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(dst);
				}
				for (align = 3u - align; align; align--) {
					MISRA_EXCEPTION_RULE_10_1()
					if ((*(++destb) = *(++fromb)) == 0) {
						MISRA_EXCEPTION_RULE_14_7()
						return(dst);
					}
				}
				++destb;
				++fromb;
			}

			/*	Source and destination are word aligned */

			k1 = K1;
			k2 = K2;

			/*	Load a word and test for a null byte. */
			MISRA_QUIET_SUSPICIOUS_PTR_CAST()
			w = *((uint_t*)(fromb));
			/*
			 *	Here we need to test if one of the bytes in w is 0, in
			 *	which case we have reached the end of the string.  If we
			 *	reach the end of the string we go to the one-byte-copy
			 *	block.  This code is endian-neutral.
			 *	The naive way would be to test each byte of the word, but
			 *	that would require at least four comparisons, few shifts,
			 *	and few branches.  This would be extremely costly especially
			 *	on machines where the conditional branch destroys the cache.
			 *
			 *	Assertion:
			 *	----------
			 *	Let B be a byte.  The most significant bit of
			 *	(B-1) & ~B will be set, if and only if B==0.
			 *
			 *	Proof:
			 *	------
			 * 		B			| 	00	| 01..7F|	80	| 81..FF|
			 * 		B-1			|	FF	| 00..7E|	7F	| 80..FE|
			 * 		~B			|	FF	| FE..80|	7F	| 7E..00|
			 *	MSB((B-1)&~B)	|	1	| 0		|	0	| 0		|
			 *
			 *	Adding 0xFF to a byte will always generate a carry
			 *	except when the byte is 0.  So when working with 4
			 *	bytes at a time, we will add FF to the first byte and
			 *	FE to the other bytes because the carry will come
			 *	from the preceding byte.
			 */
			t = w + k2;
 		 	t &= ~w;
			/*
			 *	Extract the most significant bit of each byte.  If one
			 *	bit is left it means we have seen a 0.
			 */
			t &= k1;
			if (t) {
				MISRA_EXCEPTION_RULE_14_4()
				goto bytecopy;
			}

			destb -= sizeof(int_t);

			for (;;) {
				/*	Put the tested word into the incremented destination. */
				destb += sizeof(int_t);
				MISRA_EXCEPTION_RULE_10_1()
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				*((uint_t*)(destb)) = w;

				/*	Load a word and increment source for testing. */
				fromb += sizeof(int_t);
				MISRA_EXCEPTION_RULE_10_1()
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				w = *((uint_t*)(fromb));

				t = w + k2;
 		 		t &= ~w;
				t &= k1;
				if (t) {
					MISRA_EXCEPTION_RULE_14_4()
					goto adjust;
				}
			}

		adjust:
			destb += sizeof(int_t);
		bytecopy:
			MISRA_EXCEPTION_RULE_10_1()
			if ((*destb = *fromb) == 0) {
				MISRA_EXCEPTION_RULE_14_7()
				return(dst);
			}
			for (;;) {
				MISRA_EXCEPTION_RULE_10_1()
				if ((*(++destb) = *(++fromb)) == 0) {
					MISRA_EXCEPTION_RULE_14_7()
					return(dst);
				}
			}
		#else

			const	uchar_t * p = (uchar_t *) src - 1;
					uchar_t * q = (uchar_t *) dst - 1;

			MISRA_EXCEPTION_RULE_10_1()
			while (*++q = *++p) {}

			return(dst);
		#endif

		}
		#pragma ANSI_strict reset

#endif /* !__MC68K__ || _No_String_Inlines || !defined(__cplusplus) */

#if !defined(__INTEL__)

	char_t * strncpy(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src, size_t n)
	{
		#if !defined(__POWERPC__)

			const	char_t * p = src;
			char_t * q = dst;

			n++;
			while (--n) {
				MISRA_EXCEPTION_RULE_13_1()
				if (!(*q++ = *p++)) {
					while (--n) {
						*q++ = 0;
					}
					break;
				}
			}

		#else

			const	uchar_t * p		= (const uchar_t *) src - 1;
			uchar_t * q		= (uchar_t *) dst - 1;

			n++;
			while (--n) {
				MISRA_EXCEPTION_RULE_13_1()
				if (!(*++q = *++p)) {
					while (--n) {
						*++q = 0u;
					}
					break;
				}
			}

		#endif

		return(dst);
	}

		char_t * strcat(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src)
		{
		#if !defined(__POWERPC__)

			const	char_t * p = src;
					char_t * q = dst;

			while (*q++) {}

			q--;

			MISRA_EXCEPTION_RULE_13_1()
			while ((*q++ = *p++)) {}

		#else

			const	uchar_t * p = (uchar_t *) src - 1;
					uchar_t * q = (uchar_t *) dst - 1;

			while (*++q) {}

			q--;

			MISRA_EXCEPTION_RULE_13_1()
			while ((*++q = *++p)) {}

		#endif

			return(dst);
		}

	char_t * strncat(char_t * _EWL_RESTRICT dst, const char_t * _EWL_RESTRICT src, size_t n)
	{
		#if !defined(__POWERPC__)

			const	char_t * p = src;
					char_t * q = dst;

			while (*q++) {}

			q--; n++;

			while (--n) {
				MISRA_EXCEPTION_RULE_13_1()
				if (!(*q++ = *p++)) {
					q--;
					break;
				}
			}

			*q = 0;

		#else

			const	uchar_t * p = (uchar_t *) src - 1;
					uchar_t * q = (uchar_t *) dst - 1;

			while (*++q) {}

			q--; n++;

			while (--n) {
				MISRA_EXCEPTION_RULE_13_1()
				if (!(*++q = *++p)) {
					q--;
					break;
				}
			}

			*++q = 0u;

		#endif

		return(dst);
	}

	#pragma ANSI_strict off

		int_t strcmp(const char_t * str1, const char_t * str2)
		{
			#if (defined(__POWERPC__) || defined(__MIPS__) || defined(__MC68K__) || defined(__arm))

				#if _GCCPORT_
				int_t *portPtr = NULL;
				#endif

				/*
				 *	strcmp routine designed to minimized the number of
				 *	loads and stores.  We make sure that all the loads and
				 *  stores are properly aligned.
				 *
				 */
				register uchar_t *left = (uchar_t *)str1;
				register uchar_t *right = (uchar_t *)str2;
				uint_t k1, k2, align, l1, r1, x;
				int_t result;

				/*	Check the simple case of the first byte being different. */
				l1 = *left;
				r1 = *right;
				result = (int_t)l1 - (int_t)r1;
				if (result) {
					MISRA_EXCEPTION_RULE_14_7()
					return result;
				}

				/*
				 *	If either the destination or the source are not
				 *	aligned on the same boundary, we do a byte copy.
				 *	Otherwise we align them to a word boundary.
				 */
				MISRA_EXCEPTION_RULE_11_3()
				if ((align = ((uint_t)left & 3u)) != ((uint_t)right & 3u)) {
					MISRA_EXCEPTION_RULE_14_4()
					goto bytecopy;
				}
				if (align) {
					/*	Continuation of test of first byte. */
					if (l1 == 0u) {
						MISRA_EXCEPTION_RULE_14_7()
						return (0);
					}
					for (align = 3u - align; align; align--) {
						l1 = *(++left);
						r1 = *(++right);
						result = (int_t)l1 - (int_t)r1;
						if (result) {
							MISRA_EXCEPTION_RULE_14_7()
							return result;
						}
						if (l1 == 0u) {
							MISRA_EXCEPTION_RULE_14_7()
							return (0);
						}
					}
					left++;
					right++;
				}

				/*	Strings are word aligned */

				k1 = K1;
				k2 = K2;

				/*	Load a word from each string. */
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				l1 = *(uint_t*)left;
				MISRA_QUIET_SUSPICIOUS_PTR_CAST()
				r1 = *(uint_t*)right;
				/*	See comments in strcpy function. */
				x = l1 + k2;
				x &= ~l1;
				if (x & k1) {
					MISRA_EXCEPTION_RULE_14_4()
					goto adjust;
				}
				while(l1 == r1) {
					/*	Load a word and increment strings. */
					left += sizeof(uint_t);
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					l1 = *((uint_t*)left);
					right += sizeof(uint_t);
					MISRA_QUIET_SUSPICIOUS_PTR_CAST()
					r1 = *((uint_t*)right);
					x = l1 + k2;
					if (x & k1) {
						MISRA_EXCEPTION_RULE_14_4()
						goto adjust;
					}
				}

			/*	l1 and r1 are not equal.  */
			#if _EWL_LITTLE_ENDIAN
				--left;
				--right;
				MISRA_EXCEPTION_RULE_14_4()
				goto bytecopy;
			#endif

			adjust:
				l1 = *left;
				r1 = *right;
				result = (int_t)l1 - (int_t)r1;
				if (result) {
					MISRA_EXCEPTION_RULE_14_7()
					return result;
				}
			bytecopy:
				if (l1 == 0u) {
					MISRA_EXCEPTION_RULE_14_7()
					return (0);
				}
				for(;;) {
					l1 = *(++left);
					r1 = *(++right);
					result = (int_t)l1 - (int_t)r1;
					if (result) {
						MISRA_EXCEPTION_RULE_14_7()
						return result;
					}
					if (l1 == 0u) {
						MISRA_EXCEPTION_RULE_14_7()
						return (0);
					}
				}

			#else

				const	uchar_t * p1 = (uchar_t *) str1;
				const	uchar_t * p2 = (uchar_t *) str2;
						uchar_t	c1, c2;

				c1 = *p1++;
				c2 = *p2++;
				while (c1 == c2) {
					if (!c1) {
						MISRA_EXCEPTION_RULE_14_7()
						return(0);
					}
					c1 = *p1++;
					c2 = *p2++;
				}

				return ((int_t)c1 - (int_t)c2);

			#endif
		}

		#pragma ANSI_strict reset

	int_t strncmp(const char_t * str1, const char_t * str2, size_t n)
	{
		#if defined(__POWERPC__)

			const	uchar_t * p1 = (uchar_t *) str1 - 1;
			const	uchar_t * p2 = (uchar_t *) str2 - 1;
					uint32_t		c1, c2;

			n++;
			while (--n) {
				MISRA_EXCEPTION_RULE_1_2b()
				if ((c1 = *++p1) != (c2 = *++p2)) {
					MISRA_EXCEPTION_RULE_14_7()
					return ((int_t)c1 - (int_t)c2);
				} else { if (!c1) {
					break;
				}}
			}

		#else

			const	uchar_t * p1 = (uchar_t *) str1;
			const	uchar_t * p2 = (uchar_t *) str2;
					uchar_t		c1, c2;

			n++;
			while (--n) {
				MISRA_EXCEPTION_RULE_1_2b()
				if ((c1 = *p1++) != (c2 = *p2++)) {
					MISRA_EXCEPTION_RULE_14_7()
					return ((int_t)c1 - (int_t)c2);
				} else {if (!c1) {
					break;
				}}
			}

		#endif

		return(0);
	}

	#if __dest_os != __n64_os
		char_t * strchr(const char_t * str, int_t chr)
		{
		#if !defined(__POWERPC__)

			const char_t * p = str;
			      char_t   c = (char_t)chr;
			      char_t   ch;

			MISRA_EXCEPTION_RULE_13_1()
			while((ch = *p++)) {
				if (ch == c) {
					MISRA_EXCEPTION_RULE_14_7()
					return((char_t *) (p - 1));
				}
			}

			return(c ? 0 : (char_t *) (p - 1));

		#else

			const uchar_t * p = (uchar_t *) str - 1;
			uint32_t   c = ((uint32_t)chr & 0xffu);
		    uint32_t   ch;

			MISRA_EXCEPTION_RULE_13_1()
			while((ch = *++p)) {
				if (ch == c) {
					MISRA_EXCEPTION_RULE_14_7()
					return((char_t *) p);
				}
			}

			return(c ? 0 : (char_t *) p);

		#endif
		}

	#endif /* __dest_os != __n64_os */

#endif /* __INTEL__ */
#endif /* __MACH__ */

int_t _EWL_CDECL strcoll(const char_t *str1, const char_t * str2)
{
	char_t * str1coll;
	char_t * str2coll;
	int_t res;
	int_t len1, len2;

	len1 	 = MB_LEN_MAX * ((int_t)strlen(str1) + 1);
	len2 	 = MB_LEN_MAX * ((int_t)strlen(str2) + 1);
	MISRA_EXCEPTION_RULE_20_4()
	str1coll = malloc((size_t)len1);
	MISRA_EXCEPTION_RULE_20_4()
	str2coll = malloc((size_t)len2);
	MISRA_QUIET_NULL_POINTER_ARG()
	(void)strxfrm(str1coll, str1, (size_t)len1);
	MISRA_QUIET_NULL_POINTER_ARG()
	(void)strxfrm(str2coll, str2, (size_t)len2);
	res 	 = strcmp(str1coll, str2coll);
	MISRA_EXCEPTION_RULE_20_4()
	free(str1coll);
	MISRA_EXCEPTION_RULE_20_4()
	free(str2coll);
	return(res);
}

MISRA_QUIET_LINT_04()
size_t _EWL_CDECL strxfrm(char_t * _EWL_RESTRICT str1, const char_t * _EWL_RESTRICT str2, size_t n)
{
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE

	size_t reslen = strlen(str2);
	if (str1 != NULL) {
		(void)strncpy(str1, str2, n);
	}
	return(reslen);

#else

	uchar_t * chptr,
			* collptr,
			* weight1,
			* weight2,
			* localstr1,
			* wt1ptr, *wt2ptr, *tempptr;
	struct __locale *    current_locale_ptr;
	int_t 	offset;
	int_t	tabsize;
	int_t 	i;
	uint16_t ch;
	int_t	chrval;
	size_t  reslen;
	int_t	lenstr2 = (int_t)strlen(str2);

	if (lenstr2 == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_20_4()
	localstr1 		   = (uchar_t*)malloc((size_t)(MB_LEN_MAX * (lenstr2+1)));
	current_locale_ptr = &_EWL_LOCALDATA(_current_locale);
	offset             = current_locale_ptr->coll_cmpt_ptr->char_start_value;
	tabsize            = current_locale_ptr->coll_cmpt_ptr->char_coll_tab_size;
	chptr              = (uchar_t*)str2;
	MISRA_EXCEPTION_RULE_20_4()
	weight1            = (uchar_t*)malloc((size_t)lenstr2);
	MISRA_EXCEPTION_RULE_20_4()
	weight2            = (uchar_t*)malloc((size_t)lenstr2);

	MISRA_EXCEPTION_RULE_20_4()
	if ((localstr1 == 0L) || (weight1 == 0L) || (weight2 == 0L)) {
		/* malloc failure */
		if (localstr1) { free(localstr1); }		
		if (weight1) { free(weight1); }		
		if (weight2) { free(weight2); }		
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}
	collptr            = localstr1;
	
	MISRA_EXCEPTION_RULE_17_3()
	for (wt1ptr = weight1; wt1ptr < (weight1 + lenstr2); wt1ptr++) {
		*wt1ptr = (uchar_t)'\0';
	}
	MISRA_EXCEPTION_RULE_17_3()
	for (wt2ptr = weight2; wt2ptr < (weight2 + lenstr2); wt2ptr++) {
		*wt2ptr = (uchar_t)'\0';
	}
	wt1ptr = weight1;
	wt2ptr = weight2;

	for (i = 0; i < lenstr2; i++) {
		chrval = (int_t)*chptr++;
		if ((chrval >= offset) && (chrval <= (offset + tabsize))) {
			ch = *(current_locale_ptr->coll_cmpt_ptr->char_coll_table_ptr + (chrval - offset));
			if (ch != 0u) {
				*collptr++ = (uchar_t)(ch & 0x00ffu);
				if ((ch & 0x0f00u) != 0u) {
					*wt1ptr++ = (uchar_t)((ch & 0x0f00u) >> 8);
				}
				if ((ch & 0xf000u) != 0u) {
					*wt2ptr++ = (uchar_t)((ch & 0xf000u) >> 12);
				}
			}
		}
	}
	if (current_locale_ptr->coll_cmpt_ptr->char_spec_accents) {
		MISRA_EXCEPTION_RULE_17_3()
		for (tempptr = wt1ptr - 1; tempptr >= weight1; tempptr--) {
			*collptr++ = *tempptr;
		}
	} else {
		MISRA_EXCEPTION_RULE_17_3()
		for (tempptr = weight1; tempptr < wt1ptr; tempptr++) {
			*collptr++ = *tempptr;
		}
	}

	MISRA_EXCEPTION_RULE_17_3()
	for (tempptr = weight2; tempptr < wt2ptr; tempptr++) {
		*collptr++ = *tempptr;
	}

	*collptr = (uchar_t)'\0';
	MISRA_EXCEPTION_RULE_20_4()
	free(weight1);
	MISRA_EXCEPTION_RULE_20_4()
	free(weight2);
	reslen = strlen((const char_t *)localstr1);
	if (str1 != NULL) {
		strncpy(str1, (const char_t *)localstr1, n);
	}
	MISRA_EXCEPTION_RULE_20_4()
	free(localstr1);
	return(reslen);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
}
/* end complete rewrite */

char_t * _EWL_CDECL strrchr(const char_t * str, int_t chr)
{
	#if !defined(__POWERPC__)

		const char_t * p = str;
		const char_t * q = 0;
		      char_t   c = (char_t)chr;
		      char_t   ch;

		MISRA_EXCEPTION_RULE_13_1()
		while((ch = *p++)) {
			if (ch == c) {
				q = p - 1;
			}
		}

		if (q) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) q);
		}

		return(c ? 0 : (char_t *) (p - 1));

	#else

		const uchar_t * p = (uchar_t *) str - 1;
		const uchar_t * q = 0;
		      uint32_t   c = ((uint32_t)chr & 0xffu);
		      uint32_t   ch;

		MISRA_EXCEPTION_RULE_13_1()
		while((ch = *++p)) {
			if (ch == c) {
				q = p;
			}
		}

		if (q) {
			MISRA_EXCEPTION_RULE_14_7()
			return((char_t *) q);
		}

		return(c ? 0 : (char_t *) p);

	#endif
}

typedef uchar_t char_map[32];

MISRA_EXCEPTION_RULE_19_7()
#define set_char_map(map, ch) (map[(uchar_t)(ch)>>3] |= (uchar_t)(1u << ((ch)&7u)))
#define tst_char_map(map, ch) (map[(uchar_t)(ch)>>3] &  (uchar_t)(1u << ((ch)&7u)))
_MISRA_RESTORE()

char_t * _EWL_CDECL strpbrk(const char_t * str, const char_t * set)
{
	const uchar_t * p;
	uchar_t c;
	char_map map = {0u};

	#if !defined(__POWERPC__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (tst_char_map(map, c)) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) (p - 1));
			}
		}

		return(NULL);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (tst_char_map(map, c)) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) p);
			}
		}

		return(NULL);

	#endif
}

size_t _EWL_CDECL strspn(const char_t * str, const char_t * set)
{
	const uchar_t *	p;
	uchar_t	c;
	char_map map = {0u};

	#if !defined(__POWERPC__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}
		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_17_3()
		return (size_t)((p - (uchar_t *) str) - 1);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (!tst_char_map(map, c)) {
				break;
			}
		}

		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_17_3()
		return(p - (uchar_t *) str);

#endif
}

size_t _EWL_CDECL strcspn(const char_t * str, const char_t * set)
{
	const uchar_t *	p;
	      uchar_t	c;
	      char_map	map = {0u};

	#if !defined(__POWERPC__)

		p = (uchar_t *) set;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *p++)) {
			if (tst_char_map(map, c)) {
				break;
			}
		}

		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_17_3()
		return(size_t)((p - (uchar_t *) str) - 1);

	#else

		p = (uchar_t *) set - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			set_char_map(map, c);
		}

		p = (uchar_t *) str - 1;

		MISRA_EXCEPTION_RULE_13_1()
		while ((c = *++p)) {
			if (tst_char_map(map, c)) {
				break;
			}
		}

		MISRA_EXCEPTION_RULE_10_3()
		MISRA_EXCEPTION_RULE_17_3()
		return(p - (uchar_t *) str);

	#endif
}

char_t * _EWL_CDECL strtok(char_t * _EWL_RESTRICT str, const char_t * _EWL_RESTRICT set)
{
#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static uchar_t *	strtok_dn = (uchar_t *) "";
	static uchar_t *	strtok_ds = (uchar_t *) "";
#endif

	uchar_t *p, *q;
    uchar_t c;

	char_map map = {0u};

	if (str) {
		_EWL_LOCALDATA(strtok_ds) = (uchar_t *) str;
	}

	#if !defined(__POWERPC__)

	p = (uchar_t *) set;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		set_char_map(map, c);
	}

	p = _EWL_LOCALDATA(strtok_ds);

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (!tst_char_map(map, c)) {
			break;
		}
	}

	if (!c)	{
		_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
	} else	{
		_EWL_LOCALDATA(strtok_ds) = p;
		*--p = 0u;
	}

	return((char_t *) q);

	#else

	p = (uchar_t *) set - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		set_char_map(map, c);
	}

	p = _EWL_LOCALDATA(strtok_ds) - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (!tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		_EWL_LOCALDATA(strtok_ds) = _EWL_LOCALDATA(strtok_dn);
	} else {
		_EWL_LOCALDATA(strtok_ds) = p + 1;
		*p = 0u;
	}

	return((char_t *) q);

	#endif
}

char_t * _EWL_CDECL strtok_s(char_t * _EWL_RESTRICT str, const char_t * _EWL_RESTRICT set, char_t ** _EWL_RESTRICT ptr)
{
	uchar_t *p, *q;
    uchar_t c;

	uchar_t *	n = (uchar_t *) "";
	char_map map = {0u};

	if (((set == NULL) || (ptr == NULL)) || ((*ptr == NULL) && (str == NULL))) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return NULL;
	}

	if (str) {
		*ptr = str;
	}

	#if !defined(__POWERPC__)

	p = (uchar_t *) set;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		set_char_map(map, c);
	}

	p = (uchar_t *) *ptr;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (!tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = (char_t *) n;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *p++)) {
		if (tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = (char_t *) n;
	} else {
		*ptr = (char_t *) p;
		*--p = 0u;
	}

	return((char_t *) q);

	#else

	p = (uchar_t *) set - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		set_char_map(map, c);
	}

	p = (uchar_t *) *ptr - 1;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (!tst_char_map(map, c)) {
			break;
		}
	}

	if (!c)	{
		*ptr = (char_t *) n;
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	q = p;

	MISRA_EXCEPTION_RULE_13_1()
	while ((c = *++p)) {
		if (tst_char_map(map, c)) {
			break;
		}
	}

	if (!c) {
		*ptr = (char_t *) n;
	} else	{
		*ptr = (char_t *) p + 1;
		*p = 0u;
	}

	return((char_t *) q);

	#endif
}

char_t * _EWL_CDECL strstr(const char_t * str, const char_t * pat)
{
	#if !defined(__POWERPC__)

	uchar_t * s1 = (uchar_t *) str;
	uchar_t * p1 = (uchar_t *) pat;
	uchar_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_12_4()
	MISRA_EXCEPTION_RULE_13_1()
	if ((pat == NULL) || (!(firstc = *p1++))) {
		MISRA_EXCEPTION_RULE_14_7()
		return((char_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while((c1 = *s1++)) {
		if (c1 == firstc) {
			const uchar_t * s2 = s1;
			const uchar_t * p2 = p1;

			MISRA_EXCEPTION_RULE_1_2b()
			MISRA_EXCEPTION_RULE_13_1()
			while (((c1 = *s2++) == (c2 = *p2++)) && c1) {}

			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) s1 - 1);
			}
		}
	}

	return(NULL);

	#else

	uchar_t * s1 = (uchar_t *) str-1;
	uchar_t * p1 = (uchar_t *) pat-1;
	uint32_t firstc, c1, c2;

	MISRA_EXCEPTION_RULE_12_4()
	MISRA_EXCEPTION_RULE_13_1()
	if ((pat == NULL) || (!(firstc = *++p1))) {
		MISRA_EXCEPTION_RULE_14_7()
		return((char_t *) str);
	}

	MISRA_EXCEPTION_RULE_13_1()
	while ((c1 = *++s1)) {
		if (c1 == firstc) {
			const uchar_t * s2 = s1-1;
			const uchar_t * p2 = p1-1;

			MISRA_EXCEPTION_RULE_1_2b()
			MISRA_EXCEPTION_RULE_13_1()
			while (((c1 = *++s2) == (c2 = *++p2)) && c1) {}

			if (!c2) {
				MISRA_EXCEPTION_RULE_14_7()
				return((char_t *) s1);
			}
		}
	}

	return(NULL);

	#endif
}

size_t (strnlen_s)(const char_t * str, size_t maxsize)
{
	uchar_t *end;
	uchar_t *p;
	size_t len = (size_t)-1; /* overflows to zero */

	if ((maxsize == 0U) || (str == NULL)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}

	end = (uchar_t *) &(str[maxsize - 1U]);
	p = (uchar_t *) str - 1;

	MISRA_EXCEPTION_RULE_12_4()
	MISRA_EXCEPTION_RULE_17_3()
	do {
		len++;
	} while ((end > p) && *++p);

	return len;
}

errno_t (strcpy_s)(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src)
{
	if ((dst_max == 0U) || (dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= strnlen_s(src, dst_max)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strcpy(dst, src);
	return ENOERR;
}

errno_t strncpy_s(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;

	if ((dst_max == 0U) || (dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_max <= n) {
		for (i = 0u; i < dst_max; i++) {
			if (src[i] == 0) {
				strcpy(dst, src);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strncpy(dst, src, n);
	dst[n] = 0;
	return ENOERR;
}

errno_t (strcat_s)(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src)
{
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - strnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max <= strnlen_s(src, dst_max)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strcat(dst, src);
	return ENOERR;
}

errno_t strncat_s(char_t * _EWL_RESTRICT dst, rsize_t dst_max, const char_t * _EWL_RESTRICT src, rsize_t n)
{
	rsize_t i;
	rsize_t dst_cat_max;

	if ((dst == NULL) || (src == NULL) || (dst_max > (rsize_t)RSIZE_MAX) || (n > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	dst_cat_max = dst_max - strnlen_s(dst, dst_max);

	if (dst_cat_max == 0U) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	if (dst_cat_max < n) {
		for (i = 0u; i < dst_cat_max; i++) {
			if (src[i] == 0) {
				strncat(dst, src, n);
				MISRA_EXCEPTION_RULE_14_7()
				return ENOERR;
			}
		}
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		dst[0] = 0;
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strncat(dst, src, n);
	if (dst_cat_max != n) {
		dst[(dst_max-dst_cat_max)+n] = 0;
	}
	return ENOERR;
}

char_t * _EWL_CDECL strerror(int_t errnum)
{
	static char_t errstr[__max_errstr];

	return(__strerror(errnum, errstr));
}

errno_t _EWL_CDECL strerror_s(char_t *s, rsize_t maxsize, errno_t errnum)
{
	rsize_t realsize;
	char_t errstr[__max_errstr];

	if ((s == NULL) || (maxsize > (rsize_t)RSIZE_MAX) || (maxsize == 0U))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}
	MISRA_EXCEPTION_RULE_1_2a()
	(void)__strerror(errnum, errstr);
	realsize = strlen(errstr);

	if (maxsize <= realsize)
	{
		if (maxsize > 0U)
		{
			strncpy(s, errstr, maxsize-1U);
			s[maxsize-1U] = 0;
			if (maxsize > 3U)
			{
				s[maxsize-2U] = '.';
				s[maxsize-3U] = '.';
				s[maxsize-4U] = '.';
			}
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	strcpy(s, errstr);
	return ENOERR;
}

char_t * _EWL_CDECL __strerror(int_t errnum, char_t * str)
{

#if _AEABI_PORTABILITY_LEVEL
#if _EWL_C99
	if(errnum == EILSEQ)
	{
		strcpy(str, "Wide character encoding error");
	}
	else
#endif
	if(errnum == EDOM)
	{
		strcpy(str, "Numerical argument out of domain");
	}
	else if(errnum == ERANGE)
	{
		strcpy(str, "Result too large");
	}
	else 
	{
	switch (errnum)
	{
		#if _EWL_STRERROR_KNOWS_ERROR_NAMES
		case E2BIG:			strcpy(str, "Argument list too long");				break;
		case EACCES:		strcpy(str, "Permission denied");					break;
		case EAGAIN:		strcpy(str, "Resource temporarily unavailable");	break;
		case EBADF:			strcpy(str, "Bad file descriptor");					break;
		case EBUSY:			strcpy(str, "Device busy");							break;
		case ECHILD:		strcpy(str, "No child processes");					break;
		case EDEADLK:		strcpy(str, "Resource deadlock avoided");			break;

		case EEXIST:		strcpy(str, "File exists");							break;
		case EFAULT:		strcpy(str, "Bad address");							break;
		case EFBIG:			strcpy(str, "File too large");						break;
		case EFPOS:			strcpy(str, "File Position Error");					break;



		case EINTR:			strcpy(str, "Interrupted system call");				break;
		case EINVAL:		strcpy(str, "Invalid argument");					break;
		case EIO:			strcpy(str, "Input/output error");					break;
		case EISDIR:		strcpy(str, "Is a directory");						break;
		case EMFILE:		strcpy(str, "Too many open files");					break;
		case EMLINK:		strcpy(str, "Too many links");						break;
		case ENAMETOOLONG:	strcpy(str, "File name too long");					break;
		case ENFILE:		strcpy(str, "Too many open files in system");		break;
		case ENODEV:		strcpy(str, "Operation not supported by device");	break;
		case ENOENT:		strcpy(str, "No such file or directory");			break;
		case ENOEXEC:		strcpy(str, "Exec format error");					break;
		case ENOLCK:		strcpy(str, "No locks available");					break;
		case ENOMEM:		strcpy(str, "Cannot allocate memory");				break;
		case ENOSPC:		strcpy(str, "No space left on device");				break;
		case ENOSYS:		strcpy(str, "Function not implemented");			break;
		case ENOTDIR:		strcpy(str, "Not a directory");						break;
		case ENOTEMPTY:		strcpy(str, "Directory not empty");					break;
		case ENOTTY:		strcpy(str, "Inappropriate ioctl for device");		break;
		case ENXIO:			strcpy(str, "Device not configured");				break;
		case EPERM:			strcpy(str, "Operation not permitted");				break;
		case EPIPE:			strcpy(str, "Broken pipe");							break;
		case EROFS:			strcpy(str, "Read-only file system");				break;
		case ESIGPARM:		strcpy(str, "Signal error");						break;
		case ESPIPE:		strcpy(str, "Illegal seek");						break;
		case ESRCH:			strcpy(str, "No such process");						break;
		case EUNKNOWN:		strcpy(str, "Unknown error");						break;
		case EXDEV:			strcpy(str, "Cross-device link");					break;
		#endif /* _EWL_STRERROR_KNOWS_ERROR_NAMES */
		default:			sprintf(str, "Unknown Error (%d)", errnum); 		break;
	}
	}

#else

#if _EWL_STRERROR_KNOWS_ERROR_NAMES
	switch (errnum)
	{
		case E2BIG:			strcpy(str, "Argument list too long");				break;
		case EACCES:		strcpy(str, "Permission denied");					break;
		case EAGAIN:		strcpy(str, "Resource temporarily unavailable");	break;
		case EBADF:			strcpy(str, "Bad file descriptor");					break;
		case EBUSY:			strcpy(str, "Device busy");							break;
		case ECHILD:		strcpy(str, "No child processes");					break;
		case EDEADLK:		strcpy(str, "Resource deadlock avoided");			break;
		case EDOM:			strcpy(str, "Numerical argument out of domain");	break;
		case EEXIST:		strcpy(str, "File exists");							break;
		case EFAULT:		strcpy(str, "Bad address");							break;
		case EFBIG:			strcpy(str, "File too large");						break;
		case EFPOS:			strcpy(str, "File Position Error");					break;
#if _EWL_C99
		case EILSEQ:		strcpy(str, "Wide character encoding error");		break;
#endif /* _EWL_C99 */
		case EINTR:			strcpy(str, "Interrupted system call");				break;
		case EINVAL:		strcpy(str, "Invalid argument");					break;
		case EIO:			strcpy(str, "Input/output error");					break;
		case EISDIR:		strcpy(str, "Is a directory");						break;
		case EMFILE:		strcpy(str, "Too many open files");					break;
		case EMLINK:		strcpy(str, "Too many links");						break;
		case ENAMETOOLONG:	strcpy(str, "File name too long");					break;
		case ENFILE:		strcpy(str, "Too many open files in system");		break;
		case ENODEV:		strcpy(str, "Operation not supported by device");	break;
		case ENOENT:		strcpy(str, "No such file or directory");			break;
		case ENOERR:		strcpy(str, "No error detected");					break;
		case ENOEXEC:		strcpy(str, "Exec format error");					break;
		case ENOLCK:		strcpy(str, "No locks available");					break;
		case ENOMEM:		strcpy(str, "Cannot allocate memory");				break;
		case ENOSPC:		strcpy(str, "No space left on device");				break;
		case ENOSYS:		strcpy(str, "Function not implemented");			break;
		case ENOTDIR:		strcpy(str, "Not a directory");						break;
		case ENOTEMPTY:		strcpy(str, "Directory not empty");					break;
		case ENOTTY:		strcpy(str, "Inappropriate ioctl for device");		break;
		case ENXIO:			strcpy(str, "Device not configured");				break;
		case EPERM:			strcpy(str, "Operation not permitted");				break;
		case EPIPE:			strcpy(str, "Broken pipe");							break;
		case ERANGE:		strcpy(str, "Result too large");					break;
		case EROFS:			strcpy(str, "Read-only file system");				break;
		case ESIGPARM:		strcpy(str, "Signal error");						break;
		case ESPIPE:		strcpy(str, "Illegal seek");						break;
		case ESRCH:			strcpy(str, "No such process");						break;
		case EUNKNOWN:		strcpy(str, "Unknown error");						break;
		case EXDEV:			strcpy(str, "Cross-device link");					break;
		default:			sprintf(str, "Unknown Error (%d)", errnum); 		break;
	}
#else
	/* save data space, just return a generic error */
    sprintf(str, "Unknown Error (%d)", errnum);

#endif /* _EWL_STRERROR_KNOWS_ERROR_NAMES */
#endif /* _AEABI_PORTABILITY_LEVEL */

	return(str);
}

#pragma warn_possunwant reset
