/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:31 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_VA_LIST_H
#define _EWL_VA_LIST_H

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	#ifndef _EWL_VA_LIST_TYPE
		#define _EWL_VA_LIST_TYPE char *
	#endif

	#ifndef _EWL_VA_LIST_DEFINED
		typedef _EWL_VA_LIST_TYPE va_list;
		#define _EWL_VA_LIST_DEFINED
	#endif

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_VA_LIST_H */
