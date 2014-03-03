/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:57:14 $
 * $Revision: 1.1 $
 */
 
 
 /*
   This single precision version of expf misses at most the last 3 bits when compared with
   the built in Intel algorithm.  The result is exactly the correctly rounded result produced from
   the Intel library over 67% of the time.  The accuracy can be further improved by writing the 
   constants in two_to_frac as a vector and scaling the arithmetic in the reassembly step.
*/
#if 0 /* this code is obsoleted by exponentialsf.c */

#pragma cplusplus on   
 #include <math.h>
 static const float scale_frac[]={0.0f,.5f};
 static const float two_to_frac[]={1.0f,.7071067812f};
 static const int log2e_bits[]={0x3FA80000,0x3E051180,0x3980b215,0x3180c17f};
 static const float log2e[]={*(float*)&log2e_bits[0],*(float*)&log2e_bits[1],*(float*)&log2e_bits[2],*(float*)&log2e_bits[3]};
//polynomial 1023 pg.169 Hart et. al.
 static const float  __two_to_x[]={.69314717759,.240226604484390,
                                   .05550291569,.009625022610,.001313105385,.0001830080601};  

extern "C" float expf(float x)
 {                                                     
  _INT32 int_part;                                                                             
  _INT32 index=0;
  float z;
  const int int_x=x;                                        // truncated x
  const float frac_x=x-int_x;
  float frac_part;          
 
  z=x*log2e[0] + (x*log2e[1] + (x*log2e[2] + x*log2e[3]));   // extra bits for log2(e) forces correct rounding

  int_part= (signbit(z) )? (_INT32)(z-.5f) : (_INT32)(z+.5f);  //won't round to even in half way cases(doesn't matter)
 //the order and number of terms below has an affect on accuracy.
  frac_part=(int_x*log2e[0] - (float)int_part) + (int_x*log2e[1] + (frac_x*log2e[0] + ( int_x*log2e[2] +
            (frac_x*log2e[1] +(frac_x*log2e[2] + x*log2e[3])))));

  int_part+=127;
  int_part<<=23;               // int_part is now an integral power of 2 in IEEE format
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
#pragma cplusplus reset
#endif