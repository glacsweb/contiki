/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:15 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_ERRNO_H
#define _EWL_ERRNO_H

#include <ansi_parms.h>

#if !_EWL_USING_MW_C_HEADERS
	#error You must have the non-MSL C header file access path before the MSL access path
#else

#include <cerrno>

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* _EWL_ERRNO_H */
