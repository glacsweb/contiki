/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:26 $
 * $Revision: 1.3 $
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE

#include <errno.h>

#include <ewl_misra_types.h>

int_t errno;

#endif /* _EWL_THREADSAFE */

