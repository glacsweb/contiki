/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:57:15 $
 * $Revision: 1.1 $
 */

/*  Single precision hyperbolic functions tanh,sinh,cosh
    NOTE: these are just temporary implementations which will be rewritten.
*/
    
#include <math.h>

#if !__BUILD_FDLIBM

#if _EWL_C99 || defined(_EWL_USES_SP_MATH_LIB)

_EWL_IMP_EXP_C float _EWL_MATH_CDECL sinhf(float x)
{
  return .5f*(expf(x) -expf(-x));
}


_EWL_IMP_EXP_C float _EWL_MATH_CDECL coshf(float x)
{
return .5f*(expf(x) + expf(-x));
}

_EWL_IMP_EXP_C float _EWL_MATH_CDECL tanhf(float x)
{
  return (1.0f - expf(-2.0f*x))/(1.0f + expf(-2.0f*x));
}

#endif

#endif /* !__BUILD_FDLIBM */