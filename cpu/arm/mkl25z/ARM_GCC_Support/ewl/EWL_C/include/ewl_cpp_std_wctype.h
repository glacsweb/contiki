/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/29 14:36:30 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_CPP_STD_WCTYPE_H
#define _EWL_CPP_STD_WCTYPE_H

#ifdef __cplusplus

#if _EWL_OS_X_VERSION > 2

namespace std {

#if _EWL_WIDE_CHAR
	using ::wctrans_t;
	using ::wctype_t;
	using ::wint_t;
	using ::iswalnum;
	using ::iswalpha;
#if _EWL_C99
	using ::iswblank;
#endif /* _EWL_C99 */
	using ::iswcntrl;
	using ::iswctype;
	using ::iswdigit;
	using ::iswgraph;
	using ::iswlower;
	using ::iswprint;
	using ::iswpunct;
	using ::iswspace;
	using ::iswupper;
	using ::iswxdigit;
	using ::towctrans;
	using ::towlower;
	using ::towupper;
	using ::wctrans;
	using ::wctype;
#endif /* _EWL_WIDE_CHAR */

} /* namespace std */

#endif /* _EWL_OS_X_VERSION > 2 */

#endif /* __cplusplus */

#endif /* _EWL_CPP_STD_WCTYPE_H */
