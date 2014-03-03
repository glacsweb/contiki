#ifdef _CW_X_SLLD_
/* New version of atan with double support */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

/* atan(x)
 * Method
 *   1. Reduce x to positive by atan(x) = -atan(-x).
 *   2. According to the integer k=4t+0.25 chopped, t=x, the argument
 *      is further reduced to one of the following intervals and the
 *      arctangent of t is evaluated by the corresponding formula:
 *
 *      [0,7/16]      atan(x) = t-t^3*(a1+t^2*(a2+...(a10+t^2*a11)...)
 *      [7/16,11/16]  atan(x) = atan(1/2) + atan( (t-0.5)/(1+t/2) )
 *      [11/16.19/16] atan(x) = atan( 1 ) + atan( (t-1)/(1+t) )
 *      [19/16,39/16] atan(x) = atan(3/2) + atan( (t-1.5)/(1+1.5t) )
 *      [39/16,INF]   atan(x) = atan(INF) + atan( -1/t )
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following 
 * constants. The decimal values may be used, provided that the 
 * compiler will convert from decimal to binary accurately enough 
 * to produce the hexadecimal values shown.
 */

#include <math.h>
#include <fdlibm.h>

int64_t LL_atanhi[] = {
  0x3FDDAC670561BB4FLL,
  0x3FE921FB54442D18LL,
  0x3FEF730BD281F69BLL,
  0x3FF921FB54442D18LL
};


int64_t LL_atanlo[] = {
  0x3C7A2B7F222F65E2LL,
  0x3C81A62633145C07LL,
  0x3C7007887AF0CBBDLL,
  0x3C91A62633145C07LL
};


int64_t LL_aT[] = {
  0x3FD555555555550DLL,
  (int64_t)0xBFC999999998EBC4LL,
  0x3FC24924920083FFLL,
  (int64_t)0xBFBC71C6FE231671LL,
  0x3FB745CDC54C206ELL,
  (int64_t)0xBFB3B0F2AF749A6DLL,
  0x3FB10D66A0D03D51LL,
  (int64_t)0xBFADDE2D52DEFD9ALL,
  0x3FA97B4B24760DEBLL,
  (int64_t)0xBFA2B4442C6A6C2FLL,
  0x3F90AD3AE322DA11LL
};

f64_t atan(f64_t x)
{
	f64_t w,s1,s2,z;
	int32_t hx,id; 
	uint32_t ix; 
	f64_t atanhi[4], atanlo[4], aT[11];
	int_t i;
	
	f64_t one, big;
	
	one = LL_to_Db((int64_t)0x3ff0000000000000LL);	/* 1.0 */
	big = LL_to_Db((int64_t)0x7e37e43c8800759cLL);	/* 1e300 */

	/* temporary fix */
	for(i=0; i<4; i++) {
		atanhi[i] = LL_to_Db(LL_atanhi[i]);
		atanlo[i] = LL_to_Db(LL_atanlo[i]);
	}
	for(i=0; i<11; i++) {
		aT[i] = LL_to_Db(LL_aT[i]);
	}
	
	/* GET_HIGH_WORD(&hx,x); */
	D_GET_HIGH_WORD(hx,x);
	
	ix = (uint32_t)hx&0x7fffffffu;
	if (ix>=0x44100000u) {	/* if |x| >= 2^66 */
		uint32_t low;
	
		/* GET_LOW_WORD(&low,x); */
		D_GET_LOW_WORD(low,x);
	
	    if ((ix>0x7ff00000u) || ((ix==0x7ff00000u)&&(low!=0u))) {
			return x+x;		/* NaN */
        }
	    if (hx>0) {
            return  atanhi[3]+atanlo[3];
        } else {
            return -atanhi[3]-atanlo[3];
        }
	}
    if (ix < 0x3fdc0000u) {	/* |x| < 0.4375 */
	    if (ix < 0x3e200000u) {	/* |x| < 2^-29 */
            if ((big+x)>one) {
                return x;	/* raise inexact */
            }
	    }
	    id = -1;
	} else {
        x = fabs(x);
        if (ix < 0x3ff30000u) {		/* |x| < 1.1875 */
            if (ix < 0x3fe60000u) {	/* 7/16 <=|x|<11/16 */
                id = 0; x = ((2.0*x)-one)/(2.0+x); 
            } else {			/* 11/16<=|x|< 19/16 */
                id = 1; x  = (x-one)/(x+one); 
            }
        } else {
            if (ix < 0x40038000u) {	/* |x| < 2.4375 */
                id = 2; x  = (x-1.5)/(one+(1.5*x));
            } else {			/* 2.4375 <= |x| < 2^66 */
                id = 3; x  = -1.0/x;
            }
        }
    }
    /* end of argument reduction */
	z = x*x;
	w = z*z;
    /* break sum from i=0 to 10 aT[i]z**(i+1) into odd and even poly */
	s1 = z*(aT[0]+(w*(aT[2]+(w*(aT[4]+(w*(aT[6]+(w*(aT[8]+(w*aT[10]))))))))));
	s2 = w*(aT[1]+(w*(aT[3]+(w*(aT[5]+(w*(aT[7]+(w*aT[9]))))))));
	if (id<0)  {
        return x - (x*(s1+s2));
    } else {
	    z = atanhi[id] - (((x*(s1+s2)) - atanlo[id]) - x);
	    return (hx<0)? -z:z;
	}
}

#else

/* Old version of atan */
/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

/*
 *	atan returns the arctangent of its double-precision argument,
 *	in the range [-pi/2, pi/2].
 *	There are no error returns.
 *
 *	atan2(y, x) returns the arctangent of y/x,
 *	in the range [-pi, pi].
 *	atan2 discovers what quadrant the angle is in and calls atan.
 *	atan2 returns EDOM error and value 0 if both arguments are zero.
 *
 *	Coefficients are #5077 from Hart & Cheney (19.56D).
 */

#include <math.h>
#include <sc/trans.h>

#define SQ2_M_1	0.41421356237309504880
#define SQ2_P_1	2.41421356237309504880
#define NEG	1u
#define INV	2u
#define MID	4u


f64_t atan(f64_t x)
{
    uint_t type = 0u;

    if ( x < 0.0 ) {
        x = -x;
        type = NEG;
    }

    if ( x > SQ2_P_1 ) {
        x = 1.0/x;
        type |= INV;
    } else { if ( x > SQ2_M_1 ) {
        x = (x - 1.0)/(x + 1.0);
        type |= MID;
    }}


    if ((x < -X_EPS) || (x > X_EPS)) {
	/* skip for efficiency and to prevent underflow */
	static f64_t p[] = { 0.161536412982230228262e2,
                           0.26842548195503973794141e3,
                           0.11530293515404850115428136e4,
                           0.178040631643319697105464587e4,
                           0.89678597403663861959987488e3,
                           },
	q[] = { 1.0,
		    0.5895697050844462222791e2,
		    0.536265374031215315104235e3,
		    0.16667838148816337184521798e4,
		    0.207933497444540981287275926e4,
		    0.89678597403663861962481162e3,
		};

        f64_t xsq = x * x;
        x *= _POLY4(xsq, p)/_POLY5(xsq, q);
    }


    if ( type & INV ) {
        x = M_PI_2 - x;
    } else { if ( type & MID ) {
	    x += M_PI_4;
    }}

    return ( (type & NEG) ? -x : x );
}

#endif

