/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

/*
 *	tan returns the tangent of its double-precision argument.
 *	Returns ERANGE error and value 0 if argument too large.
 *	Algorithm and coefficients from Cody and Waite (1980).
 */

#include <math.h>
#include <sc/trans.h>

#ifdef _CW_X_SLLD_
#include <prototype.h>
#endif


#define _REDUCE(TYPE, X, XN, C1, C2)    { \
	f64_t x1 = (f64_t)(TYPE)X, x2 = X - x1; \
	X = x1 - ((XN) * (C1)); X += x2; X -= (XN) * (C2); }

f64_t tan( f64_t x)
{
	f64_t y;
	
#ifdef _CW_X_SLLD_
	int64_t neg = 0, invert;
#else
	int_t neg = 0, invert;
#endif	

	if (x < 0.0) {
		x = -x;
		neg++;
	}

	if (x > (X_TLOSS/2.0)) {
		return ( 0.0 );
	}

	y = (x * M_2_PI) + 0.5;

	/* reduce using integer arithmetic if possible */
#ifdef _CW_X_SLLD_
	if (x <= (f64_t)MAXLONGLONG) {
		int64_t n = (int64_t)y;
		y = (f64_t)n;
		_REDUCE(int64_t, x, y, LL_to_Db(0x3ff9220000000000LL), 
					LL_to_Db(0xbed2aeef4b9ee59eLL))
		invert = (int64_t)n % 2;
	}
#else
	if ( x <= (f64_t)MAXLONG ) {

		int32_t n = (int32_t)y;

		y = (f64_t)n;
		_REDUCE(int32_t, x, y, 1.57080078125, -4.454455103380768678308e-6)
		invert = (int_t)n % 2;
	}
#endif
	else {
		f64_t dn;
		x = (modf(y, &dn) - 0.5) * M_PI_2;
		invert = modf(0.5 * dn, &dn) ? 1 : 0;
	}

	if ((x > -X_EPS) && (x < X_EPS)) {
			/*
			 * This case represents a range reduction
			 * from (2n + 1) * PI/2 which happens to return 0.
			 * This is mathematically insignficant, and extremely
			 * unlikely in any case, so it is best simply
			 * to substitute an arbitrarily small value
			 * as if the reduction were one bit away from 0,
			 * and not to deal with this as a range error.
			 */
		if (!x && invert) {
			x = 1.0/X_TLOSS;
		}

		y = 1.0; /* skip for efficiency and to prevent underflow */
	} else {
#ifdef _CW_X_SLLD_
		static f64_t p[3],q[5];
		
		p[0] = LL_to_Db(0xbef2bab72ea2c724LL);
		p[1] = LL_to_Db(0x3f6c0e82a63baadfLL);
		p[2] = LL_to_Db(0xbfc112b5e54d0900LL);
		
		q[0] = LL_to_Db(0x3ea0b774f07678e9LL);
		q[1] = LL_to_Db(0xbf346f6499094841LL);
		q[2] = LL_to_Db(0x3f9a479ea17e2159LL);
		q[3] = LL_to_Db(0xbfdddeb047fbd9d5LL);
		q[4] = LL_to_Db(0x3ff0000000000000LL);
#else
		static f64_t p[] = {
			-0.17861707342254426711e-4,
			 0.34248878235890589960e-2,
			-0.13338350006421960681e+0,
		}, q[] = {
			 0.49819433993786512270e-6,
			-0.31181531907010027307e-3,
			 0.25663832289440112864e-1,
			-0.46671683339755294240e+0,
			 1.0,
		};
#endif

		y = x * x;
		x += x * y * _POLY2(y, p);
		y = _POLY4(y, q);
	}


	if (neg) {
		x = -x;
	}

    return( invert ? -(y/x) : (x/y) );
}
