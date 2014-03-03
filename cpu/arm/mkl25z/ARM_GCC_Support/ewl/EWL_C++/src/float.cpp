/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 17:13:57 $
 * $Revision: 1.1 $
 */

#include <ewl_t.h>

#if !_EWL_USING_MW_C_HEADERS

#ifdef __cplusplus
	extern "C" {
#endif

_INT32 __double_huge[]  = {0x7FF00000, 0};
_INT32 __float_nan[]    = {0x7FFFFFFF};
_INT32 __float_huge[]   = {0x7f800000};

#ifdef __cplusplus
	}
#endif

#endif  // !_EWL_USING_MW_C_HEADERS
