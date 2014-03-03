/* @(#)s_matherr.c 1.2 95/01/04 */
/* $Id: s_matherr.c,v 1.4 2010/05/12 07:07:16 anczane1 Exp $ */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <fdlibm.h>

#if _EWL_FLOATING_POINT

#ifdef __STDC__
	MISRA_EXCEPTION_RULE_16_7()
	int32_t _EWL_MATH_CDECL matherr(struct exception *x)
#else
	int32_t matherr(x)
	struct exception *x;
#endif
{
	int32_t n=0;
	MISRA_EXCEPTION_RULE_13_3()
	if(x->arg1!=x->arg1) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	return n;
}

#endif /* _EWL_FLOATING_POINT  */
