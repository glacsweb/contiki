/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 15:08:18 $
 * $Revision: 1.10 $
 */

/*	Routines
 *	--------
 *		fscanf
 *		fscanf_s
 *		scanf
 *		scanf_s
 *		sscanf
 *		sscanf_s
 *		vfscanf
 *		vfscanf_s
 *		vsscanf
 *		vsscanf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <strtold.h>
#include <strtoul.h>
#include <critical_regions.h>
#include <console_io.h>

extern int_t __sformatter(int_t (_EWL_CDECL *ReadProc)(void *a, int_t b, int_t c),
						void		 *ReadProcArg,
						const char_t *format_str,
						va_list		  arg,
						int_t		  is_secure);

int_t _EWL_CDECL __FileRead(void *File, int_t ch, int_t Action)
{
	switch(Action) {
		case __GetAChar:
			MISRA_EXCEPTION_RULE_14_7()
			return __getc((struct _FILE *)File);

		case __UngetAChar:
			if (ch != (int_t)EOF) {
				MISRA_EXCEPTION_RULE_14_7()
				return __ungetc(ch, (FILE *)File);
			}
			MISRA_EXCEPTION_RULE_14_7()
			return 0;

		case __TestForError:
			{
			int_t res = (ferror((struct _FILE *)File) || feof((struct _FILE *)File));
			MISRA_EXCEPTION_RULE_14_7()
			return res;
			}

		default:
			MISRA_EXCEPTION_RULE_14_7()
			return 0;
	}
}

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE
int_t _EWL_CDECL __ConsoleRead(void *File, int_t ch, int_t Action)
{
#pragma unused (File, ch)
	char_t readch;
	size_t size = sizeof readch;

	if (Action == (int_t)__GetAChar) {
		MISRA_EXCEPTION_RULE_11_4()
		MISRA_EXCEPTION_RULE_14_7()
     	return (__read_console(1UL, (uchar_t *)&readch, &size) == (int_t)__no_io_error) ? (int_t)readch : 0;	/* raw IO call */
    }
	return 0;
	MISRA_EXCEPTION_RULE_16_7()
	MISRA_QUIET_UNUSED_ARGS()
}
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */

int_t _EWL_CDECL __StringRead(void * isc, int_t ch, int_t Action)
{
	char_t	RetVal;

	__InStrCtrl * Iscp = (__InStrCtrl*)isc;
	switch(Action)
	{
		case __GetAChar:
			RetVal = *(Iscp->NextChar);
			if (RetVal == '\0') {
				Iscp->NullCharDetected = 1;
				MISRA_EXCEPTION_RULE_14_7()
				return EOF;
			} else {
				Iscp->NextChar++;
				MISRA_EXCEPTION_RULE_14_7()
				return (int_t)(uchar_t)RetVal;
			}

		case __UngetAChar:
			if (!Iscp->NullCharDetected) {
				Iscp->NextChar--;
			} else {
				Iscp->NullCharDetected = 0;
			}
			MISRA_EXCEPTION_RULE_14_7()
			return ch;

		case __TestForError:
			MISRA_EXCEPTION_RULE_14_7()
			return Iscp->NullCharDetected;

		default:
			MISRA_EXCEPTION_RULE_14_7()
			return 0;
	}
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL fscanf(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
	int_t			 retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
#if defined(__PPC_EABI__)
	va_list args;
#endif

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

#if defined(__PPC_EABI__)
	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if  (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start( args, format );
  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, args, 0);
#else
	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, __va_start(format), 0);
#endif
  	__end_critical_region(crtrgn);
	return(retval);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL fscanf_s(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
	int_t            retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
#if defined(__PPC_EABI__)
	va_list args;
#endif

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

#if defined(__PPC_EABI__)
	if ((file == NULL) || (format == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if  (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start( args, format );
  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, args, 1);
#else
	if ((file == NULL) || (format == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, __va_start(format), 1);
#endif
  	__end_critical_region(crtrgn);
	return(retval);
}


int_t _EWL_CDECL vscanf(const char_t * _EWL_RESTRICT format, va_list arg)
{
	int_t retval;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format,  arg, 0);
  	__end_critical_region(stdin_access);
	return(retval);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
#if defined(__PPC_EABI__)
	va_list args;

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start( args, format );
  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format,  args, 0);
#else

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format, __va_start(format), 0);
#endif
  	__end_critical_region(stdin_access);
	return(retval);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;
#if defined(__PPC_EABI__)
	va_list args;
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

	va_start( args, format );
  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format,  args, 1);
#else
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(stdin, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif

  	__begin_critical_region(stdin_access);
	retval = __sformatter(__FileRead, (void *)stdin, format, __va_start(format), 1);
#endif
  	__end_critical_region(stdin_access);
	return(retval);
}

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_console(const char_t * _EWL_RESTRICT format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_console(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;

#if defined(__PPC_EABI__)
	va_list args;

	va_start( args, format );
	retval = __sformatter(__ConsoleRead, (void *)0, format,  args, 0);
#else
	retval = __sformatter(__ConsoleRead, (void *)0, format, __va_start(format), 0);
#endif

	return(retval);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s_console(const char_t * _EWL_RESTRICT format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL scanf_s_console(const char_t * _EWL_RESTRICT format, ...)
{
	int_t retval;

#if defined(__PPC_EABI__)
	va_list args;
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	va_start( args, format );
	retval = __sformatter(__ConsoleRead, (void *)0, format,  args, 1);
#else
	if (format == NULL) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	retval = __sformatter(__ConsoleRead, (void *)0, format, __va_start(format), 1);
#endif

	return(retval);
}
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */

int_t _EWL_CDECL vfscanf(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, va_list arg)
{
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;
	int_t            retval;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}
	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	#if _EWL_C99_PRINTF_SCANF
	#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
	#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, arg, 0);
  	__end_critical_region(crtrgn);
	return(retval);
}

int_t _EWL_CDECL vfscanf_s(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, va_list arg)
{
	int_t            retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if ((file == NULL) || (format == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

#if _EWL_C99_PRINTF_SCANF
#if _EWL_WIDE_CHAR && _EWL_C99
	if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */
#endif /* _EWL_C99_PRINTF_SCANF */

  	__begin_critical_region(crtrgn);
	retval = __sformatter(__FileRead, (void *)file, format, arg, 1);
  	__end_critical_region(crtrgn);
	return(retval);
}

int_t _EWL_CDECL vsscanf(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, va_list arg)
{
	__InStrCtrl isc;

	MISRA_EXCEPTION_RULE_11_4()
	isc.NextChar         = (char_t *)s;
	if ((s == NULL) || (*isc.NextChar == '\0')) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	isc.NullCharDetected = 0;

	return(__sformatter(__StringRead, (void *)&isc, format, arg, 0));
}

int_t _EWL_CDECL vsscanf_s(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, va_list arg)
{
	__InStrCtrl isc;

	if ((s == NULL) || (format == NULL))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	MISRA_EXCEPTION_RULE_11_4()
	isc.NextChar         = (char_t *)s;
	isc.NullCharDetected = (*isc.NextChar == '\0');

	return(__sformatter(__StringRead, (void *)&isc, format, arg, 1));
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sscanf(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, ...)
{
#if defined(__PPC_EABI__)
	va_list args;

	va_start( args, format );
	return(vsscanf(s, format, args));
#else
	return(vsscanf(s, format, __va_start(format)));
#endif
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sscanf_s(const char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, ...)
{
#if defined(__PPC_EABI__)
	va_list args;

	va_start( args, format );
	return(vsscanf_s(s, format, args));
#else
	return(vsscanf_s(s, format, __va_start(format)));
#endif
}
