/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:09 $
 * $Revision: 1.1 $
 */

#ifdef __CWCC__
	#if __CWCC__ >= 0x3000
		#pragma notonce
	#endif /* __CWCC__ >= 0x3000 */
#endif /* __CWCC__ */

#include <ansi_parms.h>

#if !_EWL_USING_MW_C_HEADERS
	#error You must have the non-MSL C header file access path before the MSL access path
#else
	#include <cassert>
#endif /* _EWL_USING_MW_C_HEADERS */
