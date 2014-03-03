/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/29 13:41:59 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		signal
 */

#include "critical_regions.h"
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <x86/startup.win32.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

__signal_func_ptr ctrlc = 0;   
__signal_func_ptr ctrlbreak = 0;    

/* signal handler table declared in signal.c */
extern __signal_func_ptr	signal_funcs[__ewl_signal_max];

int ConsoleCtrlHandler = 0;

int __stdcall __handler(unsigned long);

int __stdcall __handler(unsigned long CtrlType)
{
        __signal_func_ptr ctrl_action;
        __signal_func_ptr *pctrl_action;
        
        int __signal;

        if ( CtrlType == CTRL_C_EVENT ) {
                ctrl_action = *(pctrl_action = &ctrlc);
                __signal = SIGINT;
        }
        else {
                ctrl_action = *(pctrl_action = &ctrlbreak);
                __signal = SIGBREAK;
        }

        if ( ctrl_action == SIG_DFL ) {
                /*
                 * return FALSE, indicating the event has NOT been handled
                 */
                return 0;
        }

        if ( ctrl_action != SIG_IGN ) {
                /*
                 * Reset the action to be SIG_DFL and call the user's handler.
                 */
                *pctrl_action = SIG_DFL;
                (*ctrl_action)(__signal);
        }
        
        return 1;
}
void __check_console_handler(void);
void __check_console_handler(void)
{

	if (!ConsoleCtrlHandler )
	{ 		
		if (SetConsoleCtrlHandler(__handler, TRUE))
			ConsoleCtrlHandler = 1;
		else {
			__set_errno(GetLastError());
		}
	}
}      

__signal_func_ptr _EWL_CDECL signal(int __signaltype, __signal_func_ptr signal_func)
{
        __signal_func_ptr old_signal_func;
        
		if (__signaltype < 1 || __signaltype > __ewl_signal_max)
		{
			errno = ESIGPARM;
			return(SIG_ERR);
		}

		/* get old handler */
		old_signal_func = signal_funcs[__signaltype-1];

		/* special handling for these; others ignored */
		if(__signaltype == SIGINT)
		{
			__check_console_handler();
		    ctrlc = signal_func;
		}
		else if(__signaltype == SIGBREAK)  
		{
			__check_console_handler();
		    ctrlbreak = signal_func;
		}
		
		/* store new handler */
		signal_funcs[__signaltype-1] = signal_func;
		
 		return(old_signal_func);
}



/* Change record:
 * cc  010910 made win only signal
 * ejs 011128 fixed bug with signal table sharing and SIGINT/SIGBREAK
 * cc  011203 Added _EWL_CDECL for new name mangling 
 */