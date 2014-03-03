/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:57:15 $
 * $Revision: 1.1 $
 */
#include <math.h>

#define __PI_O2 1.570796327f  

/*
   Author:  Matthew D. Fassiotto
   Date:    first written 4/15/99
   Purpose: non-optimal single precision version of standard inverse trig functions
   Theory:  atan is a well known algorithm taken straight from "Computer Approximations" 
            by Hart et. al. 
            1978 reprint
            pgs. 125-130.  
                 
*/ 
#if _EWL_C99 || defined(_EWL_USES_SP_MATH_LIB)

#if !( __MIPS__ && __MIPS_single_fpu__ && __option(rsqrt))

_EWL_IMP_EXP_C float atan2f _EWL_MATH_CDECL (float y, float x)
{
 return atan(y/x);
}

_EWL_IMP_EXP_C float asinf _EWL_MATH_CDECL (float x)
{
  return atan(x*(1.0f/sqrtf(1.0f- x*x)));    /* note if |x| > 1 a quiet NaN will be returned via the sqrtf function */
}
#endif /* __MIPS__ && __MIPS_ISA4__  && __MIPS_single_fpu__ */


_EWL_IMP_EXP_C float atanf _EWL_MATH_CDECL (float x)
{  
  float z,z_square;
  int index=-1,inv=0;
  const  int sign=(*(_INT32*)&x)&0x80000000;
  
  /* poly # 4964-- poly for  [0,tan(pi/8)]  */                      
  
  static const float  atan_coeff[]={.999999999f,-.3333333213f,.19999886356f,-.14281650536f,
                                    .11041179874f, -.084597554152f,.04714243524f/*.76f */};     
                     
/* data for argument reduction  */ 
  //static const float  one_over_xi[]={2.414213562f, 1.49660575f,1.00000000f,
    //                                   .668178618f,  .414213568f,.198912367f};
                                     
  static const  float  onep_one_over_xisqr_hi[]={6.82842f,3.239828f,2.0f,    /*   6.828427125f  */
                                                 1.446462f,1.17157292f,1.039566130f};
                                       
  static const  float  onep_one_over_xisqr_lo[]={.000007135f,0.00000082f,.0f,  
                                                 0.00000063f, 0.0f, 0.0f};
                                                                                 
  static const  float  atan_xi_hi[]= {0.0f,.39269f,.5890486f,  .7853981f,  /* .3926990817f  */
                                     .981747f,1.178097f,1.374446f};
  static const  float  atan_xi_lo[]= {0.0f,.000009081698724f,.000000023f,.000000063f,
                                     .000000704f, .00000025f,.00000079f};                                     
 
  static const float   one_over_xi_hi[]={2.414213f, 1.49660575f,1.00000000f,
                                         .668178618f,.414213568f,.198912367f};
                                       
  static const float   one_over_xi_lo[]={.000000562f, 0.0f,0.0f,
                                          0.0f,  0.0f,0.0f};  
                                                                                                              
  
  (*(_INT32*)&x) &=0x7fffffff;  /*  |x| */
    
  if(x >=  2.414213565f)             /* x is <= 1.0 */ //0x401A827A
  {
   z=1.0f/x;  
   inv++ ;
  } 
  else if(.4142135624f <  x)     //0x3ED413CD  //(4.14213583e-01
  {
    index++;  // index is now 0
    switch(__HI(x)&0x7f800000)
    {  
      case 0x3F000000: /* .5  <= x < 1 */       
      if((*(int*)&x) >= 0x3F08D5B9) 
        index++;  //0.5345111 == tan(5pi/32)
      if((*(int*)&x) >= 0x3F521801)
        index++;   //0.8206788
      break;
      case 0x3F800000: /* 1 <= x < 2    */
       index+=2;
      if((*(int*)&x) >= 0x3F9bf7ec)
        index++;   //1.2185035
      if((*(int*)&x) >= 0x3FEF789E)
        index++;  //1.8708684
      break;
     case 0x40000000: /* 2 <= x <  2.414213565f   */
      index+=4;
      break;       
   }
   
   z=1.0f/(one_over_xi_hi[index]+(one_over_xi_lo[index]+x));
   z=(one_over_xi_hi[index] -  onep_one_over_xisqr_hi[index]*z) +
     (one_over_xi_lo[index] -  onep_one_over_xisqr_lo[index]*z);
  
 }
 else
   z=x;
 
  z_square=z*z;
  z+= z*z_square*(atan_coeff[1]+ z_square*(atan_coeff[2]+ 
        z_square*(atan_coeff[3]+ z_square*(atan_coeff[4]+  
        z_square*(atan_coeff[5]+ z_square*atan_coeff[6])) )) 
  );  
   z+=atan_xi_lo[index+1];
   z+=atan_xi_hi[index+1];
    
  if(inv) 
  {
    z-=__PI_O2;
    if(sign)   
      return z; 
     
   return -z;
  }
  
    (*(_INT32*)&z)|=sign;
   return z;
    
}
#endif /* _EWL_C99 */

#if _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS

_EWL_IMP_EXP_C float acosf _EWL_MATH_CDECL (float x)
{
  return __PI_O2 - atan(x*(1.0f/sqrtf(1.0f- x*x)));  
  /* note if |x| > 1 a quiet NaN will be returned via the sqrtf function */
}

#endif /* _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS */
