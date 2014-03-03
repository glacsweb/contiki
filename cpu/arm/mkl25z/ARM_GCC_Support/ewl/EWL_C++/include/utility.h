/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 17:13:45 $
 * $Revision: 1.1 $
 */

#ifndef  _UTILITY_H
#define  _UTILITY_H

#include <utility>

#ifndef _EWL_NO_CPP_NAMESPACE
	using std::pair;
	using std::make_pair;
	using namespace std::rel_ops;
	namespace std { using namespace rel_ops; }
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _UTILITY_H
