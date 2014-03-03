/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/03/05 09:00:36 $
 * $Revision: 1.5 $
 */

#include <fenv.h>
#include <math.h>
#include <float.h>
 
#if _EWL_C99

_EWL_BEGIN_EXTERN_C

#if !defined(_REENTRANT) || ! _REENTRANT

#define fp_Round_Mode _FE_DFL_ENV.Round_Mode
#define fp_FLUSH_TO_ZERO _FE_DFL_ENV.FLUSH_TO_ZERO
#define fp_IEEE_Exceptions _FE_DFL_ENV.IEEE_Exceptions
#define fp_EnableFPExceptions _FE_DFL_ENV.EnableFPExceptions
#define fp_IEEE_Condition_Codes _FE_DFL_ENV.IEEE_Condition_Codes

#else

#include <sc/thread_local_data_StarCore.h>

#endif /* _REENTRANT */


/*
 * Exception functions.
 */
/*
FEGETEXCEPTFLAG
DESCRIPTION:The fegetexceptflag function stores an implementation-defined
            representation of the states of the floating-point status flags
            indicated by the argument excepts in the object pointed to by the
            argument flagp.
*/
int_t _EWL_MATH_CDECL fegetexceptflag(fexcept_t *flagp, int_t excepts)
{
fexcept_t exc = _EWL_LOCALDATA(fp_IEEE_Exceptions);

    /* extract the EXC and AEXC bits */
    *flagp = exc & excepts & FE_ALL_EXCEPT;
	return 0;
}

/*
FESETEXCEPTFLAG
DESCRIPTION:The fesetexceptflag function sets the floating-point status flags
            indicated by the argument excepts to the states stored in the object
            pointed to by flagp. The value of *flagp shall have been set by a
            previous call to fegetexceptflag whose 2nd argument represented at
            least those floating-point exceptions represented by the argument
            excepts. This function does not raise floating-point exceptions, but
            only sets the state of the flags.
*/
int_t _EWL_MATH_CDECL fesetexceptflag(const fexcept_t *flagp, int_t excepts)
{
fexcept_t exc = _EWL_LOCALDATA(fp_IEEE_Exceptions);

    /* set both AEXC and EXC bytes of fpsr */
    exc &= (fexcept_t)~((uint_t)excepts & FE_ALL_EXCEPT);;
    exc |= *flagp & (uint_t)excepts & FE_ALL_EXCEPT;
    _EWL_LOCALDATA(fp_IEEE_Exceptions) = exc;
	return 0;
}

/*
FECLEAREXCEPT
DESCRIPTION:The feclearexcept function clears the supported floating-point
            exceptions represented by its argument.
*/
int_t _EWL_MATH_CDECL feclearexcept(int_t excepts)
{
fexcept_t exc = _EWL_LOCALDATA(fp_IEEE_Exceptions);

    /* clear EXC and AEXC flags */
    exc &= (fexcept_t)~((uint_t)excepts & FE_ALL_EXCEPT);
    _EWL_LOCALDATA(fp_IEEE_Exceptions) = exc;
	return 0;
}

/*
FERAISEEXCEPT
DESCRIPTION:The feraiseexcept function raises the supported floating-point
            exceptions represented by the argument. The order in which these
            floating-point exceptions are raised is unspecified, except as
            stated in ISO/IEC 9899:1999 F.7.6.  Whether the feraiseexcept
            function additionally raises the "inexact" floating-point
            exception whenever it raise the "overflow" or "underflow"
            floating-point exception is implementation-defined.
*/
int_t _EWL_MATH_CDECL feraiseexcept(int_t excepts)
{
volatile f64_t d;

    if ((uint_t)excepts & FE_INVALID) {
        d = HUGE_VAL * HUGE_VAL;
    }

    if ((uint_t)excepts & FE_DIVBYZERO) {
        volatile f32_t f = 1.0F, g = 0.0F;
        f = f / g;
    }

    if ((uint_t)excepts & FE_OVERFLOW) {
        d = LDBL_MAX;
        d = d * 2.0;
    }

    if ((uint_t)excepts & FE_UNDERFLOW) {
        d = -LDBL_MAX;
        d = d / 2.0;
    }

    if ((uint_t)excepts & FE_INEXACT) {
        d = 1.0;
        d = d / 3.0;
    }
	return 0;
}

/*
FETESTEXCEPT
DESCRIPTION: The fetestexcept function determines which of a specified subset of
             the floating-point exception flags are currently set.  The excepts
             argument specifies the floating-point status flags to be queried.
*/
int_t _EWL_MATH_CDECL fetestexcept(int_t excepts)
{
fexcept_t exc = _EWL_LOCALDATA(fp_IEEE_Exceptions);

    MISRA_EXCEPTION_RULE_10_3()
    return (int_t)(exc & ((uint_t)excepts & FE_ALL_EXCEPT));
}

/*
 * Rounding functions.
 */

/*
FEGETROUND
DESCRIPTION:The fegetround function returns the value of the rounding direction
            macro representing the current rounding direction or a negative
            number if there is no such rounding macro or the current rounding
            direction is not determinable.
*/
int_t _EWL_MATH_CDECL fegetround(void)
{
fexcept_t cr = _EWL_LOCALDATA(fp_Round_Mode);

    MISRA_EXCEPTION_RULE_10_3()
	/* available values - 0, 1, 2, 3; FE_DOWNWARD - 0x3*/
    return (int_t)(cr & FE_DOWNWARD);
}

/*
FESETROUND
DESCRIPTION:fesetround function establishes the rounding direction represented
            by its argument round.  If the argument is not equal to the value of
            a rounding direction macro, the rounding direction is not changed.
*/
int_t _EWL_MATH_CDECL fesetround(int_t rnd)
{
uint_t cr = (uint_t)_EWL_LOCALDATA(fp_Round_Mode);

	/* invalid rounding mode */
    if ((uint_t)rnd & (uint_t)~(uint_t)FE_DOWNWARD) {
        return 1;
    }
    cr = (uint_t)rnd;
    _EWL_LOCALDATA(fp_Round_Mode) = cr;
    return 0;
}


/*
 * Full environment access.
 */

/*
FEGETENV
DESCRIPTION: The fegetenv function stores the current floating-point environment
             in the object pointed to by envp.
*/
int_t _EWL_MATH_CDECL fegetenv(fenv_t *envp)
{
    envp->Round_Mode = _EWL_LOCALDATA(fp_Round_Mode);
    envp->FLUSH_TO_ZERO = _EWL_LOCALDATA(fp_FLUSH_TO_ZERO);
    envp->IEEE_Exceptions = _EWL_LOCALDATA(fp_IEEE_Exceptions);
    envp->EnableFPExceptions = _EWL_LOCALDATA(fp_EnableFPExceptions);
    envp->IEEE_Condition_Codes = _EWL_LOCALDATA(fp_IEEE_Condition_Codes);
    return 0;
}

/*
FESETENV
DESCRIPTION:The fesetenv function establishes the floating-point environment
            represented by the object pointed to by envp. The argument envp
            shall point to an object set by a call to fegetenv or feholdenv.
            Note that fesetenv merely installs the state of the floating-point
            status flags represented through its argument, and does not raise
            these floating-point exceptions.
*/
int_t _EWL_MATH_CDECL fesetenv(const fenv_t *envp)
{
    _EWL_LOCALDATA(fp_Round_Mode) = envp->Round_Mode;
    _EWL_LOCALDATA(fp_FLUSH_TO_ZERO) = envp->FLUSH_TO_ZERO;
    _EWL_LOCALDATA(fp_IEEE_Exceptions) = envp->IEEE_Exceptions;
    _EWL_LOCALDATA(fp_EnableFPExceptions) = envp->EnableFPExceptions;
    _EWL_LOCALDATA(fp_IEEE_Condition_Codes) = envp->IEEE_Condition_Codes;
    return 0;
}

/*
FEHOLDEXCEPT
DESCRIPTION:The feholdexcept function saves the current floating-point
            environment in the object pointed to by envp, clears the
            floating-point status flags, and then installs a non-stop (continues
            on floating-point exceptions) mode, if available, for all
            floating-point exceptions. It returns zero if and only if non-stop
            floating-point exception handling was successfully installed.
*/
int_t _EWL_MATH_CDECL feholdexcept(fenv_t *envp)
{
    (void)fegetenv(envp);
    _EWL_LOCALDATA(fp_IEEE_Exceptions) = envp->IEEE_Exceptions & (uint_t)~(uint_t)FE_ALL_EXCEPT;
    return 0;
}

/*
FEUPDATEENV
DESCRIPTION:The feupdateenv function saves the currently raised floating-point
            exceptions in its automatic storage, installs the floating-point
            environment represented by the object pointed to by envp, and then
            raises the saved floating-point exceptions.  The argument envp shall
            point to an object set by a call to feholdexcept or fegetenv.
*/
int_t _EWL_MATH_CDECL feupdateenv(const fenv_t *envp)
{
fexcept_t sr = _EWL_LOCALDATA(fp_IEEE_Exceptions);

  sr &= FE_ALL_EXCEPT;
  (void)fesetenv (envp);
  (void)feraiseexcept((int_t)sr);
  return 0;
}

_EWL_END_EXTERN_C

#endif /*_EWL_C99*/
