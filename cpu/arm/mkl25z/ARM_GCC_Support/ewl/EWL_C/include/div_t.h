/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:15 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_DIV_T_H
#define _EWL_DIV_T_H

#pragma options align=native

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	typedef struct
	{
		int	quot;
		int	rem;
	} div_t;

	typedef struct
	{
		long quot;
		long rem;
	} ldiv_t;

	#if _EWL_C99 || defined(__cplusplus)
	#if _EWL_LONGLONG
		typedef struct
		{
			long long quot;
			long long rem;
		} lldiv_t;
	#endif	/* _EWL_LONGLONG */
	#endif  /* _EWL_C99 || defined(__cplusplus) */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#pragma options align=reset

#endif /* _EWL_DIV_T_H */
