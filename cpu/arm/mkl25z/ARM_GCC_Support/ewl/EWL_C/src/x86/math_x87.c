/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/02/02 13:20:49 $
 * $Revision: 1.7 $
 */

#ifndef _INSIDE_K63D_C_

#include <ansi_parms.h>

#include <float.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 1
#undef	_EWL_INLINE
#define	_EWL_INLINE _EWL_DO_NOT_INLINE

#undef	_EWL_MATH_CDECL
#define	_EWL_MATH_CDECL __cdecl

/*	need to include this so we reference the correct version */
#include <x86/ThreadLocalData.h>

/*	turn off all the options to generate plain old __cdecl code */
#pragma k63d off
#pragma sse off
#pragma sse2 off
#pragma inline_intrinsics off

#endif	/*_INSIDE_K63D_C_*/

#if 0
#pragma mark <cmath> function definitions
#endif

#include <cmath>
#include <fenv.h>

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#define	__HALF			dword ptr 0.5f

/* acos inlined */

/* acosh inlined */

/* asin inlined */

/* asinh inlined */

/* atan inlined */

/* atan2 inlined */

/* atanh inlined */

/* cbrt in math.c */

/* ceil inlined */

/* copysign inlined */

/* cos inlined */

_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL cosh(double x)
{
	#if (_EWL_MATH_ERRHANDLING && MATH_ERRNO)
	    if (x >= HUGE_VAL)
	    {
	        errno = ERANGE;
	        return HUGE_VAL;
	    }
	#endif
	asm
	{
		fld x
		fldl2e			; st=log2(e) st(1)=x
		fmul st,st(1)		; st=y=x*log2(e) 
		fld st			; pushes st into st(1),st(2)=garbage
		fst st(2)		; copies I+f to st(2)
		frndint			; st=I,st(1)=y will work only for round 
					; to nearest or maybe chop
		fxch			;; st=I+f  st(1)=I st(2)=I+f
		fsub st,st(1)		; st=f st(1)=I  st(2)=y=I+f
		fst st(2)
		f2xm1
		fld1
		fadd			; st=2^f st(1)=2^f -1 st(2)=I+f
		fscale			; st=2^f*2^I  st(1)=I st(2)=f

		fxch st(2)		; st=f st1=I st2=2^f
		fchs			; st=-f st1=I st2=2^f
		f2xm1
		fld1
		fadd			; st=2^-f st(1)=I  st(2)=2^f
		fxch
		fchs
		fxch
		fscale
		fstp st(1)		; st=2^-y st(1)=2^y st(2)=y
		fadd
		fld __HALF
		fmul
		fstp x
	}
	return x;
}


/* exp inlined */

/* exp2 inlined */

/* expm1 inlined */

/* fabs inlined */

/* fdim inlined */

/* floor inlined */

/* fmax inlined */

/* fmin inlined */

/* fmod inlined */

/* frexp inlined */

/* hypot inlined */

/* ldexp inlined */

#if (_EWL_MATH_ERRHANDLING && MATH_ERRNO)

_EWL_IMP_EXP_C long _EWL_MATH_CDECL lrint(double x) _EWL_CANT_THROW
{
    long __ret;

    if (x > LONG_MAX || x < LONG_MIN)
    	errno = ERANGE;
    asm
    {
    fld x
    frndint
    fistp __ret 
    }
    return __ret ;
}

#if _EWL_LONGLONG
_EWL_IMP_EXP_C long long _EWL_MATH_CDECL llrint(double x) _EWL_CANT_THROW
{
    long long __ret;

    if (x > LLONG_MAX || x < LLONG_MIN)
    	errno = ERANGE;
    asm
    {
    fld x
    frndint
    fistp __ret 
    }
    return __ret ;
}
#endif

#endif

/* llround inlined */

/* log inlined */

/* log10 inlined */

/* log1p inlined */

/* log2 inlined */

/* logb inlined */

/* lrint inlined */

/* lround inlined */

/* Begin 030529 insert of modf() and modff() */												/*- mm 030529 -*/

/* Although these two functions will compile and run on Mac as well as on Windows, they are
   intended here purely for Windows.  They are stored here so that if a version that will
   compile on the Mac is required, it will be more easily seen.                              */
   
_EWL_IMP_EXP_C double _EWL_MATH_CDECL modf(double x, double* xi) _EWL_CANT_THROW
{
	_UINT32 xhi = __UHI(x);
	_UINT32 s = xhi & 0x80000000;
	int e = (int)(((xhi & 0x7FF00000) >> 20) - 0x3FF);
	if (e >= 52)  /* if x is inf, nan or "big" */
	{
		*xi = x;
		if (e != 0x400 || (xhi & 0x000FFFFF | __ULO(x)) == 0)  /*  if x is inf or big, but not nan */
		{
			/* x = +/- 0 */
			__UHI(x) = s;
			__ULO(x) = 0;
		}
		return x;
	}
	if (e < 0)  /* if abs(x) < 1 */
	{
		/* *xi = +/- 0 */
		__UHI(*xi) = s;
		__ULO(*xi) = 0;
		return x;
	}
	/* 0 <= e < 52*/
	*xi = x;
	/* zero all bits in *xi mantissa [0, 51] at and below bit e */
	if (e > 20)
	{
		__ULO(*xi) &= (0xFFFFFFFF << (52-e));
	}
	else
	{
		__ULO(*xi) = 0;
		__UHI(*xi) &= (0xFFFFFFFF << (20-e));
	}
	x -= *xi;
	/* force sign to original sign even if x == 0 */
	__UHI(x) |= s;
	return x;
}

_EWL_IMP_EXP_C float _EWL_MATH_CDECL modff(float x, float* xi) _EWL_CANT_THROW
{
	_UINT32 ix = *(_UINT32*)&x;
	_UINT32 s = ix & 0x80000000;
	int e = (int)(((ix & 0x7F800000) >> 23) - 0x7F);
	if (e >= 23)  /* if x is inf, nan or "big" */
	{
		*xi = x;
		if (e != 0x80 || (ix & 0x007FFFFF) == 0)  /*  if x is inf or big, but not nan */
		{
			/* x = +/- 0 */
			*(_UINT32*)&x = s;
		}
		return x;
	}
	if (e < 0)  /* if abs(x) < 1 */
	{
		/* *xi = +/- 0 */
		*(_UINT32*)xi = s;
		return x;
	}
	/* 0 <= e < 23*/
	*xi = x;
	/* zero all bits in *xi mantissa [0, 22] at and below bit e */
	*(_UINT32*)xi &= (0xFFFFFFFF << (23-e));
	x -= *xi;
	/* force sign to original sign even if x == 0 */
	*(_UINT32*)&x |= s;
	return x;
}

/*  End 030529 insert of modf() and modff() */


/* nan inlined */

/* nearbyint inlined */


_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL pow(double x, double y)
{
	short TEMP ;
	long  LONGTEMP;

	#define ONE		qword ptr 1.0
	#define HALF	qword ptr 0.5
	#define QNAN	dword ptr 0x7fffffff
	#define TRUNC	word ptr 0x0f7f

	if(x > 0)
	{ 
		asm
		{
			fld y
			fld x 
			fyl2x	        ; st=y*log2(x) overwrites st(1) and pops stack
			fld st	        ; backup I+f
			frndint	        ; st=I st(1)=y*log2(x)
			fxch
			fsub st,st(1)	; st(1)-st=st=f overwrites st(1)
			f2xm1
			fld1 
			fadd			; st=2^f st(1)=2^f-1 st(2)=I
			fscale
			fstp st(1)	
			fstp x
		}
		if (isnan(x))
			errno = ERANGE;
		return x; 
	}
 
	if((x==0.0 && y <= 0) || (x < 0 && (rint(y) != y)))
	{
		errno=EDOM;

#if _EWL_C99											/*- mm 030528 -*/
		if(x == 0.0 && y < 0.0) 
			return INFINITY ;
		else 
			return NAN ;
#endif /* _EWL_C99 */									/*- mm 030528 -*/
	}

	 /* if we're here we know the base is not positive and exponent integral */
 
	asm
	{
	 	fld y
	 	fld x 
		mov TEMP,1		   ; used at end
		ftst
		fstsw ax
		and ax,0x4000      ; c3=0 iff base (x) is zero(nan case taken care of by above)
	    			       ; zf=0 if base is zero
		jz negative_base
		mov TEMP, 0
		jmp zero_base
	negative_base:		; test for even or odd
		fxch
		fld st
		fistp LONGTEMP
		mov al, byte ptr[LONGTEMP]
	    test al,0x01
		fxch
		fchs			; base,exp combo legit so we change
					    ; sign of base to make it >0
		jz normal_pow
		mov TEMP,-1
	normal_pow:
		fyl2x			; st=y*log2(x) overwrites st(1) and pops stack
		fld st			; backup I+f
		frndint			; st=I st(1)=y*log2(x)
		fxch
		fsub st,st(1)	; st(1)-st=st=f overwrites st(1)
		f2xm1
		fld1 
		fadd			; st=2^f st(1)=2^f-1 st(2)=I
		fscale	
	zero_base:	
		fstp st(1)	
		fimul TEMP		; change sign of result if exponent is odd
		fstp x
	}
	
	#undef ONE
	#undef HALF
	#undef QNAN
	#undef TRUNC
	
	return x;
}

/* remainder inlined */

/* remquo inlined */

/* rint inlined */

/* round inlined */

/* scalbln inlined */

/* scalbn inlined */

/* sin inlined */

_EWL_IMP_EXP_C  double      _EWL_MATH_CDECL sinh(double x)
{
	#if (_EWL_MATH_ERRHANDLING && MATH_ERRNO)
	    if (x >= HUGE_VAL)
	    {
	        errno = ERANGE;
	        return HUGE_VAL;
	    }
	#endif
	asm
	{
		fld x
		fldl2e			; st=log2(e) st(1)=x
		fmul			; st=y=x*log2(e) st(1)=log2(e)
		fld st			; st=st(1)=y
		frndint			; st=I,st(1)=y will work only for round 
				; to nearest or maybe chop
		ftst			; if st=0 i.e. y is small -> we
		fnstsw ax
		sahf
		jnz big_y		; neednt do a fscale when I=0.
		fstp st			; pop stack(dont need I anymore I=0)
		fld st
		; fst st(1)		; st==st(1)=f (need a backup for -f)
		f2xm1
		fxch
		fchs
		f2xm1
		jmp common_sinh
		big_y:
		fxch
		fsub st,st(1)		; st=y st(1)=I  o.k. to bash y now

		fld st(1)		; st=st(1) st=stold st2=st1old
		fxch 	
		fst st(2)		; st=f=y-I st(1)=I st(2)=f
		f2xm1
		fld1
		fadd
		fscale			; st=2^f st(1)=I st(2)=f
		fxch st(2)		; st=f st1=I st2=2^f
		fchs			; st=-f st1=I st2=2^f
		f2xm1
		fld1
		fadd			; st=2^-f st(1)=I  st(2)=2^f
		fxch
		fchs
		fxch
		fscale
		fstp st(1)		; st=2^-y st(1)=2^y st(2)=y

		common_sinh:	
		fsub
		fld __HALF
		fmul
		fstp x
	}
	return x;
}

/* sqrt inlined */

/* tan inlined */

/* tanh inlined */


#define _INSIDE_X87D_C_
#include <math_api.c>
#include <math_float.c>
#include <math_double.c>
#include <math_longdouble.c>
#include <math_fma.c>


/* trunc inlined */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

/* Change record: 
 * mf  980405 updated fmax,fmin,fdim to C9X to return a non-nan value when 
 *            exactly one argument is a nan.
 * mf  980420 fixed round and roundtol     
 * mf  980608 performance enhancements, __declspec(naked)
 *            whenever possible(shrinks codesize by about 10% when combined
 *            with other minor fixes(elimination of local variables etc.)  
 * mf  980610 fixed bug in acosh(was using fmul instead of fmul st,st(1)(i.e. was popping)				 
 * mf  980710 rewrote copysign
 * mf  980713 bug in asinh(wasn't restoring ebp)
 * mf  981113 log10 wasn't poping argument from fp stack when errno occured
 * mf  990721 fixed bug in fmin(IR9907-1375)(completely rewrote fmin/max/dim
 *            math_wrapper.c routines were sharing an unprotected/volitale static(TEMP).  
 *            Each routine now has it's own local to make it threadsafe.  
 *            Fixed bug in expm1 as reported by Fred Tydeman.
 * mf  990828 temporary workaround to compiler bug--
 *            removed const designation from LOG_ONEPX_MAX
 * mf  991007 fixed bug in round introduced w/threadsafe fix. 
 *            removed workaround in previous note.
 * pmk 010404 moved math_wrapper.obj into math_wrapper.c
 * cc  011115 Added defines to new inline flags
 * ejs 020124 Moved math_wrapper.c to math_x87.c, added remaining functions
 * ejs 020222 Exclude intrinsic functions from inlining.
 * JWW 020308 Use new _EWL_DO_NOT_INLINE macro for bringing inlined functions into the library
 * mm  030528 Added _EWL_C99 wrappers.
 */
