 /* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:29 $
 * $Revision: 1.2 $
 */
 /*

   Author:  Matthew D. Fassiotto
   Date:    first written 4/15/99
   Purpose: portable single precision versions of ANSI standard powf(x,y),expf(x) functions
            and support functions.  log2f(x) is a supported part of C9X and is therefore supplied as
            a standard routine in a separate module(log2f.c).

   The single precision version of expf misses at most the last 3 bits when compared with
   the built in Intel algorithm.  The result is  the correctly rounded result produced from
   the Intel library over 67% of the time.  The accuracy can be further improved by writing the 
   constants in two_to_frac as a vector and scaling the arithmetic in the reassembly step.
   
   Notes on expf powf: 
      intends to fully comply w/C9X Annex

         
   error analysis:
     
     Most of the rounding errors are concentrated in the regions where expf^x approaches the upper limit
     of representable numbers(i.e. expf^^x ~= FLT_MAX).  In this case the variable transform from x to
     z=log2e*x is the principle cause.
     
     
   __2_to_x(float x) is basically the same routine but eliminates the expensive variable transform 
   step thereby leading to better performance and accuracy.  This is used by the pow function. i.e.
   pow(x,y)=x^^y=2^^(y*log2(x)).
   
*/
#include <math.h>

#if !__BUILD_FDLIBM
#if _EWL_C99 || defined(_EWL_USES_SP_MATH_LIB)

#ifndef __cplusplus
	#define _EWL_TURNED_ON_CPP
	#pragma cplusplus on 
#endif

 static const float scale_frac[]={0.0f,.5f};
 static const float two_to_frac[]={1.0f, 0.7071067811865f};
#ifdef __STARCORE__
 static const float log2e[]={0x3FA80000,0x3E051180,0x3980b215,0x3180c17f};
#else
 static const int log2e_bits[]={0x3FA80000,0x3E051180,0x3980b215,0x3180c17f};
 static const float log2e[]={*(float*)&log2e_bits[0],*(float*)&log2e_bits[1],*(float*)&log2e_bits[2],*(float*)&log2e_bits[3]};
#endif
//polynomial 1023 pg.169 Hart et. al.
 static const float  __two_to_x[]={.693147177596419,.240226604484390,
                                   .0555029156907419,.00962502261002979,.00131310538528088,.000183008060120550};  
inline float __2_to_x(float x)
{
  _INT32 int_part;    // truncated x                                                                         
  _INT32 index=0;     // polynomial is only valid for x > 0 -> if frac_part < 0 we multiply by 2^-.5 and add .5 to frac_part
  float z;            // intermediate value evaluated by polynomial                              
  float frac_part;        
  int_part= (signbit(x) )? (_INT32)(x-.5f) : (_INT32)(x+.5f);  //won't round to even in half way cases(doesn't matter)
  frac_part=x-int_part; 
  if (int_part > 128)
    return INFINITY;
         // this subtraction is exact
  int_part+=127;
  int_part<<=23;               // int_part is now an integral power of 2 in IEEE format  
 
  if(frac_part < 0.0f)
    index+=1;                  // arrays are setup so that negative fractions start 4 from beginning
                               // of array(right after positive scale values) 

  z=frac_part+scale_frac[index];  //puts z in [0,.5] , incurs 1 ulp rounding error in last bit
  z= z*( __two_to_x[0] + z*( __two_to_x[1] + z*( __two_to_x[2] + 
     z*( __two_to_x[3] + z*( __two_to_x[4] + z*__two_to_x[5]) ))));  // calculate 2^^x -1 -> z in [0.0 ,.414213562]
     
  // reassemble 
#if __MIPS__
   return __int2float__(int_part)*(.75f*two_to_frac[index] + two_to_frac[index]*(z+.25f));  
#else
  z=(*(float*)&int_part)*(.75f*two_to_frac[index] + two_to_frac[index]*(z+.25f));   
  return z ;                             
#endif  
                          
}

_EWL_BEGIN_EXTERN_C

float expf(float x)
 {                                                     
  _INT32 int_part;                                                                             
  _INT32 index=0;
  float z;
  const int int_x=x;                                          // truncated x
  const float frac_x=x-int_x;
  float frac_part;          
 
  z=x*log2e[0] + (x*log2e[1] + (x*log2e[2] + x*log2e[3]));    // extra bits for log2(e) forces correct rounding

  int_part= (signbit(z) )? (_INT32)(z-.5f) : (_INT32)(z+.5f); // won't round to even in half way cases(doesn't matter)
 
 //the order and number of terms below has an affect on accuracy.  This should not be changed.
 
  frac_part=(int_x*log2e[0] - (float)int_part) + (int_x*log2e[1] + (frac_x*log2e[0] + ( int_x*log2e[2] +
            (frac_x*log2e[1] +(frac_x*log2e[2] + x*log2e[3])))));

  int_part+=127;
  int_part<<=23;               // int_part is now an integral power of 2 in IEEE format
  if(int_part < 0)
    return INFINITY;           // too big
  if(frac_part < 0.0f)
    index+=1;                  // arrays are setup so that negative fractions start 4 from beginning
                               // of array(right after positive scale values) 

  z=frac_part+scale_frac[index];  //puts z in [0,.5]
  z= z*( __two_to_x[0] + z*( __two_to_x[1] + z*( __two_to_x[2] + 
     z*( __two_to_x[3] + z*( __two_to_x[4] + z*__two_to_x[5]) ))));  
     
  // reassemble 
#if __MIPS__
   return __int2float__(int_part)*(two_to_frac[index] + two_to_frac[index]*z);  
#else
  return (*(float*)&int_part)*(.75f*two_to_frac[index] + two_to_frac[index]*(z+ .25f));                                
#endif  
                          
}


float powf(float x, float y)
{	
    int tmp;
	if (x > 0.0f)
	{
#if __DSP
    return exp(y * logf(x));
#else
	return __2_to_x(y * log2f(x));
#endif		
    }    
    else if (x < 0.0f) // bug: bad for -0
    {
     tmp=y;
     if((y-(float)tmp)) 
       return NAN;  /* true for non-integral exponents */
     else 
     { 
#if __DSP       
     if(tmp%2) 
        return -exp(y * logf(-x));       /* odd integral exponent */
    
      return  exp(y * logf(-x));         /* even integral exponent */
#else
    if(tmp%2) 
        return -__2_to_x(y * log2f(-x));  /* odd integral exponent */
    
      return    __2_to_x(y * log2f(-x));  /* even integral exponent */
#endif
      }
     }  
     
    /* x=0.0f or perhaps a nan */
    if(isnan(x)) 
      return x;

    /* x is 0.0 if we reach the switch */

     switch(fpclassify(y))
     {
      case FP_ZERO:
       return 1.0f;
       break;
      case FP_NAN:
      case FP_INFINITE:
       return NAN;
       break;
      case FP_NORMAL :
      case FP_SUBNORMAL :
       if((*(_INT32*)&x)&0x80000000) 
        return INFINITY;
       else 
        return x ;
       break;
      }
    return 0.0f;
}
_EWL_END_EXTERN_C

#ifdef _EWL_TURNED_ON_CPP
	#pragma cplusplus reset 
#endif

#endif
#endif /*!__BUILD_FDLIBM */

