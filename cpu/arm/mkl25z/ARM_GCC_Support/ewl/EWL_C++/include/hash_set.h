/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 17:13:32 $
 * $Revision: 1.1 $
 */

/*
	WARNING - WARNING - WARNING

	This header is NON-STANDARD

	The classes herein are offered as extensions to the C++ standard.
	They are marked as such by the namespace Metrowerks.
*/

#ifndef _HASH_SET_H
#define _HASH_SET_H

#include <hash_set>
#include <hash_fun.h>

#ifndef _EWL_NO_CPP_NAMESPACE
	using Metrowerks::hash_set;
	using Metrowerks::hash_multiset;
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _HASH_SET_H
