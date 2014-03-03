#ifdef _CW_X_SLLD_
/* New version of fabs with double support */
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

/*
 * fabs(x) returns the absolute value of x.
 */

#include <math.h>
#include <fdlibm.h>

f64_t fabs(f64_t x)
{
	int32_t high;
	D_GET_HIGH_WORD(high,x);
	D_SET_HIGH_WORD(x,(uint32_t)high&0x7fffffffu);
    return x;
}

#else
/* Old version */
/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

/*
 *	fabs returns the absolute value of its double-precision argument.
 */

#include <math.h>

f64_t
fabs(f64_t x)
{
	return ((x < 0.0) ? -x : x);
}

#endif
