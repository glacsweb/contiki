/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/29 13:41:59 $
 * $Revision: 1.4 $
 */

/*
 *	Routines
 *	--------
 * 		_CRTStartup
 */

#include <x86/startup.win32.h>
#include <critical_regions.h>
#include <x86/crtl.h>
#include <errno.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <extras/extras.h>
#include <stdlib.h>
#if _EWL_THREADSAFE						/*- mm 010521 -*/
	#include <critical_regions.h>
#endif									/*- mm 010521 -*/
#include <x86/threadlocaldata.h>			/*- cc 011207 -*/
#include <abort_exit.h>
#include <pool_alloc.h>
#include <file_struc.h>

int _doserrno = 0;

FileStruct *_HandleTable[NUM_HANDLES];

int _HandPtr;

/*
 * Core functionality for dup() and startup code.
 * Returns 0 for success or Win32 error code.
 */
unsigned long _EWL_CDECL __dup_core(void *handle, void **dup_handle)
{
	if (!handle
	||	(handle == INVALID_HANDLE_VALUE)
	||	!DuplicateHandle(
				GetCurrentProcess(),
				handle,
				GetCurrentProcess(), 
				dup_handle,
				SYNCHRONIZE,
				TRUE,
				DUPLICATE_SAME_ACCESS))
	{
		// there was an error
		*dup_handle = INVALID_HANDLE_VALUE;
		return GetLastError();
	}
	else
	{
		return 0;
	}
}

/*	Map Win32 error to errno */
int _EWL_CDECL __set_errno(DWORD err)
{
	if (err != ERROR_SUCCESS)
	{
		/* library functions may not reset errno to 0 */
		_doserrno = err;
	}
	
	switch (err)
	{
	case ERROR_SUCCESS:	
		/*errno = ENOERR;*/
		/* library functions may not reset errno to 0 */
		break;

	case ERROR_INVALID_FUNCTION:
		errno = EINVAL; break;

	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
	case ERROR_BAD_NETPATH:
	case ERROR_DEV_NOT_EXIST:
		errno = ENOENT; break;

	case ERROR_TOO_MANY_OPEN_FILES:
		errno = ENFILE; break;

	case ERROR_ACCESS_DENIED:
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
		errno = EPERM; break;

	case ERROR_INVALID_HANDLE:
		errno = EBADF; break;

	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
	case ERROR_SHARING_BUFFER_EXCEEDED:
		errno = ENOMEM; break;

	case ERROR_BAD_ENVIRONMENT:
		errno = EINVAL; break;

	case ERROR_BAD_FORMAT:
		errno = ENOEXEC; break;

	case ERROR_INVALID_ACCESS:
	case ERROR_INVALID_DATA:
		errno = EINVAL; break;

	case ERROR_INVALID_DRIVE:
		errno = ENOENT; break;

	case ERROR_CURRENT_DIRECTORY:
		errno = EEXIST; break;

	case ERROR_NOT_SAME_DEVICE:
		errno = EXDEV; break;

	case ERROR_NO_MORE_FILES:
		errno = EMFILE; break;

	case ERROR_WRITE_PROTECT:
		errno = EACCES; break;

	case ERROR_NOT_READY:
		errno = ENXIO; break;

	case ERROR_BAD_COMMAND:
		errno = ENODEV; break;

	case ERROR_CRC:
	case ERROR_SEEK:
	case ERROR_SECTOR_NOT_FOUND:
	case ERROR_OUT_OF_PAPER:
	case ERROR_WRITE_FAULT:
	case ERROR_READ_FAULT:
	case ERROR_GEN_FAILURE:
	case ERROR_NOT_DOS_DISK:
		errno = EIO; break;

	case ERROR_BAD_LENGTH:
		errno = ENAMETOOLONG; break;

	case ERROR_HANDLE_DISK_FULL:
		errno = ENOSPC; break;

	case ERROR_NOT_SUPPORTED:
		errno = ENOSYS; break;

	case ERROR_NETWORK_BUSY:
		errno = EBUSY; break;

	case ERROR_BUFFER_OVERFLOW:
		errno = ERANGE; break;
		
	default:
		errno = EUNKNOWN; break;
	}
	return errno;
}

/* used in abort_exit.c to get counter -- needed for DLL interface */
int * _EWL_CDECL __get_EWL_init_count(void)
{
	static int __EWL_init_count = 0;			/*- blc 980812 -*/
	return &__EWL_init_count;
}

static void _InitStdHandles(void)
{
	HANDLE handle;
	HANDLE msl_handle;

	_HandleTable[0] = (FileStruct *)malloc(sizeof(FileStruct));
	if (_HandleTable[0])
	{
		handle = GetStdHandle(STD_INPUT_HANDLE);
		
		// Duplicate the standard handle, don't copy it.
		// This allows us to close it properly at shutdown.
		if (__dup_core(handle, &msl_handle) != NO_ERROR) {
			_HandleTable[0]->handle = INVALID_HANDLE_VALUE;		
	#if !_WINSIOUX			
			stdin->mode.file_kind = __closed_file;	/*- ejs 011120 -*/
	#endif
		}
		else
		    _HandleTable[0]->handle = msl_handle;
		    
	   	_HandleTable[0]->translate = 1;	/*- KO 961208 -*/
	   	_HandleTable[0]->append = 0;
	}
	_HandleTable[1] = (FileStruct *)malloc(sizeof(FileStruct));
	if (_HandleTable[1])
	{
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		
		// Duplicate the standard handle, don't copy it.
		// This allows us to close it properly at shutdown.
		if (__dup_core(handle, &msl_handle) != NO_ERROR) {
			_HandleTable[1]->handle = INVALID_HANDLE_VALUE;		
	#if !_WINSIOUX			
			stdout->mode.file_kind = __closed_file;	/*- ejs 011120 -*/
	#endif
		}
		else
		    _HandleTable[1]->handle = msl_handle;

	    _HandleTable[1]->translate = 1;	/*- KO 961208 -*/
	    _HandleTable[1]->append = 0;
	}
	_HandleTable[2] = (FileStruct *)malloc(sizeof(FileStruct));
	if (_HandleTable[2])
	{
		handle = GetStdHandle(STD_ERROR_HANDLE);
		
		// Duplicate the standard handle, don't copy it.
		// This allows us to close it properly at shutdown.
		if (__dup_core(handle, &msl_handle) != NO_ERROR) {
			_HandleTable[2]->handle = INVALID_HANDLE_VALUE;		
	#if !_WINSIOUX			
			stderr->mode.file_kind = __closed_file;	/*- ejs 011120 -*/
	#endif
		}
		else
		    _HandleTable[2]->handle = msl_handle;

		_HandleTable[2]->translate = 1;	/*- KO 961208 -*/
	    _HandleTable[2]->append = 0;
	}
	
	/* Next available handle is 3 */
	
	_HandPtr = 3;
}

/*	Start up the C library and runtime.  Return 1 for success. */
int _EWL_CDECL _CRTStartup(void)
{
    /* Only initialize once, since we might be called by DLLs and the main */
    if ((*__get_EWL_init_count())++) 
    	return 1;	/*- blc 980811 -*/

	__init_critical_regions(); 

	/* Initialize the thread local data structure. */
	if (!_InitializeMainThreadData()) {
		/* Initialization failed. */
		return 0;
	}

	/* Setup stdout, stdin, stderr */
	_InitStdHandles();
		
	return 1;
}

/* This function should be equivalent to the ANSI "exit" without the ExitProcess
 * call. This function is needed so that a DLL can clean up itself and return
 * to Windows which finishes cleaning up the process (other DLLs and the main
 * application might not have destructed yet).
 * I would have just separated this code out of exit but that code is already too
 * separated...
 */
 
 /* 
    for the shared version of msl on windows _CleanUpEWL must be contained only
    in the shared library.  It was moved to this file for that purpose only.
    The reason is that each object attached to a process including msl, must
    have it's own destructor chain.  
    
    The destructor chain inside _CleanupEWL() now calls destructors for EWL only
    (including the C and C++ library).  The whole purpose of this routine is to 
    do nothing until we are sure that all objects (a process and possibly other dll's)
    which are attached to the shared msl have had a chance to clean themselves up.
    The msl DLL now has it's own version of abort and exit which refer only to the
    destructor chain of msl.  abort and exit are statically linked in by things 
    linking against the shared msl so that abort and exit will reference their
    individual static objects when calling their own destructor chain.
    
    Note this change assumes there are exactly two distinct ways that a dll unloads.
    
    1. When the process exits through ExitProcess, the dll receives a DLL_PROCESS_DETACH 
    message.
    
    2. The dll was loaded manually by LoadLibrary and unloaded by FreeLibrary.  FreeLibrary
    will also send a DLL_PROCESS_DETACH message to the dll entry point which MUST call
    __destroy_global_chain();  (see _CRT_INIT(HINSTANCE, DWORD, LPVOID) in dllmaincrt.c)
    
    I see no compelling reason nor a safe way to have a single destructor chain.
    
    m.f.  000807
 */
void _EWL_CDECL _CleanUpEWL(void)
{
	if (--(*__get_EWL_init_count()) > 0) 
	  return ;		
	
	__destroy_global_chain();  /* __destroy_global_chain(); needs to be called before
	                              destroying the standard handles, since some destructor
	                              called by __destroy_global_chain may be legally using
	                              one of these handles
	                           */
	
	if (__stdio_exit)
	{
		(*__stdio_exit)();
		__stdio_exit = 0;
	}
	
	if (__console_exit)
	{
		(*__console_exit)();
		__console_exit = 0;
	}
	
	{							/* explicitly free up the memory malloced by _InitStdHandles */
		int i;
		for (i = 0; i < 3; i++)
		if (_HandleTable[i])
		{
			free(_HandleTable[i]);
			_HandleTable[i] = NULL;
		}
	}
	
#if _EWL_THREADSAFE								/*- mm 010521 -*/
	 _DisposeAllThreadData();
	__kill_critical_regions();
#endif									/*- mm 010521 -*/

#if !(_EWL_CLASSIC_MALLOC || _EWL_PRO4_MALLOC)
	__malloc_free_all();	
#else
	__pool_free_all(&__malloc_pool);
#endif

	_DisposeThreadDataIndex();
}

/* Change record:
 * KO  961208 Initialized the translate field in the FileStructs for stdin, stdout,
 * 			  and stderr. Now EOLs should always be properly converted between
 * 			  '\n's.
 * KO  961216 Changed the initialization of environ. Now the strings are no longer
 *			  converted to uppercase (getenv doesn't rely on that so there
 * 			  is no reason to). Made the global Initialized local to _CRTStartup.
 * hh  980122 Replaced <windows.h> with the following TWO includes because it is seriously
 *            broken.  The following 2 includes must be carefully ordered as shown, because
 *            they are broken too.
 * mf  980810 (& blc) Fix to x86 runtime dll crash(when > 1 dll is attached to EWL)
 * gcm 980828 Free env strings - fixes a memory leak when loading and unloading
 *            multiple dll's.
 * vss 981210 Want the Environ string to actually be an array of pointers to environ strings
 * mm  990115 Changed Environ to _Environ
 * vss 990507 Clean up the code a bit
 * blc 991007 Fixed memory leak by dropping call to GetEnvironmentStrings()
 * cc  000515 Fixed #include
 * cc  000712 Added JC fix validate the handle via GetHandleInformation().
 * cc  000714 Added JC Fix to check for valid handle .
 * ejs 000724 Reverted JC's fixes and duplicated the standard handles instead.
 *            GetHandleInformation() doesn't accept console handles (as passed under cygwin)
 *            until Win2000.  Also, duplicating allows us to close the handle at shutdown.
 *            It's not clear whether an application is allowed to close a standard handle.
 * cc  010601 Added _doserrno, _HandleTable, _HandPtr, __dup_core, _CleanUpEWL - needed by EWL C
 * ejs 011116 Moved environment setup to sysenv.win32.c
 * ejs 011120 Mark unopened console filehandles as __closed_file to avoid closing them later
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * ejs 011206 Cleanup of init/term code
 * cc  011207 Moved #include <threadlocaldata.h> needed for _InitializeMainThreadData _CRTStartup
 * ejs 020114 Avoid resetting errno to 0 (C9X 7.5.3)
 * JWW 020130 Changed _MWMT to _EWL_THREADSAFE for consistency's sake
 * ejs 020207 Fix broken _WINSIOUX build: std handles should not be marked as __closed_file
 * ejs 030407 Fix problem duplicating bad handles
 * ejs 030721 Move SetupArgs/SetupEnv call to runtime
 */
