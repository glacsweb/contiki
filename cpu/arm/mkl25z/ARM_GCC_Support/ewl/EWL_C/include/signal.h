/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:26 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_SIGNAL_H
#define _EWL_SIGNAL_H

#include <ansi_parms.h>

#if !_EWL_USING_MW_C_HEADERS
	#error You must have the non-MSL C header file access path before the MSL access path
#else

#include <csignal>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::sig_atomic_t;
	using std::raise;
	using std::signal;
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* _EWL_SIGNAL_H */
