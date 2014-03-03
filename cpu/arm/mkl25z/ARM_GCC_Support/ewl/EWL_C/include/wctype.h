/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:33 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCTYPE_H
#define _EWL_WCTYPE_H

#include <ansi_parms.h>

#if !_EWL_USING_MW_C_HEADERS
	#if !__MACH__
	#error You must have the non-MSL C header file access path before the MSL access path
	#endif
#else

#include <cwctype>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
#if _EWL_WIDE_CHAR
	using std::wctrans_t;
	using std::wctype_t;
	using std::wint_t;
	using std::iswalnum;
	using std::iswalpha;
	#if _EWL_C99
	using std::iswblank;
	#endif /* _EWL_C99 */
	using std::iswcntrl;
	using std::iswctype;
	using std::iswdigit;
	using std::iswgraph;
	using std::iswlower;
	using std::iswprint;
	using std::iswpunct;
	using std::iswspace;
	using std::iswupper;
	using std::iswxdigit;
	using std::towctrans;
	using std::towlower;
	using std::towupper;
	using std::wctrans;
	using std::wctype;
#endif /* _EWL_WIDE_CHAR */
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* _EWL_WCTYPE_H */
