/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/11/05 19:15:41 $
 * $Revision: 1.2 $
 */
 
/*
 Author:  Matthew D. Fassiotto
 Date:    first written 4/15/99
 Purpose: non-optimal single precision version of standard sqrt functions
 Assumptions: --IEEE 754 single precision float format
     *fp difference should never produce -0
     *casting a float to an int always truncates regardless of fp rounding mode.
     *the type _INT32 is 32 bits(i.e. sizeof(_INT32)=sizeof(float)
     
Note: need to eliminate two divisions in Newton iteration     
*/
#include <math.h>
#include <errno.h>
#if !(__MIPS__ && __option(sqrt))
#if 0
static int sqrt_guess[]=  {0x3F3504F3,0x3F37D375,0x3F3A9728,0x3F3D5087,
                            0x3F400000,0x3F42A5FE,0x3F4542E1,0x3F47D706,
                            0x3F4A62C2,0x3F4CE665,0x3F4F623A,0x3F51D689,
                            0x3F544395,0x3F56A99B,0x3F5908D9,0x3F5B6186,
                            0x3F5DB3D7,0x3F600000,0x3F624630,0x3F648695,
                            0x3F66C15A,0x3F68F6A9,0x3F6B26A9,0x3F6D517F,
                            0x3F6F7751,0x3F71983E,0x3F73B46A,0x3F75CBF2,
                            0x3F77DEF6,0x3F79ED91,0x3F7BF7DF,0x3F7DFDFC};
                           
#elif 0                          
static int sqrt_guess[]=  {0x3F3504F3,0x3F366D96,0x3F37D375,0x3F3936A1,0x3F3A9728,
                0x3F3BF51B,0x3F3D5087,0x3F3EA979,0x3F400000,0x3F415428,0x3F42A5FE,
				0x3F43F58D,0x3F4542E1,0x3F468E06,0x3F47D706,0x3F491DEC,0x3F4A62C2,
				0x3F4BA592,0x3F4CE665,0x3F4E2545,0x3F4F623A,0x3F509D4E,0x3F51D689,
				0x3F530DF3,0x3F544395,0x3F557775,0x3F56A99B,0x3F57DA10,0x3F5908D9,
				0x3F5A35FE,0x3F5B6186,0x3F5C8B77,0x3F5DB3D7,0x3F5EDAAE,0x3F600000,
				0x3F6123D4,0x3F624630,0x3F636719,0x3F648695,0x3F65A4A9,0x3F66C15A,
				0x3F67DCAE,0x3F68F6A9,0x3F6A0F50,0x3F6B26A9,0x3F6C3CB7,0x3F6D517F,
				0x3F6E6507,0x3F6F7751,0x3F708862,0x3F71983E,0x3F72A6EA,0x3F73B46A,
				0x3F74C0C0,0x3F75CBF2,0x3F76D603,0x3F77DEF6,0x3F78E6CE,0x3F79ED91,
				0x3F7AF340,0x3F7BF7DF,0x3F7CFB72,0x3F7DFDFC,0x3F7EFF7F};
                            
                            
#else

static int sqrt_guess[]={0x3F35B99E,0x3F366D96,0x3F3720DD,0x3F37D375,0x3F388560,0x3F3936A1,0x3F39E738,
0x3F3A9728,0x3F3B4673,0x3F3BF51B,0x3F3CA321,0x3F3D5087,0x3F3DFD4E,0x3F3EA979,0x3F3F5509,0x3F400000,
0x3F40AA5F,0x3F415428,0x3F41FD5C,0x3F42A5FE,0x3F434E0D,0x3F43F58D,0x3F449C7E,0x3F4542E1,0x3F45E8B9,
0x3F468E06,0x3F4732CA,0x3F47D706,0x3F487ABC,0x3F491DEC,0x3F49C098,0x3F4A62C2,0x3F4B046A,0x3F4BA592,
0x3F4C463A,0x3F4CE665,0x3F4D8613,0x3F4E2545,0x3F4EC3FC,0x3F4F623A,0x3F500000,0x3F509D4E,0x3F513A26,
0x3F51D689,0x3F527278,0x3F530DF3,0x3F53A8FD,0x3F544395,0x3F54DDBC,0x3F557775,0x3F5610BF,0x3F56A99B,
0x3F57420B,0x3F57DA10,0x3F5871A9,0x3F5908D9,0x3F599FA0,0x3F5A35FE,0x3F5ACBF5,0x3F5B6186,0x3F5BF6B1,
0x3F5C8B77,0x3F5D1FD9,0x3F5DB3D7,0x3F5E4773,0x3F5EDAAE,0x3F5F6D87,0x3F600000,0x3F609219,0x3F6123D4,
0x3F61B531,0x3F624630,0x3F62D6D3,0x3F636719,0x3F63F704,0x3F648695,0x3F6515CC,0x3F65A4A9,0x3F66332E,
0x3F66C15A,0x3F674F2F,0x3F67DCAE,0x3F6869D6,0x3F68F6A9,0x3F698327,0x3F6A0F50,0x3F6A9B26,0x3F6B26A9,
0x3F6BB1D9,0x3F6C3CB7,0x3F6CC744,0x3F6D517F,0x3F6DDB6B,0x3F6E6507,0x3F6EEE53,0x3F6F7751,0x3F700000,
0x3F708862,0x3F711076,0x3F71983E,0x3F721FBA,0x3F72A6EA,0x3F732DCF,0x3F73B46A,0x3F743ABA,0x3F74C0C0,
0x3F75467E,0x3F75CBF2,0x3F76511E,0x3F76D603,0x3F775AA0,0x3F77DEF6,0x3F786305,0x3F78E6CE,0x3F796A52,
0x3F79ED91,0x3F7A708B,0x3F7AF340,0x3F7B75B1,0x3F7BF7DF,0x3F7C79CA,0x3F7CFB72,0x3F7D7CD8,0x3F7DFDFC,
0x3F7E7EDE,0x3F7EFF7F,0x3F7F7FE0,0x3F800000,0x3F803FF0};

static int sqrt_guess2[]={0x3F00FF02,0x3F017DC7,0x3F01FC10,0x3F0279DF,0x3F02F734,0x3F037413,0x3F03F07B,
0x3F046C6F,0x3F04E7EE,0x3F0562FC,0x3F05DD98,0x3F0657C5,0x3F06D182,0x3F074AD3,0x3F07C3B6,0x3F083C2F,
0x3F08B43D,0x3F092BE3,0x3F09A320,0x3F0A19F6,0x3F0A9067,0x3F0B0672,0x3F0B7C1A,0x3F0BF15E,0x3F0C6641,
0x3F0CDAC3,0x3F0D4EE4,0x3F0DC2A7,0x3F0E360B,0x3F0EA912,0x3F0F1BBD,0x3F0F8E0C,0x3F100000,0x3F10719A,
0x3F10E2DC,0x3F1153C4,0x3F11C456,0x3F123491,0x3F12A476,0x3F131406,0x3F138341,0x3F13F229,0x3F1460BE,
0x3F14CF01,0x3F153CF2,0x3F15AA92,0x3F1617E3,0x3F1684E4,0x3F16F196,0x3F175DFA,0x3F17CA11,0x3F1835DC,
0x3F18A15A,0x3F190C8C,0x3F197774,0x3F19E211,0x3F1A4C65,0x3F1AB66F,0x3F1B2032,0x3F1B89AC,0x3F1BF2DF,
0x3F1C5BCB,0x3F1CC471,0x3F1D2CD1,0x3F1D94EC,0x3F1DFCC2,0x3F1E6455,0x3F1ECBA4,0x3F1F32AF,0x3F1F9979,
0x3F200000,0x3F206646,0x3F20CC4A,0x3F21320E,0x3F219792,0x3F21FCD7,0x3F2261DC,0x3F22C6A3,0x3F232B2B,
0x3F238F75,0x3F23F383,0x3F245753,0x3F24BAE7,0x3F251E3E,0x3F25815A,0x3F25E43B,0x3F2646E1,0x3F26A94D,
0x3F270B7F,0x3F276D77,0x3F27CF36,0x3F2830BC,0x3F28920A,0x3F28F31F,0x3F2953FD,0x3F29B4A4,0x3F2A1514,
0x3F2A754D,0x3F2AD550,0x3F2B351D,0x3F2B94B5,0x3F2BF417,0x3F2C5345,0x3F2CB23E,0x3F2D1104,0x3F2D6F95,
0x3F2DCDF3,0x3F2E2C1E,0x3F2E8A16,0x3F2EE7DB,0x3F2F456F,0x3F2FA2D0,0x3F300000,0x3F305CFF,0x3F30B9CC,
0x3F31166A,0x3F3172D6,0x3F31CF13,0x3F322B20,0x3F3286FE,0x3F32E2AC,0x3F333E2C,0x3F33997C,0x3F33F49F,
0x3F344F93,0x3F34AA5A,0x3F3504F3,0x3F355F5F,0x3F35B99E,
};



#endif
 
float sqrtf(float x)
{
  const _UINT32 numbits = (sizeof(sqrt_guess))/(4*64) + 5;

  /* calculated at compile time(hopefully)--assumes minimal # of
     elements in sqrt_guess is 32 or an integral (power of two)*32 
  */
     
  const _UINT32 bit_shift=23-numbits;
  const _UINT32 bit_mask=0x007fffff&(~(sizeof(sqrt_guess)>>2)<<bit_shift);
  const _UINT32 first_several_sig_bits_of_x=(*(_UINT32*)&x) & bit_mask;
  const _INT32 biased_exp=(*(_UINT32*)&x) & 0x7f800000;
  float guess;
  float scaled_x;
 
  //if(*(_UINT32*)&x & 0x80000000) /* either < 0 or -0 */
 // {
  // if((*(_UINT32*)&x) & 0x7fffffff) return NAN;
 //  else return x;  /* x = -0 */
 // }  
  
  //if(!biased_exp) return 0.0f;  //flush denormal to 0.0
  /* the condition below insures that we round x so that ||sqrt(x)-guess||<=||sqrt(x)-y|| for all y in sqrt_guess[](round to nearest)
     since sqrt is monotonically increasing --> ||sqrt(x)-sqrt(guess)|| <= ||sqrt(x)-sqrt(y)||
     we look at the remaining low order significant bits of x below the bit_mask.
  */
#if _EWL_C99  
		if ((x < 0) && (math_errhandling & MATH_ERRNO))	
		{
			_EWL_LOCALDATA(errno) = EDOM;
			return(NAN);
		} 
#endif
   if( biased_exp & 0x00800000)  // if biased_exp is odd then the sqrt of the exponent is 2^^intsqrt(2)
   {
    (*(_UINT32*)&scaled_x)=0x3E800000 + ((*(_UINT32*)&x)&0x007fffff);  //scaled_x in [.25,.5)
    (*(_UINT32*)&guess)=sqrt_guess2[(first_several_sig_bits_of_x>>bit_shift)]; 
    
   }
   else
   {
    (*(_UINT32*)&scaled_x)=0x3f000000 + ((*(_INT32*)&x)&0x007fffff);  //scaled_x in [.5,1.0)
    (*(_UINT32*)&guess)=sqrt_guess[(first_several_sig_bits_of_x>>bit_shift)]; 
   } 
   
   guess += scaled_x/guess;  // now have 12 sig bits
   guess =.25f*guess + (scaled_x/guess);  // now we have about 24 sig bits
              
   /* we now reduce x to 2^^n*y  where y is in [.5,1) we then calculate sqrt(x)=sqrt(2^^n)*sqrt(y) 
      where if n is even we simply shift the exponent of guess appropriately or if n is odd we shift
      and multiply by sqrt(2) if n > 0 and 1/sqrt(2) if n > 0
   */
   
   if(biased_exp > 0x3f000000)
    (*(_INT32*)&guess)+=(((biased_exp - 0x3e800000)>>1)&0xffbfffff) ;  // this subtracts off bias(127=0x3f80...)                                                              // from biased_exp and one more which divides by two                
   else
    (*(_INT32*)&guess)-=((0x3f000000 - biased_exp)>>1)&0xffbfffff;  
    
  return guess;
}  
#endif
