/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

/*
 *	tanh returns the hyperbolic tangent of its double-precision argument.
 *	It calls exp for absolute values of the argument > ~0.55.
 *	There are no error returns.
 *	Algorithm and coefficients from Cody and Waite (1980).
 */

#include <math.h>
#include <sc/trans.h>

#ifdef _CW_X_SLLD_
#include <prototype.h>
#endif

#define X_BIG	(0.5 * (LN_MAXDOUBLE + M_LN2))

#ifdef _CW_X_SLLD_
#define LL_LN_3_2		0x3fe193ea7aad030bLL
#else
#define LN_3_2	0.54930614433405484570
#endif 

f64_t tanh(f64_t x)
{
	register int_t neg = 0;

	if (x < 0.0) {
		x = -x;
		neg++;
	}
	if (x > X_BIG) {
		x = 1.0;
    }
	
#ifdef _CW_X_SLLD_
	else if (x > LL_to_Db(LL_LN_3_2)) {
#else
	else if (x > LN_3_2) {
#endif
		x = 0.5 - (1.0/(exp(x + x) + 1.0)); /* two steps recommended */
		x += x;
	} else { if (x > X_EPS) { /* skip for efficiency and to prevent underflow */
		f64_t y;
#ifdef _CW_X_SLLD_
		static f64_t p[3], q[4];
		p[0] = LL_to_Db(0xbfeedc28cefba77fLL);
		p[1] = LL_to_Db(0xc058ce75a1ba5cccLL);
		p[2] = LL_to_Db(0xc09935a5c9be1e18LL);
		
		q[0] = LL_to_Db(0x3ff0000000000000LL);
		q[1] = LL_to_Db(0x405c2fa9e1ebf7faLL);
		q[2] = LL_to_Db(0x40a1738b4d01f0f3LL);
		q[3] = LL_to_Db(0x40b2e83c574e9693LL);
#else
		static f64_t p[] = {
			-0.96437492777225469787e0,
			-0.99225929672236083313e2,
			-0.16134119023996228053e4,
		}, q[] = {
			 1.0,
			 0.11274474380534949335e3,
			 0.22337720718962312926e4,
			 0.48402357071988688686e4,
		};
#endif

		y = x * x;
		x += ((x * y) * _POLY2(y, p))/_POLY3(y, q);
	}}
	return (neg ? -x : x);
}
