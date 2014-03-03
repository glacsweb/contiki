/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/06 13:38:53 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		abort
 *
 *		atexit
 *		exit
 *		_Exit
 *
 *		__atexit
 *		__exit
 *
 *		__setup_exit
 */

#include <ansi_parms.h>
#include <abort_exit.h>
#include <critical_regions.h>
#include <misc_io.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <pool_alloc.h>
#include <stdarg.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define max_funcs 64

int	__aborting = 0;														/*- hh 971206 -*/

void _EWL_CDECL (* __stdio_exit)  (void) = 0;
void _EWL_CDECL (* __console_exit)(void) = 0;

#if (STOP_PROGRAM_BEFORE_EXIT==1)	    								/*- mm 981227 -*/
	static void __StopProgramBeforeExit();								/*- mm 981227 -*/
#endif															    	/*- mm 981227 -*/

void _EWL_CDECL abort(void)
{
	#if (_WINSIOUX==1)													/*- mm 981227 -*/
		WinSIOUXAbort();												/*- mm 981227 -*/
	#elif (STOP_PROGRAM_BEFORE_EXIT==1)									/*- mm 981227 -*/
		__StopProgramBeforeExit();										/*- mm 981227 -*/
	#endif																/*- mm 981227 -*/
	
	raise(SIGABRT);
	
	__aborting = 1;
	
	exit(EXIT_FAILURE);
}

int _EWL_CDECL atexit(void (_EWL_CDECL *func)(void))
{	
	int result;
	
	__begin_critical_region(atexit_funcs_access);
	
	result = __register_atexit(func);
	
	__end_critical_region(atexit_funcs_access);
	
	return result;
}


#if(STOP_PROGRAM_BEFORE_EXIT==1)

	static void __StopProgramBeforeExit()
	{
		DWORD read;
		HANDLE h;
		SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
		char* buf="\n";
		
		if(GetFileType(GetStdHandle(STD_OUTPUT_HANDLE))==FILE_TYPE_CHAR)
		{
			printf("\n \n Press Enter to continue \n");
			/*  fflush(stdin);           */ 							/*- mdf 971119 -*/
			h=CreateFile("CONIN$",
					       GENERIC_READ | GENERIC_WRITE,
						   FILE_SHARE_READ | FILE_SHARE_WRITE,
						   &sa,
					       OPEN_EXISTING,NULL,NULL);
			
			if(!ReadFile(h, &buf, 1, &read,0))
			{
				read=GetLastError();
				printf("exit routine error GetLastError=%i \n",read);
			}
		}
	}
	
#endif


void _EWL_CDECL exit(int status)									/*- cc 010326 -*/
{
    if (!__aborting)
	{
		#if (_WINSIOUX==1)	
																	/*- mm 990122 -*/
			__begin_critical_region(atexit_funcs_access);
			WinSIOUXAbort();										/*- mm 990122 -*/
			__end_critical_region(atexit_funcs_access);
			
		#elif (STOP_PROGRAM_BEFORE_EXIT==1)							/*- mm 990122 -*/
		
			__begin_critical_region(atexit_funcs_access);
			__StopProgramBeforeExit();								/*- mm 981227 -*/
			__end_critical_region(atexit_funcs_access);
			
		#endif														/*- mm 981227 -*/
		
        
  	    __destroy_global_chain(); /* need to move destroy global chain above __stdio_exit 
									 as some static objects may have destructors that flush streams */
	}
	
	__exit(status);
}

#if _EWL_C99														/*- mm 030709 -*/
void _EWL_CDECL _Exit(int status)
{
	ExitProcess(status);
}
#endif /* _EWL_C99 */												/*- mm 030709 -*/
		
void _EWL_CDECL __exit(int status)
{
	#pragma unused(status)
	
	_CleanUpEWL();
	
	if (__console_exit)
	{
		(*__console_exit)();
		__console_exit = 0;
	}

	ExitProcess(status);
}

/* Change record:
 * JFH 950914 First code release.
 * JFH 951012 Added #include of <SegLoad.h> for ExitToShell() (in case
 *			  MacHeaders not included).
 * JFH 951031 Fixed exit() to longjmp(__program_exit,1) instead of call _exit on PPC
 * JFH 951215 Reworked abort/exit handling to conform to new runtime architecture.
 * JFH 951220 Renamed _atexit/_exit to __atexit/__exit for ANSI naming conformance
 * JFH 951227 Pulled guts out of __setup_exit for new runtime. Tossed __program_exit
 *								 and added __aborting for PPC and CFM68K projects.
 * JFH 960212 Tossed __setup_exit, which had become a NOP.
 * JFH 960301 Merged Be code into source. For the moment that means: __setup_exit(),
 *								 it's baaack!
 * JFH 960426 Merged Win32 changes in.
 *						CTV
 * mm  960927 Made sure dest_os set for Macintosh
 * KO  961218 Moved the begin_critical_region call after the error check. Before, if
 *            there was an error, the critical section would be entered and never left.
 * KO  961219 Added CleanUpEWL.
 * bkoz970218 line 154 moved call of __destroy_global_chain() up to 
 *			  exit() before stdio closed
 * MEA 970720 Changed __ppc_eabi_bare to __ppc_eabi.
 * mdf 970821 added printf/scanf to exit for win32 to allow control of
 *            console apps  	
 * mdf 970930 wrapped immediately above change in GetFileType to prevent app from
 *			  stopping when output is redirected to a file.
 *            Also add fflush so getc will have an empty stream
 *            when called in the exit routine		   
 * mdf 971021 fixed exiting for windows console apps in cases where
 *            the app explicitly closes stdin before exit is reached.
 *            this can be done with fclose or freopen and then close on
 *            the file associated with the stdin stream.       
 * mdf 971119 removed fflush on stdin                
 * hh  980122 #include "critical_regions.h" doesn't belong here
 * mf  980512 changes for wince  
 * mf  980810 ( & blc ) fix to x86 runtime dll crash(when > 1 dll is attached to EWL) 
 * mf  980825 Deallocate memory pools allocated by system - this fixes a
 *			  problem with memory leaking when multiple dll's are loaded and
 *			  unloaded.                               
 * mm  981015 Added version of exit for __DSP568
 * mea 981022 added calls to destructors  
 * mm  981029 Changed __DSP568 to __m56800__
 * mm  981227 Avoided closing console windows during abort. WB1-1897
 * mm  990122 Avoided closing console windows during exit.
 * hh  990227 Interface to __pool_free_all changed
 * beb 990727 added __emb_68k to the guard around call to _ExitProcess.
 * mf  000514 call to __kill_critical_regions() in __exit was deleting data used 
 *            by the dll upon unloading. This was crashing on windows.  The call to 
 *            _CleanUpEWL() will simply tell the dll that the process is exiting and
 *            allow the dll to cleanup up it's own resources when it is sure all processes
 *            are detached.
 * mf  000731 reversed the order of calling __destroy_global_chain, and (*__stdio_exit)().
 *            __destroy_global_chain must occur first since one of the destructors it calls
 *            could rely on one of the standard handles (which __stdio_exit destroys).      
 * mf  000808 moved _CleanUpEWL() into extras.win32.c.  This must always be in the dll.
 *            it can't be in the same source as abort_exit.c because of complications with
 *            the shared library.  See additional notes in extras.win32.c.  Also, atexit
 *            will be moved into atexit.c  for similar reasons. It is just preprocessed out
 *            for windows until next release.                                    
 * as  000823 added mcore guard to __exit() so we call _ExitProcess()                                                   
 * hh  010307 Merged atexit chain with global destructor chain
 * cc  010821 Added eppc to core
 * cc  010821 Added __dest_os  __dolphin_os
 * sw  011005 Reordered the PPC_EABI in front of !win32 for __atexit_funcs, since it was defined twice
 * JWW 011027 Added test for Mach-O to use the old Mac code path
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * cc  020225 Removed register_atexit prototype moved to header
 * JWW 020330 Call __EWL_console_shutdown() at Mach-O atexit time to kill the Console before Carbon
 * JWW 020423 Use new INIT_BEFORE_TERM_AFTER to ensure the console is always killed before Carbon
 * JWW 021209 Added _Exit for C99
 * mm  030709 Added C99 wrappers for _Exit
 */