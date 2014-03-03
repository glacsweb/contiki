/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:33 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WEOF_H
#define _EWL_WEOF_H

#if !_EWL_USING_MW_C_HEADERS
	#error You must have the non-MSL C header file access path before the MSL access path
#else

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

#ifndef _EWL_WEOF
	#define _EWL_WEOF ((__std(wint_t))(-1))
#endif

#ifndef WEOF
	#define WEOF _EWL_WEOF
#endif /* WEOF */

#endif /* _EWL_WIDE_CHAR */

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* _EWL_WEOF_H */
