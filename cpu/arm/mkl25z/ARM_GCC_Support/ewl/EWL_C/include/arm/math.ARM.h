
/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/01 13:23:11 $
 * $Revision: 1.2 $
 */

/* $Id: math.ARM.h,v 1.2 2010/04/01 13:23:11 rb501c Exp $ */

#ifndef _EWL_MATH_ARM_H
#define _EWL_MATH_ARM_H

#ifndef _EWL_CMATH
#error This header may only be included from <cmath>
#endif

#define __SET_ERRNO__
#ifdef __SET_ERRNO__
	#include <cerrno>
#endif

_EWL_BEGIN_NAMESPACE_STD
	typedef float float_t;
	typedef double double_t;
_EWL_END_NAMESPACE_STD

_EWL_BEGIN_EXTERN_C

/*  
 * private functions 
 */
 
_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL gamma(double);

#if _USE_AEABI_PRIVATE_
#define PREFIX(s) __FSL ## s
#else
#define PREFIX(s) s
#endif
/*
 * Floating-to-integer format conversions, rounding to nearest or
 * configurably.
 */
	extern int PREFIX(_ffix_r)(float);
	extern int PREFIX(_dfix_r)(double);
	
#if _EWL_LONGLONG
	extern long long PREFIX(_ll_sfrom_d_r)(double);
	extern long long PREFIX(_ll_sfrom_f_r)(float);
#endif /* _EWL_LONGLONG */
 
/*
 * Single-precision arithmetic routines.
 */
 
	extern float PREFIX(_frem)(float, float);
	extern float PREFIX(_frnd)(float);

/*
 * Double-precision arithmetic routines.
 */

	extern double PREFIX(_drem)(double, double);

	extern double PREFIX(_drnd)(double);
	extern double PREFIX(_dsqrt)(double);
	extern float PREFIX(_fsqrt)(float);
	
_EWL_END_EXTERN_C

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#if _EWL_USE_INLINE

/* 
 * Calling the ARM floating point library for assembly versions
 */

_EWL_INLINE double _EWL_MATH_CDECL 
	sqrt(double d) { return PREFIX(_dsqrt)(d);}

#if _EWL_C99 
	_EWL_INLINE long  _EWL_CDECL
		lrint(double d) { return PREFIX(_dfix_r)(d);}
#endif

#if _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS
	_EWL_INLINE float _EWL_MATH_CDECL
		acosf(float x) { return (float)(acos)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		acosl(long double x) { return (long double)(acos)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		asinf(float x) { return (float)(asin)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		asinl(long double x) { return (long double)(asin)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		atanf(float x) { return (float)(atan)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		atanl(long double x) { return (long double)(atan)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		atan2f(float y, float x) { return (float)(atan2)((double)(y), (double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		atan2l(long double y, long double x) { return (long double)(atan2)((double)(y), (double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		ceilf(float x) { return (float)(ceil)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		ceill(long double x) { return (long double)(ceil)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		cosf(float x) { return (float)(cos)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		cosl(long double x) { return (long double)(cos)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		coshf(float x) { return (float)(cosh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		coshl(long double x) { return (long double)(cosh)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		expf(float x) { return (float)(exp)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		expl(long double x) { return (long double)(exp)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		fabsf(float x) { return (float)(fabs)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		fabsl(long double x) { return (long double)(fabs)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		floorf(float x) { return (float)(floor)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		floorl(long double x) { return (long double)(floor)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		fmodf(float x, float y) { return (float)(fmod)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		fmodl(long double x, long double y) { return (long double)(fmod)((double)(x), (double)(y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		frexpf(float x, int* y) { return (float)(frexp)((double)(x), (y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		frexpl(long double x, int* y) { return (long double)(frexp)((double)(x), (y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		ldexpf(float x, int y) { return (float)(ldexp)((double)(x), (y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		ldexpl(long double x, int y) { return (long double)(ldexp)((double)(x), (y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		logf(float x) { return (float)(log)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		logl(long double x) { return (long double)(log)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		log10f(float x) { return (float)(log10)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		log10l(long double x) { return (long double)(log10)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		modff(float x, float* iptr) {
	  double iptrd;
	  float result = (float)modf((double)x, &iptrd);
	  *iptr = (float)iptrd;
	  return result;
	}	
	_EWL_INLINE long double _EWL_MATH_CDECL
		modfl(long double x, long double* iptr) {
	  double iptrd;
	  long double result = (long double)modf((double)x, &iptrd);
	  *iptr = (long double)iptrd;
	  return result;
	}
	_EWL_INLINE float _EWL_MATH_CDECL
		powf(float x, float y) { return (float)(pow)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		powl(long double x, long double y) { return (long double)(pow)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		sinl(long double x) { return (long double)(sin)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		sinf(float x) { return (float)(sin)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		sinhf(float x) { return (float)(sinh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		sinhl(long double x) { return (long double)(sinh)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		sqrtf(float x) { return PREFIX(_fsqrt)(x); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		sqrtl(long double x) { return (long double)(sqrt)((double)(x)); }		
	_EWL_INLINE float _EWL_MATH_CDECL
		tanf(float x) { return (float)(tan)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		tanl(long double x) { return (long double)(tan)((double)(x)); }	
	_EWL_INLINE float _EWL_MATH_CDECL
		tanhf(float x) { return (float)(tanh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		tanhl(long double x) { return (long double)(tanh)((double)(x)); }	
#endif /* _EWL_C99 || _EWL_C_HAS_CPP_EXTENSIONS */

#if _EWL_C99		
	_EWL_INLINE float _EWL_MATH_CDECL
		acoshf(float x) { return (float)(acosh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		acoshl(long double x) { return (long double)(acosh)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		asinhf(float x) { return (float)(asinh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		asinhl(long double x) { return (long double)(asinh)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		atanhf(float x) { return (float)(atanh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		atanhl(long double x) { return (long double)(atanh)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		cbrtl(long double x) { return (long double)(cbrt)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		copysignf(float x, float y) { return (float)(copysign)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		copysignl(long double x, long double y) { return (long double)(copysign)((double)(x), (double)(y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		expm1f(float x) { return (float)(expm1)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		expm1l(long double x) { return (long double)(expm1)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		fdimf(float x, float y) { return (float)(fdim)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		fdiml(long double x, long double y) { return (long double)(fdim)((double)(x), (double)(y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		fmaxf(float x, float y) { return (float)(fmax)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		fmaxl(long double x, long double y) { return (long double)(fmax)((double)(x), (double)(y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		fminf(float x, float y) { return (float)(fmin)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		fminl(long double x, long double y) { return (long double)(fmin)((double)(x), (double)(y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		hypotf(float x, float y) { return (float)(hypot)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		hypotl(long double x, long double y) { return (long double)(hypot)((double)(x), (double)(y)); }
#if _EWL_USES_SUN_MATH_LIB
	_EWL_INLINE int _EWL_MATH_CDECL
		ilogbf(float x) { return (ilogb)((double)(x)); }
	_EWL_INLINE int _EWL_MATH_CDECL
		ilogbl(long double x) { return (ilogb)((double)(x)); }
#endif /* _EWL_USES_SUN_MATH_LIB */
#if _EWL_LONGLONG
	_EWL_INLINE long long _EWL_MATH_CDECL 
		llrint(double d) { return PREFIX(_ll_sfrom_d_r)(d);}
	_EWL_INLINE long long _EWL_MATH_CDECL
		llrintf(float x) { return PREFIX(_ll_sfrom_f_r)(x); }
	_EWL_INLINE long long _EWL_MATH_CDECL
		llrintl(long double x) { return PREFIX(_ll_sfrom_d_r)(x);}
#if !_EWL_USES_SUN_MATH_LIB			
	_EWL_INLINE long long _EWL_MATH_CDECL
		llroundf(float x) { return (llround)((double)(x)); }
	_EWL_INLINE long long _EWL_MATH_CDECL
		llroundl(long double x) { return (llround)((double)(x)); }
#endif /* !_EWL_USES_SUN_MATH_LIB */
#endif /* _EWL_LONGLONG */
	_EWL_INLINE long _EWL_CDECL 
		lrintf(float f) { return PREFIX(_ffix_r)(f);}
	_EWL_INLINE float _EWL_MATH_CDECL
		log1pf(float x) { return (float)(log1p)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		log1pl(long double x) { return (long double)(log1p)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		log2f(float x) { return (float)(log2)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		log2l(long double x) { return (long double)(log2)((double)(x)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		logbf(float x) { return (float)(logb)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		logbl(long double x) { return (long double)(logb)((double)(x)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		nextafterl(long double x, long double y) { return (long double)(nextafter)((double)(x), (double)(y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		nexttowardl(long double x, long double y) { return (long double)(nexttoward)((double)(x), (y)); }
	_EWL_INLINE double _EWL_MATH_CDECL 
		remainder(double d, double z) { return PREFIX(_drem)(d,z);}
	_EWL_INLINE float _EWL_MATH_CDECL
		remainderf(float x, float y) { return PREFIX(_frem)(x,y); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		remainderl(long double x, long double y) { return (long double)(remainder)((double)(x), (double)(y)); }
	_EWL_INLINE double _EWL_MATH_CDECL 
		rint(double d) { return PREFIX(_drnd)(d);}
	_EWL_INLINE float _EWL_MATH_CDECL
		rintf(float x) { return PREFIX(_frnd)(x); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		rintl(long double x) { return (long double)PREFIX(_drnd)((double)(x)); }	
	_EWL_INLINE float _EWL_MATH_CDECL
		scalblnf(float x, long int y) { return (float)(scalbln)((double)(x), (y)); }
	_EWL_INLINE long double _EWL_MATH_CDECL
		scalblnl(long double x, long int y) { return (long double)(scalbln)((double)(x), (y)); }
	_EWL_INLINE float _EWL_MATH_CDECL
		scalbnf(float x, int y) { return (float)(scalbn)((double)(x), (y)); }
#if !_EWL_USES_SUN_MATH_LIB	
	_EWL_INLINE long double _EWL_MATH_CDECL
		scalbnl(long double x, int y) { return (long double)(scalbn)((double)(x), (y));}
#endif /* !_EWL_USES_SUN_MATH_LIB */

#endif /* _EWL_C99 */

#endif /* _EWL_USE_INLINE */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_MATH_ARM_H */
