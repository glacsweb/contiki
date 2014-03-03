#ifdef _CW_X_SLLD_
/* New version of sin with double and long long support */
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

/* sin(x)
 * Return sine function of x.
 *
 * kernel function:
 *	__kernel_sin		... sine function on [-pi/4,pi/4]
 *	__kernel_cos		... cose function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on 
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2 
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded 
 */

#include <fdlibm.h>
#include <math.h>

f64_t sin(f64_t x)
{
	f64_t y[2],z=0.0;
	int32_t n, ix;

    /* High word of x. */
	/* GET_HIGH_WORD(&ix,x); */
	D_GET_HIGH_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;
	if(ix <= 0x3fe921fb) return __kernel_sin(x,z,0);

    /* sin(Inf or NaN) is NaN */
	else if (ix>=0x7ff00000) return x-x;

    /* argument reduction needed */
	else {
	    n = __ieee754_rem_pio2(x,y);
	    switch(n&3) {
		case 0: return  __kernel_sin(y[0],y[1],1);
		case 1: return  __kernel_cos(y[0],y[1]);
		case 2: return -__kernel_sin(y[0],y[1],1);
		default:
			return -__kernel_cos(y[0],y[1]);
	    }
	}
}

#else
/* Old version of sin with no double and long long support */
/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

#include <math.h>
#include <sc/trans.h>

static f64_t
sin_cos( f64_t x, int_t cosflag)
{
   f64_t y;
   int_t neg = 0;

   if (x < 0.0) {
      x = -x;
      neg++;
   }


   /* bypass range reduction if |x| < (pi/2) and taking sin */	
   if ( cosflag || (x >= M_PI_2)) {
      y = x;

      if (cosflag) {
         neg = 0;
         y += M_PI_2;
      }

      if (y > X_TLOSS) {
         return (0.0);
      }

      y = (y * M_1_PI) + 0.5;

      /* reduce using integer arithmetic if possible */
      if (x <= (f64_t)MAXLONG) {
         int32_t n = (int32_t) y;
         y = (f64_t) n;
         if (cosflag) {
            y -= 0.5;
	     }

	 /*
         _REDUCE(long, x, y, 3.1416015625, -8.908910206761537356617e-6);
	 */
	    {
	    f64_t x1 = (f64_t) (int32_t) x, x2 = x - x1;
	    x = x1 - (y * 3.1416015625);
	    x += x2;
	    x -= y * -8.908910206761537356617e-6;
	    }
        MISRA_EXCEPTION_RULE_12_7()
        neg ^= (int_t)n % 2;
      } else {
         f64_t dn;
         x = (modf(y, &dn) - 0.5) * M_PI;
         if (modf(0.5 * dn, &dn)) {
            MISRA_EXCEPTION_RULE_12_7()
            neg ^= 1;
         }
      }

      if (x < 0.0) {
         x = -x;
         MISRA_EXCEPTION_RULE_12_7()
         neg ^= 1;
      }
   }


   /* skip for efficiency and to prevent underflow */
   if (x > X_EPS) {
      static f64_t p[] = { 0.27204790957888846175e-14,
                          -0.76429178068910467734e-12,
                           0.16058936490371589114e-9,
                          -0.25052106798274584544e-7,
                           0.27557319210152756119e-5,
                          -0.19841269841201840457e-3,
                           0.83333333333331650314e-2,
                          -0.16666666666666665052e0,
                         };
      y = x * x;
      x += x * y * _POLY7(y, p);

      /* inhibit roundoff out of range */
      if (x > 1.0) {
         x = 1.0;
      }
   }

   return ( neg ? -x : x );
}

f64_t
sin( f64_t x )
{
   return ( sin_cos(x, 0) );
}

f64_t
cos( f64_t x )
{
   return ( x ? sin_cos(x, 1) : 1.0 );
}


#endif /* _CW_X_SLLD_ */


