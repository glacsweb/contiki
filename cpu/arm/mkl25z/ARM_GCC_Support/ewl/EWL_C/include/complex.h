/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/18 13:14:25 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_COMPLEX_H
#define _EWL_COMPLEX_H

#ifdef __cplusplus

#include <complex>

#ifndef _EWL_NO_CPP_NAMESPACE
	#if _EWL_FLOATING_POINT && !defined(_EWL_NO_MATH_LIB)
		using std::complex;
	#endif
#endif

#else /* __cplusplus */

#include <ansi_parms.h>

#if !_EWL_USING_MW_C_HEADERS
	#if !__MACH__
	#error You must have the non-MSL C header file access path before the MSL access path
	#endif
#else

#if _EWL_COMPLEX_SUPPORT

#define complex			_Complex
#define imaginary		_Imaginary
#define	_Complex_I		(0.0f + __builtin__Imaginary_I)
#define _Imaginary_I	__builtin__Imaginary_I
#define I				_Imaginary_I

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

/* Trigonometric functions */

_EWL_IMP_EXP_C double complex       _EWL_CDECL cacos (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cacosf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cacosl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL casin (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL casinf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL casinl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL catan (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL catanf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL catanl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL ccos (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ccosf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ccosl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL csin (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csinf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csinl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL ctan (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ctanf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ctanl(long double complex z);

/* Hyperbolic functions */

_EWL_IMP_EXP_C double complex       _EWL_CDECL cacosh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cacoshf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cacoshl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL casinh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL casinhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL casinhl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL catanh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL catanhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL catanhl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL ccosh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ccoshf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ccoshl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL csinh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csinhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csinhl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL ctanh (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL ctanhf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL ctanhl(long double complex z);

/* Exponential and logarithmic functions */

_EWL_IMP_EXP_C double complex       _EWL_CDECL cexp (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cexpf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cexpl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL clog (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL clogf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL clogl(long double complex z);

/* Power and absolute value functions */

_EWL_IMP_EXP_C double      _EWL_CDECL cabs (double complex z);
_EWL_IMP_EXP_C float       _EWL_CDECL cabsf(float complex z);
_EWL_IMP_EXP_C long double _EWL_CDECL cabsl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL cpow (double complex x, double complex y);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cpowf(float complex x, float complex y);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cpowl(long double complex x, long double complex y);

_EWL_IMP_EXP_C double complex       _EWL_CDECL csqrt (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL csqrtf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL csqrtl(long double complex z);

/* Manipulation functions */

_EWL_IMP_EXP_C double      _EWL_CDECL carg (double complex z);
_EWL_IMP_EXP_C float       _EWL_CDECL cargf(float complex z);
_EWL_IMP_EXP_C long double _EWL_CDECL cargl(long double complex z);

_EWL_INLINE _EWL_IMP_EXP_C double  _EWL_CDECL cimag(double complex z)
 {
     union u
     {
             double complex cplx;
             double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }

_EWL_INLINE _EWL_IMP_EXP_C float _EWL_CDECL cimagf(float complex z)
 {
     union u
     {
             float complex cplx;
             float arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }

_EWL_INLINE _EWL_IMP_EXP_C long double _EWL_CDECL cimagl(long double complex z)
 {
     union u
     {
             long double complex cplx;
             long double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[1];
 }


_EWL_IMP_EXP_C double complex       _EWL_CDECL conj (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL conjf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL conjl(long double complex z);

_EWL_IMP_EXP_C double complex       _EWL_CDECL cproj (double complex z);
_EWL_IMP_EXP_C float complex        _EWL_CDECL cprojf(float complex z);
_EWL_IMP_EXP_C long double complex  _EWL_CDECL cprojl(long double complex z);

_EWL_INLINE _EWL_IMP_EXP_C double _EWL_CDECL creal(double complex z)
 {
     union u
     {
             double complex cplx;
             double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }
 
 
_EWL_INLINE _EWL_IMP_EXP_C float _EWL_CDECL crealf(float complex z)
 {
     union u
     {
             float complex cplx;
             float arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }
 
_EWL_INLINE _EWL_IMP_EXP_C long double _EWL_CDECL creall(long double complex z)
 {
     union u
     {
             long double complex cplx;
             long double arr[2];
     } temp;
     temp.cplx = z;
     return temp.arr[0];
 }


_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_COMPLEX_SUPPORT */

#endif /* _EWL_USING_MW_C_HEADERS */

#endif /* __cplusplus */

#endif /* _EWL_COMPLEX_H */
