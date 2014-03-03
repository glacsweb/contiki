/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/29 13:41:59 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		_SetupEnv
 *		getenv
 *		getenv_s
 *		_putenv
 *		system
 */

#define __STDC_WANT_LIB_EXT1__ 1
#include <x86/crtl.h>
#include <stdint.h>
#include <stdlib.h>
#include <extras/extras.h>
#include <errno.h>
#include <string.h>
#include <file_io.h>
#include <x86/sysenv_api.h>
#include <wctype.h>

/*	
	Array of environment variables in the form "variable=value",
	case-insensitively sorted by variable.

	_environ_length does not include the trailing NULL.
*/

TCHAR **_tenviron;
static size_t _environ_length;
static size_t _environ_max;
#define ENVIRON_GROW 16

#if ( __dest_os==__wince_os)

void _tSetupEnv(void)
{
}

_TCHAR * _EWL_CDECL _tgetenv(const _TCHAR * /*inVarName*/)
{
	return 0;
}

errno_t getenv_s(size_t* _EWL_RESTRICT needed, char* _EWL_RESTRICT value, size_t maxsize,
	const char* _EWL_RESTRICT name)
{
	if ((name == NULL) || (maxsize > RSIZE_MAX) || ((maxsize > 0) && (value == NULL)))
	{
		__msl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		
		if (needed != NULL)
			*needed = 0;
		
		return ERANGE;
	}
	
	if (needed != NULL)
		*needed = 0;
	
	if (maxsize > 0)
		value[0] = 0;
	
	return ERANGE;
}

#else	// not wince
#pragma mark -

/*
 *	Unlike __msl_strnicmp, the environment is sorted by uppercase comparisons,
 *	so variables starting with '_' are at the end of the sort order.
 *	Also, we stop at '=' or end-of-string.
 */
static int __env_tstricmp(const _TCHAR *s1, const _TCHAR *s2)
{
	_TCHAR c1, c2;
	int equ=1;
	while (1)
	{
		c1 = _totupper(*s1++);
		c2 = _totupper(*s2++);
		if (c1 != c2) break;
		if (!c1 || c1=='=') return 0;
	}
	if (c1 == '=') c1 = 0;
	if (c2 == '=') c2 = 0;
	if (c1 == c2) return 0;
	return c1 - c2;
}


/*
 *	Free environment block 
 */
static void free_environ(void)
{
	if (_tenviron)
	{
		int i;
		for (i = 0; _tenviron[i]; i++)
			if (_tenviron[i]) 
				free(_tenviron[i]);
		free(_tenviron);
		_tenviron = 0L;
	}
}

/*
 * 	create _tenviron array
 */
static void init_environ(_TCHAR *envblock)
{
	_TCHAR *ptr;

	/* EJS 040518: this routine is called multiple times; only initialize once! */
	if (_tenviron) return;
	
	_environ_length = 0;
	_environ_max = _environ_length + ENVIRON_GROW;
	_tenviron = (_TCHAR **)malloc((_environ_max) * sizeof(_TCHAR *));
	_tenviron[0] = 0L;
	
	/* 	Add each entry to our sorted array (the incoming environment
		may LOOK sorted, but it's not guaranteed to be) */
	ptr = envblock;
	while (*ptr)
	{
		/* ignore current directory */
		if (*ptr != '=')
			__msl_tputenv(ptr);
		ptr += _tcslen(ptr) + 1;
	}
	
	/* cleanup at exit */
	atexit(free_environ);
}

#define CHUNK(val, sz) \
	((( (val) + (sz) - 1 ) / (sz)) * (sz))
	
static void resize_environ(size_t length)
{
	size_t oldmax = _environ_max;
	
	_environ_max = CHUNK(length+1, ENVIRON_GROW);
	if (_environ_max != oldmax)
	{
		_tenviron = realloc(_tenviron, sizeof(_TCHAR *) * _environ_max);
	}
	_environ_length = length;
}

/*
	Initialize environment strings block.
	
	Windows manages environment strings for us, but only provides ways to
	read a variable, set a variable, and get the entire block of strings.
	Windows/DOS programs want to manipulate an _tenviron array instead
*/
void _EWL_CDECL _tSetupEnv(void)
{
    _TCHAR *theEnvStrings;

	/* Get original environment variable string block */
	theEnvStrings = GetEnvironmentStrings();

	/* Add its vars... */
	init_environ(theEnvStrings);
	
	/* Release to Windows */
	FreeEnvironmentStrings(theEnvStrings);
}

/*
 *	Lookup an environment variable.  We maintain _tenviron in sorted order.
 *	Return the pointer and the index of the variable,
 *	or return NULL and the insertion index.
 */
static _TCHAR *search_env(const _TCHAR *inVarName, int *index)
{
	_TCHAR *ptr;
	int theVarNameLength = _tcslen(inVarName);
	int Min, Max;
	int StrInx = 0;
	int cmp = -1;
	
	Min = 0; Max = _environ_length - 1;
	while (Min <= Max)
	{
		StrInx = (Min + Max) >> 1;
		ptr = _tenviron[StrInx];

		cmp = __env_tstricmp(inVarName, ptr);
		if (cmp == 0)
		{
			/* The string matches. Return a pointer to the value. */
			*index = StrInx;
			return ptr;
		}
		else if (cmp < 0)
		{
			Max = StrInx - 1;
		}
		else
		{
			Min = StrInx + 1;
		}
	}
	*index = cmp < 0 ? StrInx : StrInx+1;
	return 0L;
}

_TCHAR * _EWL_CDECL _tgetenv(const _TCHAR * inVarName)
{
	_TCHAR *ptr;
	int StrInx = 0;
	int theVarNameLength = _tcslen(inVarName);

	if ((ptr = search_env(inVarName, &StrInx)) != 0)
		return ptr + theVarNameLength + 1;
	else
		return 0L;
}

#ifndef UNICODE
errno_t getenv_s(size_t* _EWL_RESTRICT needed, char* _EWL_RESTRICT value, size_t maxsize,
	const char* _EWL_RESTRICT name)
{
	char *ptr;
	int StrInx = 0;
	int theVarNameLength = strlen(name);
	
	if ((name == NULL) || (maxsize > RSIZE_MAX) || ((maxsize > 0) && (value == NULL)))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		
		if (needed != NULL)
			*needed = 0;
		
		return ERANGE;
	}
	
	if ((ptr = search_env(name, &StrInx)) != 0)
	{
		if (needed != NULL)
			*needed = strlen(ptr) + 1;
		
		if (maxsize < 1)
			return ERANGE;
		
		return strncpy_s(value, maxsize, ptr, maxsize);
	}
	
	if (needed != NULL)
		*needed = 0;
	if (maxsize > 0)
		value[0] = 0;
	return ERANGE;
}
#endif

/*
 *	Update the environment strings.
 *
 *	To define a new variable or redefine an old one: "varname=value";
 *	to delete a variable: "varname=".
 *
 *	We keep the strings in sorted order.
 */
int _EWL_CDECL __msl_tputenv(const _TCHAR * varSetting)
{
	/* scan the incoming string for the variable name and its value */
	const _TCHAR *equal_sign = _tcschr(varSetting, '=');

	if (equal_sign)
	{
		/* isolate variable and argument */
		int var_length = equal_sign - varSetting;
		const _TCHAR *arg = equal_sign + 1;
		int index;
		int edelta;
		
		/* 	scan for location to write new variable */
		if (search_env(varSetting, &index))
		{
			/* found existing element */
			if (!*arg)
			{
				/* if defining to "", remove entry */
				edelta = -1;
				varSetting = 0L;
			}
			else
			{
				/* replace entry */
				edelta = 0;
			}
			free(_tenviron[index]);
		}
		else if (*arg)
		{
			/* defining new string */
			edelta = 1;
		}
		else
		{
			/* deleting undefined variable */
			return 0;
		}
			
		/* resize _tenviron */
		if (edelta < 0)
		{
			memmove(&_tenviron[index], &_tenviron[index+1], (_environ_length - index) * sizeof(_TCHAR *));
			resize_environ(_environ_length + edelta);
		}
		else if (edelta > 0)
		{
			resize_environ(_environ_length + edelta);
			memmove(&_tenviron[index+1], &_tenviron[index], (_environ_length - index) * sizeof(_TCHAR *));
		}
		
		/* copy in new string */
		if (varSetting)
		{
			int len = _tcslen(varSetting);
			_tenviron[index] = malloc((len + 1) * sizeof(_TCHAR));
			_tcscpy(_tenviron[index], varSetting);
		}
	}
	else
	{
		/* error: malformed argument */
		return -1;	
	}

	return 0;	/* success */
}

#endif	// wince!

#pragma mark -

/*
	Get the command processor
*/
static const _TCHAR *get_comspec(void)
{
	const _TCHAR *comspec = _tgetenv(_T("COMSPEC"));
	if (!comspec)
	{
		/* 	fallback if COMSPEC is not set -- 
			get assumed command processor based on Windows version */
		OSVERSIONINFO osvi;
		
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		
		if (GetVersionEx (&osvi))
		{
			switch (osvi.dwPlatformId)
			{
			case VER_PLATFORM_WIN32_NT:
				comspec = _T("cmd.exe");
				break;

			case VER_PLATFORM_WIN32s:
			case VER_PLATFORM_WIN32_WINDOWS:
				comspec = _T("command.com");
				break;
			}
		}
	}
	
	return comspec;
}

/*
 *	Get a copy of the environment block.
 */
static _TCHAR *get_envblock(const _TCHAR *const *envp)
{
	int i;
	int len = 0;
	_TCHAR *block, *ptr;
	_TCHAR curdir[_MAX_PATH];
	
	/* count total length */
	for (i = 0; envp[i]; i++)
		len += _tcslen(envp[i]) + 1;
	
	/* current directory */
	if (!GetCurrentDirectory(sizeof(curdir)-4, curdir+4)) 
		*curdir = 0;
	else
	{
		curdir[0] = '=';
		curdir[1] = curdir[4];	/*- ejs 031212 -*/
		curdir[2] = ':';
		curdir[3] = '=';
		len += _tcslen(curdir) + 1;
	}
	
	/* terminator */
	len++;
	
	block = malloc(sizeof(_TCHAR) * len);
	if (!block) {
		errno = ENOMEM;
		return 0L;
	}
	
	/* copy each string consecutively */
	ptr = block;

	/* copy current directory */
	if (*curdir)
	{
		len = _tcslen(curdir) + 1;
		_tcsncpy(ptr, curdir, len);
		ptr += len;
	}
	
	for (i = 0; envp[i]; i++)
	{
		len = _tcslen(envp[i]) + 1;
		_tcsncpy(ptr, envp[i], len);
		ptr += len;
	}
	
	/* terminate block with null string */
	*ptr = 0;
	
	return block;
}


/*
	Execute 'cmdLine' in a process created with crFlags, 
	and return process information in piProcInfo. 
	
	Returns 0 for success, -1 for failure
*/
int _EWL_CDECL __msl_tsystem_core(const _TCHAR *comspec, const _TCHAR * cmdLine, const _TCHAR *const *envp,
						DWORD crFlags, PROCESS_INFORMATION *piProcInfo)
{
	int 		result = 0;
	STARTUPINFO	siStartInfo;
	_TCHAR *		real_cmdLine; 					/*- cc 000609 -*/
	_TCHAR *		envblock;
		
	/* construct full command line for Windows */
	if (comspec)
	{
		/* pass "%COMSPEC% /c cmdline" */
		real_cmdLine = malloc((_tcslen(comspec) + 4 + _tcslen(cmdLine) + 1) * sizeof(_TCHAR));
		if (!real_cmdLine)											/*- cc 001113 -*/
		{
			errno = ENOMEM;
			return ERROR_OUTOFMEMORY;
		}
		
		_tcscpy(real_cmdLine, comspec);
		_tcscat(real_cmdLine, _T(" /c "));  	/*- cc 000609 -*/
		_tcscat(real_cmdLine, cmdLine);			/*- cc 000609 -*/
	}
	else
	{
		/* just run "cmdline" */
		real_cmdLine = malloc((_tcslen(cmdLine) + 1)*sizeof(_TCHAR));
		if (!real_cmdLine)											/*- cc 001113 -*/
		{
			errno = ENOMEM;
			return ERROR_OUTOFMEMORY;
		}
		
		_tcscpy(real_cmdLine, cmdLine);
	}

	/* Create environment block */
	envblock = get_envblock(envp ? envp : _tenviron);
	
	/* Create the child process. */ 
    memset(&siStartInfo,0,sizeof(siStartInfo));
    siStartInfo.cb = sizeof(siStartInfo);
	siStartInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

	result = CreateProcess(	NULL,				/* application name */
							real_cmdLine,		/* command line */ /*- cc 001113 -*/
							NULL,				/* process security attributes */
							NULL,				/* primary thread security attributes */
							TRUE,				/* handles are inherited */
							crFlags,			/* creation flags */ /*- ejs 011116 -*/
							envblock,			/* use current environment */
							NULL,				/* working directory path */
							&siStartInfo,		/* STARTUPINFO pointer */
							piProcInfo);		/* receives PROCESS_INFORMATION */

	/* store error result */
	if (!result)
		__set_errno(GetLastError());
	else
		errno = _doserrno = 0;

	/* release memory allocated for environment block */
	free(envblock);
	free(real_cmdLine);/*- cc 001113 -*/

	return result ? 0 : -1;
}

int _EWL_CDECL _tsystem(const _TCHAR *cmdLine)
{
	const _TCHAR *comspec;
	
	/* get command processor */		
	comspec = get_comspec();
	
	/* 	if cmdLine is NULL, ANSI C says check the existence of the command processor,
		returning 1 if found, 0 otherwise.  Note, this fails if COMSPEC is not set,
		unless the shell lives in the current directory, since comspec is not a full path. */
	if (!cmdLine)
	{
		return comspec && GetFileAttributes(comspec) != 0xFFFFFFFF;
	}
	else
	{
		PROCESS_INFORMATION piProcInfo;
		DWORD exitcode;
		
		/* else launch the program and wait for it to complete. */
		if(__msl_tsystem_core(comspec, cmdLine, 0, 0, &piProcInfo) == 0)
		{
			WaitForSingleObject(piProcInfo.hProcess, INFINITE); 
			GetExitCodeProcess(piProcInfo.hProcess, &exitcode);
			CloseHandle(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
			return exitcode;
		} 
		else 
		{
			return -1;
		}
	}
}

/* Change record:
 * KO  961211 Wrote this file.
 * mm  990115 Rewrote getenv() to make use of _environ
 * vss 990121 Moved system function to sysenv.c
 * mm  990125 Change to allow compilation without warning message
 * blc 990922 Added _putenv, modified system to use runtime environment
 * cc  000221 Added errno.h 
 * cc  000512 Added #include<extras.h>
 * cc  000609 Changed system to take in path to shell plus commmand line
 * cc  000612 Added case for Win2000 shell
 * cc  001113 Added fix so the value of COMSPEC will not be overwritten
 * cc  001113 Added free(real_cmdLine)
 * cc  010605 Moved _putenv to extras.win32.c
 * cc  010605 Made calls equal to _strnicmp call __msl_strnicmp & added #include 
 *			  file_io.h
 * ejs 011116 Rewrote system()
 * ejs 011119 Rewrote environment handling code
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * ejs 030721 Added UNICODE support
 * JWW 031030 Added the __STDC_WANT_LIB_EXT1__ secure library extensions
 * ejs 031212 Fix bug with passing current directory in environment block
 * JWW 040317 Updated the __STDC_WANT_LIB_EXT1__ secure library based on new secure specifications
 * ejs 040518 Do init_environ() work only once
 */