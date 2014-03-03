/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/21 12:43:38 $
 * $Revision: 1.10 $
 */

/********************************************************************************
 *      A collection of functions designed to provide access to the floating    *
 *      point environment for numerical programming. It is modeled after the    *
 *      Numerical C Extensions Group's requirements ( NCEG / X3J11.1 ).         *
 *                                                                              *
 *      The file <fenv.h> declares many functions in support of numerical       *
 *      programming.  It provides a set of environmental controls similar to    *
 *      the ones found in <SANE.h>.  Programs that test flags or run under      *
 *      non-default modes must do so under the effect of an enabling            *
 *      "fenv_access" pragma.                                                   *
 *                                                                              *                                                                              *
 *      Written by Ali Sazegari, started on October 1992.                       *
 ********************************************************************************/

#ifndef _EWL_FENV_H
#define _EWL_FENV_H

#ifndef __FENV__
#define __FENV__	/* JWW - Keep the Apple Universal Interfaces happy */

#include <ansi_parms.h>
#include <ewl_thread_local_data.h>

#if !_EWL_USING_MW_C_HEADERS
	#if !__MACH__
	#error You must have the non-EWL C header file access path before the EWL access path
	#else
	#include <CarbonCore/fenv.h>
	#endif
#else

#if _EWL_C99 || _EWL_C99_MATH_LEAK

#ifndef _EWL_C99_TC2_FENV
	#define _EWL_C99_TC2_FENV 0		/* 1: use newer C99 TC2 fenv prototypes */
#endif

_EWL_BEGIN_NAMESPACE_STD

#ifdef __m56800E__
	/* FP TYPES */
	typedef int		fenv_t;
	typedef int		fexcept_t;
	/* FP EXCEPTION FLAGS */
	#define FE_DIVBYZERO	0x0020
	#define FE_INEXACT	0x0004
	#define FE_INVALID	0x0040
	#define FE_OVERFLOW	0x0010
	#define FE_UNDERFLOW	0x0008
	/* ROUNDING MODES */
	#define FE_DOWNWARD	0x0003
	#define FE_TONEAREST	0x0000
	#define FE_TOWARDZERO	0x0001
	#define FE_UPWARD		0x0002

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif

#ifdef __POWERPC__
	/* FP TYPES */
	typedef unsigned long 	fenv_t;
	typedef unsigned long 	fexcept_t;

	#if __SPE__
		/* FP EXCEPTION FLAGS */
		#define FE_DIVBYZERO	0x00080000u	/* bit 44 of SPEFPSCR[32..63] */
		#define FE_INEXACT		0x00200000u	/* bit 42 */
		#define FE_INVALID		0x00100000u	/* bit 43 */
		#define FE_OVERFLOW		0x00020000u	/* bit 46 */
		#define FE_UNDERFLOW	0x00040000u	/* bit 45 */
	#else
		/* FP EXCEPTION FLAGS */
		#define FE_DIVBYZERO	0x04000000u	/* bit 5 from left of the FPSCR, !!don't forget 2 count bit 0!! */
		#define FE_INEXACT		0x02000000u	/* bit 6 */
		#define FE_INVALID		0x20000000u	/* bit 2 */
		#define FE_OVERFLOW		0x10000000u	/* bit 3 */
		#define FE_UNDERFLOW	0x08000000u	/* bit 4 */
	#endif
	/* ROUNDING MODES */
	#define FE_DOWNWARD		0x00000003u	/* bits 30 - 31 */
	#define FE_TONEAREST	0x00000000u
	#define FE_TOWARDZERO	0x00000001u
	#define FE_UPWARD		0x00000002u

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __POWERPC__ */


#if  defined(__COLDFIRE__)
	#if __COLDFIRE_HW_FPU__
		/* FP TYPES */
		typedef unsigned long fexcept_t;
		typedef struct {
		    unsigned long FPCR;
		    unsigned long FPSR;
		    unsigned long FPIAR;
		} fenv_t;
		/* FP EXCEPTION FLAGS */
		#define FE_INEXACT		0x0200U
		#define FE_DIVBYZERO	0x0400U
		#define FE_UNDERFLOW	0x0800U
		#define FE_OVERFLOW		0x1000U
		#define FE_INVALID		0x2000U
	#else 	/* endif __COLDFIRE_HW_FPU__ */
		/* FP TYPES */
		typedef unsigned short fexcept_t;
		typedef unsigned short fenv_t;
		/* FP EXCEPTION FLAGS */
		#define FE_INVALID		0x8000U
		#define FE_UNDERFLOW	0x2000U
		#define FE_OVERFLOW		0x4000U
		#define FE_DIVBYZERO	0x1000U
		#define FE_INEXACT		0x0800U
	#endif	/* __COLDFIRE__ */

	/* ROUNDING MODES */
	#define FE_TONEAREST		0x0000U
	#define FE_TOWARDZERO		0x0010U
	#define FE_DOWNWARD			0x0020U
	#define FE_UPWARD			0x0030U
	/* PRECISION ROUNDING */
	#define FE_DBLPREC			0x00U
	#define FE_FLTPREC			0x40U

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __COLDFIRE__ */

#ifdef __arm
	/* FP TYPES */
	typedef unsigned int __ieee_edata_t;     /* exception flags passed to traps */

	typedef union {
	    float f;
	    float s;
	    double d;
	    int i;
	    unsigned int ui;
#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__)
	    long long l;
	    unsigned long long ul;
#endif /* __STRICT_ANSI__ */
	    struct { int word1, word2; } str;
	    } __ieee_value_t;                        /* in/out values passed to traps */

	typedef __ieee_value_t (*__ieee_handler_t) (__ieee_value_t, __ieee_value_t,
						__ieee_edata_t);

	typedef struct {
	    unsigned statusword;			/* VFP FPSCR control word/status word  */
	    __ieee_handler_t invalid_handler;
	    __ieee_handler_t divbyzero_handler;
	    __ieee_handler_t overflow_handler;
	    __ieee_handler_t underflow_handler;
	    __ieee_handler_t inexact_handler;
	    } fenv_t, fexcept_t;

	/* FP EXCEPTION FLAGS */
	/* NOTE: These are the FPSCR exception status bits, not the exception enable bits */
	#define FE_INVALID		0x00000001u	/* bit 0 of FPSCR */
	#define FE_DIVBYZERO	0x00000002u	/* bit 1 */
	#define FE_OVERFLOW		0x00000004u	/* bit 2 */
	#define FE_UNDERFLOW	0x00000008u	/* bit 3 */
	#define FE_INEXACT		0x00000010u	/* bit 4 */
	/* ROUNDING MODES */
	#define FE_TONEAREST	0x00000000u	/* FPSCR bits[23:22] = 0b00 */
	#define FE_TOWARDZERO	0x00C00000u	/* FPSCR bits[23:22] = 0b11 */
	#define FE_UPWARD		0x00400000u	/* FPSCR bits[23:22] = 0b01 */
	#define FE_DOWNWARD		0x00800000u	/* FPSCR bits[23:22] = 0b10 */

	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */

#endif /* __arm */

#if __STARCORE__

	typedef struct {
		unsigned long Round_Mode;
		unsigned long FLUSH_TO_ZERO;
		unsigned long IEEE_Exceptions;
		unsigned long EnableFPExceptions;
		unsigned long IEEE_Condition_Codes;
		}		fenv_t;
	typedef unsigned long fexcept_t;

	/* FP EXCEPTION FLAGS */
	#define FE_INEXACT		0x01u
	#define FE_DIVBYZERO	0x02u
	#define FE_UNDERFLOW	0x04u
	#define FE_OVERFLOW		0x08u
	#define FE_INVALID		0x10u

	/* ROUNDING MODES */
	#define FE_TOWARDZERO	0x0000u
	#define FE_TONEAREST	0x0001u
	#define FE_UPWARD		0x0002u
	#define FE_DOWNWARD		0x0003u

	#define ROUND_TOWARDS_ZERO       FE_TOWARDZERO
	#define ROUND_TO_NEAREST_EVEN    FE_TONEAREST
	#define ROUND_TOWARDS_MINUS_INF  FE_UPWARD
	#define ROUND_TOWARDS_PLUS_INF   FE_DOWNWARD

	#if ! _REENTRANT
	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */
	#else
	#define FE_DFL_ENV ((fenv_t *)&(_EWL_LOCALDATA(fp_Round_Mode)))
	#endif
	
#endif /*  __STARCORE__ */

#if defined(__INTEL__) || (__dest_os == __win32_os)
	/* FP TYPES */
	typedef short fexcept_t;
	typedef long fenv_t;			/* control word/status word  */
	/* FP EXCEPTION FLAGS */
	#define FE_INVALID		0x0001
	#define FE_DIVBYZERO	0x0004
	#define FE_OVERFLOW		0x0008
	#define FE_UNDERFLOW	0x0010
	#define FE_INEXACT		0x0020
	/* ROUNDING MODES */
	#define FE_TONEAREST	0x00000000
	#define FE_TOWARDZERO	0x00000C00
	#define FE_UPWARD		0x00000800
	#define FE_DOWNWARD		0x00000400
	/* PRECISION */
	#define FE_LDBLPREC		0x00000300
	#define FE_DBLPREC		0x00000200
	#define FE_FLTPREC		0x00000000

	#if ! _REENTRANT
	extern fenv_t _FE_DFL_ENV;
	#define FE_DFL_ENV &_FE_DFL_ENV				/* pointer to default environment */
	#else
	#define FE_DFL_ENV ((fenv_t *)&(_EWL_LOCALDATA(fp_Round_Mode)))
	#endif
#endif /* __INTEL__ */

#define FE_ALL_EXCEPT (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* PROTOTYPE FUNCTIONS */
_EWL_BEGIN_EXTERN_C

#if _EWL_C99_TC2_FENV

_EWL_IMP_EXP_C int _EWL_MATH_CDECL feclearexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetexceptflag ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetexceptflag ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
#if defined(__arm)
	#define fegetexcept fegetexceptflag
	#define fesetexcept fesetexceptflag
#elif ! __COLDFIRE__
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetexcept ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetexcept ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
#endif
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feraiseexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fetestexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fegetround ( void ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fesetround ( int rnd ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetenv ( fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL feholdexcept ( fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetenv ( const fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_MATH_CDECL feupdateenv ( const fenv_t * envp ) _EWL_CANT_THROW;
#if (defined(__COLDFIRE__) || defined(__INTEL__))
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetprec ( void ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetprec ( int prec ) _EWL_CANT_THROW;
#endif  /* defined(__COLDFIRE__) || defined(__INTEL__) */

#else

_EWL_IMP_EXP_C void _EWL_MATH_CDECL feclearexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fegetexceptflag ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fesetexceptflag ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
#if defined(__arm)
	#define fegetexcept fegetexceptflag
	#define fesetexcept fesetexceptflag
#elif ! __COLDFIRE__
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetexcept ( fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void _EWL_MATH_CDECL fesetexcept ( const fexcept_t * flagp, int excepts ) _EWL_CANT_THROW;
#endif
_EWL_IMP_EXP_C void _EWL_MATH_CDECL feraiseexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fetestexcept ( int excepts ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fegetround ( void ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL fesetround ( int rnd ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fegetenv ( fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int  _EWL_MATH_CDECL feholdexcept ( fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C void _EWL_MATH_CDECL fesetenv ( const fenv_t * envp ) _EWL_CANT_THROW;
_EWL_IMP_EXP_C void _EWL_MATH_CDECL feupdateenv ( const fenv_t * envp ) _EWL_CANT_THROW;
#if defined(__COLDFIRE__)
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fegetprec ( void ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_MATH_CDECL fesetprec ( int prec ) _EWL_CANT_THROW;
#endif  /* __COLDFIRE__ */

#endif /* _EWL_C99_TC2_FENV */

_EWL_END_EXTERN_C

#if defined(__INTEL__)
	#include <x86/fenv_x87.h>
#elif defined(__arm)
    #include <arm/fenv.ARM.h>
#endif

_EWL_END_NAMESPACE_STD

#endif /* __FENV__ */
#endif /* _EWL_C99 */
#endif /* _EWL_USING_MW_C_HEADERS */
#endif /* _EWL_FENV_H */

