/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/14 12:39:16 $
 * $Revision: 1.6 $
 */

#include "x86/ThreadLocalData.h"
#include <stdlib.h>    /*- hh 980122 -*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "critical_regions.h"
#include "lconv.h"									/*- mm 010503 -*/
#include <locale.h>									/*- mm 011130 -*/

static unsigned long _gThreadDataIndex = -1;

// this static variable points the the head of the linked list of thread
// local data structures.  When the process ends, all of these are freed.
static _ThreadLocalData *firstTLD;

// This function must be called before _InitializeThreadData.
// Returns false on error, true on success.
int _EWL_CDECL _InitializeThreadDataIndex(void)
{
	if (_gThreadDataIndex != -1) {
		// _gThreadDataIndex is already initialized
		return 1;
	}
	
	_gThreadDataIndex = TlsAlloc();

	if (_gThreadDataIndex == -1) {
		// The allocation failed.
		return 0;
	}
/*	
	if (atexit(_DisposeThreadDataIndex)) {
		return 0;
	} else {
		return 1;
	}
*/
return 1;	
}

void _EWL_CDECL _DisposeThreadDataIndex(void)
{
	if (_gThreadDataIndex == -1) {
		// _gThreadDataIndex has an invalid value.
		// Maybe it was never allocated?
		return;
	}
	
	TlsFree(_gThreadDataIndex);
	_gThreadDataIndex = -1;
}

// This function sets up a structure of thread local data needed by various EWL functions.
// When the thread ends it should call _DisposeThreadData.
// Returns false on error, true on success.
int _EWL_CDECL _InitializeThreadData(void *inThreadHandle)
{
	HANDLE threadHandle = (HANDLE)inThreadHandle;
	_ThreadLocalData * theThreadLocalData;
	
	theThreadLocalData = TlsGetValue(_gThreadDataIndex);
	
	if (theThreadLocalData == 0)
	{
		if (GetLastError() == NO_ERROR)
		{
			theThreadLocalData =
				(_ThreadLocalData *)malloc(sizeof(_ThreadLocalData));
		
			if (!theThreadLocalData)
			{
				return 0;
			}

			// add TLD to head of linked list
			__begin_critical_region(thread_access);

			theThreadLocalData->next = firstTLD;
			firstTLD = theThreadLocalData;
			
			__end_critical_region(thread_access);
			
			// All initialization of items in the _ThreadLocalData structure
			// goes here...
			
			theThreadLocalData->errno = 0;	
			theThreadLocalData->random_next = 1;
			theThreadLocalData->strtok_dn = (unsigned char *) "";
			theThreadLocalData->strtok_ds = (unsigned char *) "";
			theThreadLocalData->thread_handle = threadHandle;
			theThreadLocalData->locale_name = "C";
			theThreadLocalData->localtime_tm.tm_sec   =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_min   =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_hour  =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_mday  =  1;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_mon   =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_year  =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_wday  =  1;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_yday  =  0;				/*- mm 010516 -*/
			theThreadLocalData->localtime_tm.tm_isdst = -1;				/*- mm 010516 -*/
			
			theThreadLocalData->gmtime_tm.tm_sec   =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_min   =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_hour  =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_mday  =  1;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_mon   =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_year  =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_wday  =  1;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_yday  =  0;				/*- mm 010516 -*/
			theThreadLocalData->gmtime_tm.tm_isdst = -1;					/*- mm 010516 -*/

#if !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
#if _EWL_C_LOCALE_ONLY

			theThreadLocalData->_current_locale.time_cmpt_ptr  = 			/*- mm 011130 -*/
										_current_locale.time_cmpt_ptr;		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.coll_cmpt_ptr  = 			/*- mm 011130 -*/
										_current_locale.coll_cmpt_ptr;		/*- mm 011130 -*/
#if _EWL_WIDE_CHAR
			theThreadLocalData->_current_locale.ctype_cmpt_ptr = 			/*- mm 011130 -*/
										_current_locale.ctype_cmpt_ptr;		/*- mm 011130 -*/
#endif
#else
			theThreadLocalData->_current_locale.next_locale    = NULL;		/*- mm 011130 -*/
			strcpy(theThreadLocalData->_current_locale.locale_name, 		/*- mm 011130 -*/
										_current_locale.locale_name);		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.coll_cmpt_ptr  = 			/*- mm 011130 -*/
										_current_locale.coll_cmpt_ptr;		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.ctype_cmpt_ptr = 			/*- mm 011130 -*/
										_current_locale.ctype_cmpt_ptr;		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.mon_cmpt_ptr   = 			/*- mm 011130 -*/
										_current_locale.mon_cmpt_ptr;		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.num_cmpt_ptr   = 			/*- mm 011130 -*/
										_current_locale.num_cmpt_ptr;		/*- mm 011130 -*/
			theThreadLocalData->_current_locale.time_cmpt_ptr  = 			/*- mm 011130 -*/
										_current_locale.time_cmpt_ptr;		/*- mm 011130 -*/
			memcpy((void*)&theThreadLocalData->_current_locale.next_locale,	/*- mm 011130 -*/ 
				(void*)&_current_locale.next_locale, sizeof(struct __locale));/*- mm 011130 -*/	/*- mm 011205 -*/
#endif
#endif
			theThreadLocalData->__lconv = __lconv;					/*- mm 010503 -*/
			
			theThreadLocalData->user_se_translator = 0L;				/*- ejs 010625 -*/
			return TlsSetValue(_gThreadDataIndex, theThreadLocalData);
		}
		return 0;	// problem retreiving TLS handle
	}
	return 1;	// data is already setup, just return success	
}


int _EWL_CDECL _InitializeMainThreadData(void)
{
    // First, get the thread data index by using tlsAlloc
    
    if (_InitializeThreadDataIndex())
    {
        // now, get the data for the main thread
		// (thread handle is NULL since you never call endthread() or
		// endthreadex() on the main thread of the program)
        
        if (_InitializeThreadData(NULL))
        {
            return 1;
        }
    }
    
    return 0;
}

static void release_thread_data(_ThreadLocalData *tld)
{
	_ThreadLocalData **current;

	__begin_critical_region(thread_access);

	current = &firstTLD;
	
	while (*current)
	{
		if (*current == tld)
			break;
		current = &(*current)->next;
	}

	if (*current)
	{
		_ThreadLocalData *next = (*current)->next;
		free (*current);
		*current = next;
	}

	__end_critical_region(thread_access);
}

void _EWL_CDECL _DisposeAllThreadData()
{
	_ThreadLocalData *current;

	__begin_critical_region(thread_access);

	current = firstTLD;
	while (current)
	{
		_ThreadLocalData *next = current->next;
		free(current);
		current = next;
	}
	firstTLD = NULL;

	__end_critical_region(thread_access);
}

void _EWL_CDECL _DisposeThreadData(void)
{
	_ThreadLocalData * theThreadLocalData = _GetThreadLocalData(0);
	
	if (!theThreadLocalData) {
		return;
	}

	TlsSetValue(_gThreadDataIndex, 0);

	release_thread_data(theThreadLocalData); /*- cc 010507 -*/

}

_ThreadLocalData * _EWL_CDECL _GetThreadLocalData(int inInitializeDataIfMissing)	/*- cc 010531 -*/
{
    _ThreadLocalData *tld;
    
	tld = (_ThreadLocalData *)TlsGetValue(_gThreadDataIndex);
	
	if(inInitializeDataIfMissing)	/*- cc 010531 -*/
	{	
		if (tld == 0)
		{
			_InitializeThreadData(NULL);
			tld = (_ThreadLocalData *)TlsGetValue(_gThreadDataIndex);
		}
		
		if (tld == 0)
		{
			/*- ejs 020418 -*/
			/*
				This may be caused by loading too many statically-linked
				DLLs into the same process, or linking together mismatched
				static EWL libraries.  Make sure that if "link default libraries"
				is enabled, you don't have stray libraries in your project.
			*/
		    MessageBox(NULL, 
		    	TEXT("Could not allocate thread local data."),
			    TEXT("MW Win32 Runtime"), MB_OK|MB_ICONEXCLAMATION);
		    exit(127);
		}
	}	/*- cc 010531 -*/
	return tld;
}

/* Change record:
 * hh  980122 added <cstdlib>
 * hh  980122 replaced <wtypes.h> with <WINDEF.H> 
 * rjk 980314 fixed _GetThreadLocalData to allocate it if it was not already
 * blc 990317 modified Windows.h include strategy, added threadHandle support
 * mf  000710 Glen's changes to making global linked list threadsafe.
 * mm  010503 Initialization for new field in thread local data
 * cc  010507 commented back in release_thread_data call
 * mm  010516 Initialization for more fields in thread local data 
 * cc  010531 Added _GetThreadLocalData's flag 
 * ejs 010625 Added user_se_handler
 * mm  011130 Added locale to thread local data
 * cc  011203 Added _EWL_CDECL for new name mangling
 * mm  011205 Changed _LOCALE to __locale 
 * ejs 020418 Changed thread-local data failure to give better message and to exit with failure code
 * JWW 020604 Keep a complete copy of __lconv in the thread local data
 */