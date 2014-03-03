/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/29 13:41:59 $
 * $Revision: 1.3 $
 */

#include <stdlib.h>

#if _EWL_THREADSAFE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <x86/process.h>	// for _beginthread, _beginthreadex, _endthread, and
						// _endthreadex declarations

#include "x86/ThreadLocalData.h"

static unsigned long WINAPI _ThreadStart(void  *lpThreadParameter);
static unsigned long WINAPI _ThreadStartEx(void  *lpThreadParameter);

// An internal structure used to pass information from _beginthreadex to
// _ThreadStart.

typedef struct {
	LPVOID mCodeAddress;	// address of either an unsigned (__stdcall *)(void*) 
							// or an unsigned (__cdecl *) (void*)
	HANDLE mThreadHandle;
	LPVOID mParameter;
} _ThreadCreationInfo;

static unsigned long begin_thread_implementation (
        void *inSecurity,
        unsigned inStacksize,
        void *inCodeAddress,
        void *inParameter,
        unsigned int inCreationFlags,
        unsigned int *inThreadID,
		unsigned long __stdcall (*inStartCode)(void*)
        )		
{
	HANDLE threadHandle;

	_ThreadCreationInfo * theThreadCreationInfo = 
			(_ThreadCreationInfo *)(malloc(sizeof(_ThreadCreationInfo)));
	
	if (!theThreadCreationInfo) {
		return 0;
	}
	
	theThreadCreationInfo->mCodeAddress = inCodeAddress;			
	theThreadCreationInfo->mParameter = inParameter;			
	
	threadHandle = CreateThread(
		inSecurity,								// lpThreadAttributes
		inStacksize,							// dwStackSize
		inStartCode,							// lpStartAddress
		theThreadCreationInfo,					// lpParameter
		inCreationFlags | CREATE_SUSPENDED,		// dwCreationFlags
		(LPDWORD)inThreadID						// lpThreadId
	);

	// pass the thread handle value through the special parameter
	theThreadCreationInfo->mThreadHandle = threadHandle;
	
	// resume the thread now if the original caller didn't specify that
	// it should be suspended
	if (!(inCreationFlags & CREATE_SUSPENDED))
	{
		ResumeThread(threadHandle);
	}
	
	return (unsigned long) threadHandle;
}

unsigned long _EWL_CDECL _beginthreadex (
        void *inSecurity,
        unsigned int inStacksize,
        unsigned int (__stdcall *inCodeAddress) (void*) ,
        void *inParameter,
        unsigned int inCreationFlags,
        unsigned int *inThreadID
        )		
{
	return begin_thread_implementation(
		inSecurity,
		inStacksize,
		(void *)inCodeAddress,
		inParameter,
		inCreationFlags,
		inThreadID,
		_ThreadStartEx);
}

VOID _EWL_CDECL _endthreadex(UINT inReturnCode)
{
	_DisposeThreadData();
	
	ExitThread(inReturnCode);
}

unsigned long _EWL_CDECL _beginthread(
		void (__cdecl *inCodeAddress)(void *),
		unsigned int inStackSize,
		void *inParameter
		)
{
	unsigned int threadID;

	return begin_thread_implementation(
		NULL,
		inStackSize,
		(void *)inCodeAddress,
		inParameter,
		0,
		&threadID,
		_ThreadStart);
}

VOID _EWL_CDECL _endthread(void)
{
	CloseHandle(_GetThreadLocalData(1)->thread_handle);	/*- cc 010531 -*/
	_DisposeThreadData();
	
	ExitThread(0);
}

// This is the function _beginthreadex uses as the starting address of all
// new threads. This does some thread initialization and then calls the 
// user's code.
static unsigned long WINAPI _ThreadStartEx(void *inParameter)
{
	unsigned long retcode;
	
	_ThreadCreationInfo * theThreadCreationInfo = (_ThreadCreationInfo *)inParameter;
	HANDLE threadHandle = theThreadCreationInfo->mThreadHandle;
	LPVOID theUserParameter = theThreadCreationInfo->mParameter;
	unsigned int (__stdcall *theUserCode)(void *) = theThreadCreationInfo->mCodeAddress;
		
	// This block was malloc'ed by _beginthreadex and should be free'd here.
	free(theThreadCreationInfo);
	
	_InitializeThreadData(threadHandle);
	
	// EJS 040113: need minimal SEH frame to enable SEH handling in thread.
	__try {
		retcode = (*theUserCode)(theUserParameter);
	} __except(EXCEPTION_CONTINUE_EXECUTION) {
	}

	// if the user returns without ending itself, end the thread here
	_endthreadex(retcode);
	
	return 0;  /*- vss 990827 -*/
}

// This is the function _beginthread uses as the starting address of all
// new threads. This does some thread initialization and then calls the 
// user's code.
static unsigned long WINAPI _ThreadStart(void *inParameter)
{
	_ThreadCreationInfo * theThreadCreationInfo = (_ThreadCreationInfo *)inParameter;
	HANDLE threadHandle = theThreadCreationInfo->mThreadHandle;
	LPVOID theUserParameter = theThreadCreationInfo->mParameter;
	void (__cdecl *theUserCode)(void *) = theThreadCreationInfo->mCodeAddress;
	
	// This block was malloc'ed by _beginthreadex and should be free'd here.
	free(theThreadCreationInfo);
	
	_InitializeThreadData(threadHandle);
	
	// if the user returns without ending itself, end the thread here
	(*theUserCode)(theUserParameter);
	_endthread();
	return 0;  /*- vss 990827 -*/
}

#endif /* _EWL_THREADSAFE */

/* Change record:
 * blc 031098 made _ThreadStart call _endthreadex if thread returns without exiting
 * mf  020398 made _beginthreadex/_endthreadex compatible with microsoft's api.  This
 *            eliminates the need for process.h in the EWL headers
 * rjk 980313 removed syntax error in call to _endthreadex.
 * mf  980527 _begin/endthreadex follow the prototypes in process.h
 * mf  990301 wrapped file w/#if defined(_MWMT) to prevent compilation into single threaded
 *            lib.
 * blc 990308 moved check of _MT to after include of stdlib.h so we could
 *            correctly pick up the defintion of _MT from ansi_prefix.win32.h
 * blc 990317 revised to support _beginthread/_endthread
 * vss 990827 Added return 0 to _ThreadStart and _ThreadStartEx
 * cc  000515 Fixed #include
 * cc  010531 Added _GetThreadLocalData's flag
 * JWW 020130 Changed _MWMT to _EWL_THREADSAFE for consistency's sake
 * EJS 040113 Added __try...__except block to handle SEH in threads
 */