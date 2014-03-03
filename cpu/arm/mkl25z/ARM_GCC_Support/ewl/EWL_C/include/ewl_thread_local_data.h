/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/03/17 14:43:50 $
 * $Revision: 1.4 $
 */

#ifndef _EWL_THREAD_LOCAL_DATA_H
#define _EWL_THREAD_LOCAL_DATA_H

#include <ansi_parms.h>

#if _EWL_THREADSAFE && _EWL_LOCALDATA_AVAILABLE
	#if _EWL_PTHREADS
		#include <thread_local_data_pthreads.h>
	#endif
    #if (__dest_os == __starcore)
		#include <sc/thread_local_data_StarCore.h>
    #elif (__dest_os == __win32_os)
        #include <x86/ThreadLocalData.h>
	#endif
#endif /* _EWL_THREADSAFE */

#endif /* _EWL_THREAD_LOCAL_DATA_H */
