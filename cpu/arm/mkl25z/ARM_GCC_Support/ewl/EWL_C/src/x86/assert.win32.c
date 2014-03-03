/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/06 12:14:43 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#include <assert.h>
#include <assert_api.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>
#include <wchar.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* if set, assert() failures always put up dialog, even in console mode */
#ifndef _MSL_CONSOLE_ASSERT_DIALOG
	#define _MSL_CONSOLE_ASSERT_DIALOG 1
#endif

/*
 *	If you're here, you hit the "retry" button in an
 *	assertion failure dialog.  Step up the call stack
 *	to see where the call to __msl_assertion_failed() came from.
 */
static int AssertDialogW(wchar_t *title, wchar_t *buf)
{
	DWORD ret;
	ret = MessageBoxW(NULL, buf, title,
					MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);
	if (ret == IDRETRY)
	{
		asm int3		// allow debugger entry
		return 0;
	}
	else if (ret == IDIGNORE)
	{
		return 0;
	}
	
	// else, IDABORT
	return 1;
}

_MSL_IMP_EXP_C void _MSL_CDECL __wchar_assertion_failed(wchar_t const *condition, char const *filename, int lineno) _MSL_CANT_THROW;
void __wchar_assertion_failed(wchar_t const *condition, char const *filename, int lineno)
{
	__msl_wchar_assertion_failed(condition, filename, NULL, lineno);
}

void __msl_wchar_assertion_failed(wchar_t const *condition, char const *filename, char const *funcname, int lineno)
{
    #define BUFFSIZE 1024
	wchar_t buf[2*BUFFSIZE];
	int i;
	wchar_t tmpfile[BUFFSIZE];
	wchar_t tmpfunc[BUFFSIZE];
    char const *p;
	int do_abort;
#if !_MSL_CONSOLE_ASSERT_DIALOG
	HANDLE conerr;
	DWORD writ;
#endif /* !_MSL_CONSOLE_ASSERT_DIALOG */
	
	/* need to convert filename into a L string literal */
	i = 0;
    p=filename;
	while(*p != 0x00 && i < (BUFFSIZE-1)) 
	{
		tmpfile[i] = (wchar_t)*(p++);
        i++;
	}
	tmpfile[i]=0;
	
	/* need to convert funcname into a L string literal */
	if (funcname != NULL)
	{
		i = 0;
	    p=funcname;
		while(*p != 0x00 && i < (BUFFSIZE-1)) 
		{
			tmpfunc[i] = (wchar_t)*(p++);
	        i++;
		}
		tmpfunc[i]=0;
	}
	
#if !_MSL_CONSOLE_ASSERT_DIALOG
	/* 	Use MessageBox as a last resort.  The user doesn't need to
		see a dialog box when using console applications.  */
	
	if (funcname != NULL)
		swprintf(buf, sizeof(buf),
			L"Assertion (%ls) failed in \"%ls\", function \"%ls\", line %d\n",
			condition, tmpfile, tmpfunc, lineno);
	else
		swprintf(buf, sizeof(buf),
			L"Assertion (%ls) failed in \"%ls\", line %d\n",
			condition, tmpfile, lineno);

	conerr = GetStdHandle(STD_ERROR_HANDLE);
	if (conerr != 0 
	&& 	conerr != INVALID_HANDLE_VALUE
	&&	WriteFile(conerr, buf, wcslen(buf) * sizeof(wchar_t), &writ, NULL))
	{
		/* done */
		do_abort = 1;
	}
	else
#endif /* !_MSL_CONSOLE_ASSERT_DIALOG */

	{
		if (funcname != NULL)
		{
			swprintf(buf, sizeof(buf) + 1,
				L"Assertion (%ls) failed in \"%ls\", function \"%ls\", line %d\n",
				condition, tmpfile, tmpfunc, lineno);
		}
		else
			swprintf(buf, sizeof(buf) + 1,
				L"Assertion (%ls) failed in \"%ls\", line %d\n",
				condition, tmpfile, lineno);
		
		do_abort = AssertDialogW(L"Assertion Failure", buf);
	}
	
	if (!do_abort) return;
	
#if _WINSIOUX
	WinSIOUXAbort();
#else
	abort();
#endif /* _WINSIOUX */
}

#pragma mark -

/*
 *	If you're here, you hit the "retry" button in an
 *	assertion failure dialog.  Step up the call stack
 *	to see where the call to __msl_assertion_failed() came from.
 */
static int AssertDialogA(char *title, char *buf)
{
	DWORD ret;
	ret = MessageBoxA(NULL, buf, title,
					MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);
	if (ret == IDRETRY)
	{
		asm int3		// allow debugger entry
		return 0;
	}
	else if (ret == IDIGNORE)
	{
		return 0;
	}
	
	// else, IDABORT
	return 1;
}

_MSL_IMP_EXP_C void _MSL_CDECL __assertion_failed(char const *condition, char const *filename, int lineno) _MSL_CANT_THROW;
void __assertion_failed(char const *condition, char const *filename, int lineno)
{
	__msl_assertion_failed(condition, filename, NULL, lineno);
}

void _MSL_CDECL __msl_assertion_failed(char const *condition, char const *filename, char const *funcname, int lineno)
{
	char buf[4096];
	int do_abort;
	
#if !_MSL_CONSOLE_ASSERT_DIALOG
	/* 	Use MessageBox as a last resort.  The user doesn't need to
		see a dialog box when using console applications.  */

	HANDLE conerr;
	DWORD writ;

	if (funcname != NULL)
		snprintf(buf, sizeof(buf),
			"Assertion (%s) failed in \"%s\", function \"%s\", line %d\n",
			condition, filename, funcname, lineno);
	else
		snprintf(buf, sizeof(buf),
			"Assertion (%s) failed in \"%s\", line %d\n",
			condition, filename, lineno);

	conerr = GetStdHandle(STD_ERROR_HANDLE);
	if (conerr != 0 
	&& 	conerr != INVALID_HANDLE_VALUE
	&&	WriteFile(conerr, buf, strlen(buf), &writ, NULL))
	{
		/* done */
		do_abort = 1;
	}
	else
#endif /* !_MSL_CONSOLE_ASSERT_DIALOG */

	{
		if (funcname != NULL)
			snprintf(buf, sizeof(buf),
					"Assertion (%s) failed in \"%s\", function \"%s\", line %d\n"
					"\n"
					"Choose 'Abort' to end the program, 'Retry' to debug, or 'Ignore' to continue.\n",
					condition, filename, funcname, lineno);
		else
			snprintf(buf, sizeof(buf),
					"Assertion (%s) failed in \"%s\", line %d\n"
					"\n"
					"Choose 'Abort' to end the program, 'Retry' to debug, or 'Ignore' to continue.\n",
					condition, filename, lineno);
	
		do_abort = AssertDialogA("Assertion Failure", buf);
	}
	
	if (!do_abort) return;
	
#if _WINSIOUX					/*- mm 981109 -*/
	WinSIOUXAbort();			/*- mm 981109 -*/
#else							/*- mm 981109 -*/
	abort();
#endif/* _WINSIOUX */			/*- mm 981109 -*/
}


/* Change record:
 * JFH 950612 First code release.
 * MM  950616 Added include of signal.h and code to replace abort()
 * JFH 950918 Undid previous change. abort() should behave properly now.
 * JFH 960426 Merged Win32 changes in.
 *						CTV
 * hh  980122 Replaced <windows.h> with the following four includes because it is seriously
 *            broken.  The following 4 includes must be carefully ordered as shown, because
 *            they are broken too.
 * mf  980428 added support for windows ce-- do not need to explicitly include x86_prefix.h
 *            since it is included by ansi_parms.h
 * mf  980510 added new function __whar_assertion_failed to support unicode.  This was due to
 *            microsoft's version of swprintf not being standard. the ms implementation of tchar
 *            maps tsprintf into the ms implementation of swprintf if UNICODE is defined. our implementation
 *            is different(takes an int arg to limit size of string as specified in the standard.                       
 * mf  980512 wince changes   
 * mf  980515 needed windows headers before standard headers and therefore needed to also
 *            put include of ansi_parms first so that WIN32 env. is defined before prototype
 *            of MessageBox          
 * mm  981109 Changed __assertion_failed so that when __WINSIOUX defined, responding to 
 *     		  assertion failure box does not terminate WinSIOUX.
 * mm  990419 Changes to avoid messages when warnings when #pragma const_strings on in force IL9903-1579       
 * as  000823 added #if __dest_os == __mcore_bare to __assertion_failed()
 * MEA 001107 added ... || defined(_No_Disk_File_OS_Support)  to __assertion_failed()
 * cc  010821 Added eppc to core
 * JWW 010905 Moved to using platform specific implementations of __assertion_failed
 * JWW 010905 Renamed to __msl_assertion_failed so new C99 parameters won't conflict with old libraries
 * EJS 011024 Print assertion failure to stderr in console applications
 * EJS 011101 Make the no-dialog policy configurable, and revert to previous behavior by default.
 * cc  011203 Added _MSL_CDECL for new name mangling
 * JWW 020404 Don't display __func__ if funcname is NULL (because _MSL_ASSERT_DISPLAYS_FUNC is off)
 * JWW 020628 Added __assertion_failed function to call __msl_assertion_failed for backwards compatibility
 * JWW 021218 Remove the need to check for Windows-specific UNICODE define
 * JWW 030319 Changed incorrect %s (multibyte) specifiers for UNICODE assert to %ls (wchar_t)
 */