/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:57:16 $
 * $Revision: 1.1 $
 */

#include <math.h>

#if !__BUILD_FDLIBM
#if _EWL_C99 || defined(_EWL_USES_SP_MATH_LIB)

#  define  __ln2        0.69314718f
#  define  __sqrt2_m1  .41421356237f							

//poly 26662--misses double result by > 1 ulp but < 2 ulp .0034%
static  const float lnpoly[] = 
    {
	 1.99999999f,
	 0.66666948f,
	 0.39965795f,
	 0.301003281f
    };
        
float logf(float x)
 {
    float   __lnsqrt2 = 0.34657359f;  
    _INT32 exp,tmp;
    float y,zsq;
    
	if (((*(_UINT32*)&x)&0x80000000))
	{
	  if ((*(_INT32*)&x)&0x7fffffff)
		return NAN;  //not zero
	 else 
		return -INFINITY;  //minus zero
	}
	else
	{
	 
	 exp = ((*(_UINT32*)&x) >> 23) - 126; 
     if((*(_INT32*)&x)&=0x007fffff)
      	(*(_INT32*)&x)|= 0x3f000000;
	 else
	   return --exp* __ln2;

	 if( (*(_INT32*)&x) < 0x3F3504F3) //constant for 1/sqrt2  , this check greatly improves accuracy w/only   	             
    	x=x+ __sqrt2_m1*x;            //minor performance penalty.  without this the 2nd ulp is missed 2000 times more frequently
     else
       __lnsqrt2=0.0f;    

     x = 1.0f -(2.0f/(x + 1.0f));
	 zsq = x*x;
	 y=(((lnpoly[3]*zsq + lnpoly[2]) * zsq + 
	      lnpoly[1])*zsq + lnpoly[0])*x;
	 tmp= y;         //truncated toward zero
     y-=(float)tmp;
	 return (exp*__ln2 + tmp) + (y - __lnsqrt2);
	}
 }
#endif
#endif /*!__BUILD_FDLIBM */
