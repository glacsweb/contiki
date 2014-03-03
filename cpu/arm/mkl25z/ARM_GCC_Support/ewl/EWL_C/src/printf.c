/*
 * Copyright © 2008 Freescale Corporation.  All rights reserved.
 *
 */

/*    Functions:
 *    ---------
 *        __FileWrite
 *        __StringWrite
 *
 *    Routines
 *    --------
 *
 *        vfprintf
 *        vfprintf_s
 *        fprintf
 *        fprintf_s
 *
 *        vprintf
 *        vprintf_s
 *        printf
 *        printf_s
 *
 *        vsprintf
 *        vsprintf_s
 *        vsnprintf
 *        vsnprintf_s
 *        sprintf
 *        sprintf_s
 *        snprintf
 *        snprintf_s
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>
#include <critical_regions.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#if _EWL_WIDE_CHAR
#include <wchar.h>
#endif
#include <console_io.h>

extern int_t __pformatter(void *(_EWL_CDECL *WriteProc)(void *a, const char_t *b, size_t c),
                 void * WriteProcArg,
                 const char_t * _EWL_RESTRICT format_str,
                 va_list arg,
                 int_t is_secure);

void * _EWL_CDECL __FileWrite(void *File, const char_t * Buffer, size_t NumChars)
{
    return ((__fwrite(Buffer, 1U, NumChars, (FILE *)File) == NumChars) ? File : 0U);	/* std IO call */
}

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE
void * _EWL_CDECL __ConsoleWrite(void *File, const char_t * Buffer, size_t NumChars)
{
#pragma unused(File)
	MISRA_EXCEPTION_RULE_11_3()
	MISRA_EXCEPTION_RULE_11_4()
    return ((__write_console(1UL, (uchar_t *)Buffer, &NumChars) == (int_t)__no_io_error) ? (void *)1 : 0);	/* raw IO call */
	MISRA_EXCEPTION_RULE_16_7()
	MISRA_QUIET_UNUSED_ARGS()
}
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */

void * _EWL_CDECL __StringWrite(void * osc, const char_t * Buffer, size_t NumChars)
{
    size_t CharsToBeWritten;

    __OutStrCtrl * Oscp = (__OutStrCtrl*)osc;

    CharsToBeWritten = ((Oscp->CharsWritten + NumChars) <= Oscp->MaxCharCount) ? NumChars :
                                                            (Oscp->MaxCharCount - Oscp->CharsWritten);
    memcpy(Oscp->CharStr + Oscp->CharsWritten, Buffer, CharsToBeWritten);
    Oscp->CharsWritten += CharsToBeWritten;

    MISRA_EXCEPTION_RULE_11_3()
    return ((void*)1);
}


#if _EWL_CONSOLE_SUPPORT
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf(const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;

    #if _EWL_WIDE_CHAR
    #if _EWL_C99_PRINTF_SCANF
    if (fwide(stdout, -1) >= 0)
    {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
    #endif /*_EWL_C99_PRINTF_SCANF */
    #endif /* _EWL_WIDE_CHAR && _EWL_C99 */

    __begin_critical_region(stdout_access);

    #if defined(__PPC_EABI__)
    {
        va_list args;
        va_start( args, format );
        result = __pformatter(__FileWrite, (void *)stdout, format, args, 0);
    }
    #else
        result = __pformatter(__FileWrite, (void *)stdout, format, __va_start(format), 0);
    #endif    /* __PPC_EABI__ || __MIPS__  */

      __end_critical_region(stdout_access);

    return(result);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s(const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;

    if (format == NULL) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }

    #if _EWL_WIDE_CHAR
    #if _EWL_C99_PRINTF_SCANF
    if (fwide(stdout, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
    	return(-1);
    }
    #endif /* _EWL_C99_PRINTF_SCANF */
    #endif /* _EWL_WIDE_CHAR */

    __begin_critical_region(stdout_access);

    #if defined(__PPC_EABI__)
    {
        va_list args;
        va_start( args, format );
        result = __pformatter(__FileWrite, (void *)stdout, format, args, 1);
    }
    #else
        result = __pformatter(__FileWrite, (void *)stdout, format, __va_start(format), 1);
    #endif    /* __PPC_EABI__ || __MIPS__  */

      __end_critical_region(stdout_access);

    return(result);
}

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_console(const char_t * format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_console(const char_t * format, ...)
{
    int_t result;

    #if defined(__PPC_EABI__)
    {
        va_list args;
        va_start( args, format );
        result = __pformatter(__ConsoleWrite, (void *)0, format, args, 0);
    }
    #else
		result = __pformatter(__ConsoleWrite, (void *)0, format, __va_start(format), 0);
    #endif    /* __PPC_EABI__ */

      __end_critical_region(stdout_access);

    return(result);
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s_console(const char_t * format, ...);
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL printf_s_console(const char_t * format, ...)
{
    int_t result;

    if (format == NULL)
    {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }

    #if defined(__PPC_EABI__)
    {
        va_list args;
        va_start( args, format );
        result = __pformatter(__ConsoleWrite, (void *)0, format, args, 1);
    }
    #else
    result = __pformatter(__ConsoleWrite, (void *)0, format, __va_start(format), 1);
    #endif    /* __PPC_EABI__ || __MIPS__  */

    return(result);
}
#endif /* _EWL_CONSOLE_FILE_IS_DISK_FILE */
#endif /* _EWL_CONSOLE_SUPPORT */

#if _EWL_OS_DISK_FILE_SUPPORT
MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL fprintf(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;
	MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions crtrgn;

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(file, -1) >= 0) {
        return(-1);
	}
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    if (file == stdout) {
        crtrgn = stdout_access;
	} else if (file == stderr) {
        crtrgn = stderr_access;
	} else {
        crtrgn = files_access;
	}
  __begin_critical_region(crtrgn);

#if defined(__PPC_EABI__)
    {
		va_list args;

		va_start( args, format );
		result = __pformatter(__FileWrite, (void *)file, format, args, 0);
    }
#else
	result = __pformatter(__FileWrite, (void *)file, format, __va_start(format), 0);
#endif    /* __PPC_EABI__ || __MIPS__ */

    __end_critical_region(crtrgn);

    return(result);                                        /* -mm 001013 -*/
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL fprintf_s(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, ...)
{
    int_t result;
	MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions crtrgn;

    if ((file == NULL) || (format == NULL)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        return(-1);
    }

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(file, -1) >= 0) {
        return(-1);
	}
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    if (file == stdout) {
        crtrgn = stdout_access;
	} else if (file == stderr) {
        crtrgn = stderr_access;
	} else {
        crtrgn = files_access;
	}
    __begin_critical_region(crtrgn);

#if defined(__PPC_EABI__)
    {
		va_list args;

		va_start( args, format );
		result = __pformatter(__FileWrite, (void *)file, format, args, 1);
    }
#else
    result = __pformatter(__FileWrite, (void *)file, format, __va_start(format), 1);
#endif    /* __PPC_EABI__ */

    __end_critical_region(crtrgn);

    return(result);                                        /* -mm 001013 -*/
}
#endif /* _EWL_OS_DISK_FILE_SUPPORT */


int_t _EWL_CDECL vprintf(const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(stdout, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
#endif /* _EWL_WIDE_CHAR */
#endif /* _EWL_C99_PRINTF_SCANF */

    __begin_critical_region(stdout_access);
    retval = __pformatter(__FileWrite, (void *)stdout, format, arg, 0);
    __end_critical_region(stdout_access);
    return(retval);
}


int_t _EWL_CDECL vprintf_s(const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;

    if (format == NULL) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(stdout, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    __begin_critical_region(stdout_access);
    retval = __pformatter(__FileWrite, (void *)stdout, format, arg, 1);
    __end_critical_region(stdout_access);
    return(retval);
}


int_t _EWL_CDECL vfprintf(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions crtrgn;

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    if (file == stdout) {
        crtrgn = stdout_access;
    } else if (file == stderr) {
        crtrgn = stderr_access;
    } else {
        crtrgn = files_access;
    }
    __begin_critical_region(crtrgn);
    retval = __pformatter(__FileWrite, (void *)file, format, arg, 0);
    __end_critical_region(crtrgn);
    return(retval);
}


int_t _EWL_CDECL vfprintf_s(FILE * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
    critical_regions crtrgn;

    if ((file == NULL) || (format == NULL)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

#if _EWL_WIDE_CHAR
#if _EWL_C99_PRINTF_SCANF
    if (fwide(file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
        return(-1);
    }
#endif /* _EWL_C99_PRINTF_SCANF */
#endif /* _EWL_WIDE_CHAR */

    if (file == stdout) {
        crtrgn = stdout_access;
    } else if (file == stderr) {
        crtrgn = stderr_access;
    } else {
        crtrgn = files_access;
    }
    __begin_critical_region(crtrgn);
    retval = __pformatter(__FileWrite, (void *)file, format, arg, 1);
    __end_critical_region(crtrgn);
    return(retval);
}


int_t _EWL_CDECL vsnprintf(char_t * _EWL_RESTRICT s, size_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t          end;
    __OutStrCtrl osc;

    osc.CharStr      = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0u;

    end = __pformatter(__StringWrite, &osc, format, arg, 0);

    if (s != 0L) {
        if ((size_t)end < n) {
            s[end] = '\0';
        } else { if (n > 0U) {
            s[n - 1U] = '\0';
        }}
    }

    return(end);
}


int_t _EWL_CDECL vsnprintf_s(char_t * _EWL_RESTRICT s, size_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t       end;
    __OutStrCtrl osc;

    if ((s == NULL) || (format == NULL) || (n == 0U) || (n > (size_t)RSIZE_MAX)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        if ((s != NULL) && (n > 0U) && (n <= (size_t)RSIZE_MAX)) {
            s[0] = '\0';
        }
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

    osc.CharStr      = s;
    osc.MaxCharCount = n;
    osc.CharsWritten = 0u;

    end = __pformatter(__StringWrite, &osc, format, arg, 1);

    if (end >= 0) {
        if ((size_t)end < n) {
            s[end] = '\0';
        } else {
            s[n - 1U] = '\0';
        }
    }

    return(end);
}


int_t _EWL_CDECL vsprintf(char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, va_list arg)
{
    return(vsnprintf(s, (uint_t)ULONG_MAX, format, arg));
}


int_t _EWL_CDECL vsprintf_s(char_t * _EWL_RESTRICT s, rsize_t n, const char_t * _EWL_RESTRICT format, va_list arg)
{
    int_t retval;

    retval = vsnprintf_s(s, n, format, arg);

    if ((retval >= 0) && (n > 0U) && (n < (rsize_t)RSIZE_MAX) && ((rsize_t)retval >= n)) {
        __ewl_runtime_constraint_violation_s(NULL, NULL, -1);
        s[0] = '\0';
		MISRA_EXCEPTION_RULE_14_7()
        return -1;
    }

    if ((retval < 0) && (s != NULL) && (n > 0U) && (n < (rsize_t)RSIZE_MAX)) {
        s[0] = '\0';
    }

    return retval;
}


MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL snprintf(char_t * _EWL_RESTRICT s, size_t n, const char_t * _EWL_RESTRICT format, ...)
{
#if defined(__PPC_EABI__)
    va_list args;
    va_start( args, format );
    return(vsnprintf(s, n, format, args));
#else
    return(vsnprintf(s, n, format, __va_start(format)));
#endif /* __PPC_EABI__ || __MIPS__ */
}


MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL snprintf_s(char_t * _EWL_RESTRICT s, rsize_t n, const char_t * _EWL_RESTRICT format, ...)
{
#if defined(__PPC_EABI__)
    va_list args;
    va_start( args, format );
    return(vsnprintf_s(s, n, format, args));
#else
    return(vsnprintf_s(s, n, format, __va_start(format)));
#endif /* __PPC_EABI__ || __MIPS__ */
}


MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sprintf(char_t * _EWL_RESTRICT s, const char_t * _EWL_RESTRICT format, ...)
{
  #if defined(__PPC_EABI__)
        va_list args;

        va_start( args, format );
        return(vsnprintf(s, ULONG_MAX, format, args));
  #else
    return(vsnprintf(s, (uint_t)ULONG_MAX, format, __va_start(format)));
  #endif /* __PPC_EABI__ || __MIPS__ */
}

MISRA_EXCEPTION_RULE_16_1()
int_t _EWL_CDECL sprintf_s(char_t * _EWL_RESTRICT s, rsize_t n, const char_t * _EWL_RESTRICT format, ...)
{
  #if defined(__PPC_EABI__)
        va_list args;

        va_start( args, format );
        return(vsprintf_s(s, n, format, args));
  #else
    return(vsprintf_s(s, n, format, __va_start(format)));
  #endif /* __PPC_EABI__ || __MIPS__ */
}
