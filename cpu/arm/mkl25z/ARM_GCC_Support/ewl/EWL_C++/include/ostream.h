/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 17:13:39 $
 * $Revision: 1.1 $
 */

#ifndef _OSTREAM_H
#define _OSTREAM_H

#include <ostream>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::basic_ostream;
	using std::ostream;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wostream;
#endif
	using std::endl;
	using std::ends;
	using std::flush;
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _OSTREAM_H
