/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:23 $
 * $Revision: 1.1 $
 */
 
#ifndef _EWL_ISO646_H
#define _EWL_ISO646_H

#include <ansi_parms.h>

#if _EWL_C99

#if !_EWL_USING_MW_C_HEADERS
	#if !__MACH__
	#error You must have the non-MSL C header file access path before the MSL access path
	#endif
#else

#include <ciso646>

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* _EWL_C99 */

#endif /* _EWL_ISO646_H */
