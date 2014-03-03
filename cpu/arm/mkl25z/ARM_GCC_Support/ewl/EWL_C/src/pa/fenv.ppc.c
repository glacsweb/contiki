/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:22 $
 * $Revision: 1.7 $
 */

/*
	SUMMARY: definitions for the following

		FECLEAREXCEPT - see C99 std for exact definitions of functions.
		FEGETEXCEPTFLAG
		FERAISEEXCEPT
		FESETEXCEPTFLAG
		FETESTEXCEPT

		FEGETROUND
		FESETROUND

		FEGETENV
		FEHOLDEXCEPT
		FESETENV
		FEUPDATEENV


  NAMING CONVENTIONS USED:

	register fenv_t    		ENV = *envp;
	register const fenv_t   *ENV_Ptr = envp;
	register fenv_t    		EXCEPTIONS = excepts;
	register fexcept_t		FLAG = *flagp;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register int_t 			ROUND = round;
	register int_t 			mask;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem ;

*/
#include <ansi_parms.h>

#if _EWL_C99

#include <fenv.h>
#include <ewl_misra_types.h>

/*lint -e529, -e530 arguments and locals are often build dependent, turn off usage warnings */
/*lint -e950 turn off asm comments */

#if !defined(_No_Floating_Point_Regs)

#define fieldmask 255		/* which bits in the FPSCR do you want to affect? 255 = all of them */

/* DEFINE FUNCTIONS */



/*
FECLEAREXCEPT
DESCRIPTION: The feclearexcept function clears the supported
floating-point exceptions represented by it's argument.
*/
_EWL_IMP_EXP_C void feclearexcept ( int_t excepts )
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFCu);		/* ignore attempts 2 chng rounding mode */

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
#if _GCCPORT_
__asm(
	"\tmffs %0\n"										/* load FPSCR in2 GPR */
	"\tstfd %0,0(%3)\n"
	"\tlwz   %1, 4(%3)\n"
	"\tandc  %1, %1,%4\n" 								/* clear exceptions */
	"\tstw   %1, 4(%3)\n"
	"\tlfd   %0,0(%3)\n"
	"\tmtfsf 255,%0\n"									/* load back in2 FPSCR */
	:"=r"(FPSCR),"=r"(FPSCR_Temp),"=r"(FPSCR_Mem)
	:"0"(FPSCR),"1"(FPSCR_Temp),"r"(EXCEPTIONS),"r"(*FPSCR_MemPtr)
	);
#else
	asm
	{
		mffs  FPSCR										/* load FPSCR in2 GPR */
		stfd  FPSCR, 	  0(FPSCR_MemPtr)
		lwz   FPSCR_Temp, 4(FPSCR_MemPtr)
		andc  FPSCR_Temp, FPSCR_Temp,    EXCEPTIONS 	/* clear exceptions */
		stw   FPSCR_Temp, 4(FPSCR_MemPtr)
		lfd   FPSCR,      0(FPSCR_MemPtr)
		mtfsf fieldmask,FPSCR							/* load back in2 FPSCR */
	}
#endif
 	(void)fetestexcept((int_t)EXCEPTIONS);
}

/*
FEGETEXCEPTFLAG
DESCRIPTION: The fegetexceptflag function stores an implementation-defined representation of the states
of the floating-point status flags indicated by the argument excepts in the object pointed to by the argument flagp.
*/
_EWL_IMP_EXP_C void fegetexceptflag (fexcept_t *flagp, int_t excepts)
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 			FPSCR_Temp3;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

#if _GCCPORT_
__asm(
	"\t mffs %0\n"										/* load FPSCR in2 GPR */
	"\tstfd %0, 0(%4)\n"
	"\tlwz %2,4(%4)\n"
	"\tand %1,%2,%5\n"									/* get pertinent values of FPSCR */
	"\tstw %1,0(%6)\n"									/* load in2 obj pnted 2 by flagp */
	"\tlwz %3,0(%6)\n"
	:"=r"(FPSCR),"=r"(FPSCR_Temp2)
	:"0"(FPSCR),"r"(FPSCR_Temp1),"r"(FPSCR_Temp3),"r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"1"(FPSCR_Temp2),"r"(*FLAG_Ptr)
		);
#else
	asm
	{
		mffs FPSCR										/* load FPSCR in2 GPR */
		stfd FPSCR,       0(FPSCR_MemPtr)
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)
		and  FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS		/* get pertinent values of FPSCR */
		stw  FPSCR_Temp2, 0(FLAG_Ptr)					/* load in2 obj pnted 2 by flagp */

		lwz FPSCR_Temp3, 0(FLAG_Ptr)
	}
#endif

}

/*
FERAISEEXCEPT
DESCRIPTION: The feraiseexcept function raises the supported floating-point exceptions represented by the argument.
The order in which these floating-point exceptions are raised is unspecified, except as stated in ISO/IEC 9899:1999 F.7.6.
Whether the feraiseexcept function additionally raises the "inexact" floating-point exception whenever it raise the
"overflow" or "underflow" floating-point exception is implementation-defined.
*/
_EWL_IMP_EXP_C void feraiseexcept (int_t excepts )
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT; /*MASK */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

#if _GCCPORT_
__asm(
	"\tmffs %0\n"										/* load FPSCR in2 GPR */
	"\tstfd %0,0(%2)\n"
	"\tlwz %3, 4(%2)\n"
	"\tor %1, %3, %4\n"									/* raise exceptions */
	"\tstw %1,4(%2)\n"
	"\tlfd %0, 0(%2)\n"
	"\tmtfsf 255, %0\n"									/* load back in2 FPSCR */
	:"=r"(FPSCR),"=r" (FPSCR_Temp2)
	:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2)
	);
#else
	asm
	{
		mffs  FPSCR										/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
		or    FPSCR_Temp2, FPSCR_Temp1,  EXCEPTIONS		/* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR						/* load back in2 FPSCR */
	}
#endif
		/* we need to raise exceptions in stored in FPSCR_Temp2 */
}

/*
FESETEXCEPTFLAG
DESCRIPTION: The fesetexceptflag function sets the floating-point
status flags indicated by the argument excepts to the states stored
in the object pointed to by flagp.  The value of *flagp
shall have been set by a previous call to fegetexceptflag whose
2nd argument represented at least those floating-point exceptions
represented by the argument excepts.  This function does not raise
floating-point exceptions, but only set the state of the flags.
*/
_EWL_IMP_EXP_C void fesetexceptflag (const fexcept_t *flagp, int_t excepts)
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;		/* what exceptions to raise */	 /* mask */
	register fexcept_t		FLAG = *flagp;				/* what values to raise 2 */
	register int_t 			mask;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 			FPSCR_Temp3;

	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

	if (!EXCEPTIONS) {
		MISRA_EXCEPTION_RULE_14_7()
		return;
	}
#if _GCCPORT_
__asm(
	"\tand %2,%5,%7\n"
	"\tmffs %0\n"										/* load FPSCR in2 GPR */
	"\tstfd  %0,0(%3)\n"
	"\tlwz   %4, 4(%3)\n"
	"\tor 	  %1, %4, %2\n"								/* raise exceptions */
	"\tstw   %1, 4(%3)\n"
	"\tlfd   %0,0(%3)\n"
	"\tmtfsf 255,%0\n"									/* load back in2 FPSCR */

	"\tmffs  %0\n"										/* load FPSCR in2 GPR */
	"\tstfd  %0,0(%3)\n"
	"\tlwz %6, 4(%3)\n"
	:"=r"(FPSCR),"=r" (FPSCR_Temp2), "=r"(mask)
	:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2) ,"r"(FPSCR_Temp3),"r"(FLAG), "2"(mask));
#else
	asm
	{
		and   mask,        EXCEPTIONS,      FLAG
		mffs  FPSCR										/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
		or 	  FPSCR_Temp2, FPSCR_Temp1, mask			/* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR						/* load back in2 FPSCR */

		mffs  FPSCR										/* load FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz FPSCR_Temp3, 4(FPSCR_MemPtr)
	}
#endif
}

/*
FETESTEXCEPT
DESCRIPTION: The fetestexcept function determines which of a specified subset of the floating-point exception flags are
currently set.  The excepts argument specifies the floating-point status flags to be queried.
*/
_EWL_IMP_EXP_C int_t fetestexcept(int_t excepts)
{
	register fenv_t    		EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT; /* mask*/
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	MISRA_EXCEPTION_RULE_12_7()
	excepts =               (excepts & 0xFFFFFFFC);

#if _GCCPORT_
__asm(
	"\tmffs %0\n"										/* load FPSCR in2 GPR */
	"\tstfd %0,0(%2)\n"
	"\tlwz %3, 4(%2)\n"
	"\tand %1, %3,%4\n"									/* test exceptions */
	:"=r"(FPSCR), "=r"(FPSCR_Temp2)
	:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1),"r"(EXCEPTIONS), "0"(FPSCR)
	);
#else
	asm
	{
		mffs FPSCR										/* load FPSCR in2 GPR */
		stfd FPSCR,		  0(FPSCR_MemPtr)
		lwz  FPSCR_Temp1, 4(FPSCR_MemPtr)
		and  FPSCR_Temp2, FPSCR_Temp1,	 EXCEPTIONS		/* test exceptions */
	}
#endif
	return FPSCR_Temp2;									/* return result of test */
}

/*
FEGETROUND
DESCRIPTION: The fegetround function returns the value of the rounding direction macro representing the current rounding
direction or a negative number if there is no such rounding macro or the current rounding direction is not determinable.
*/
_EWL_IMP_EXP_C int_t fegetround (void)
{
	register int_t 			ROUND;
	register int_t 			mask = 0x4;				/* only want bits 30 and 31 of FPSCR */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;

#if _GCCPORT_
__asm(
	"\tmffs %0\n"									/* load FPSCR in2 GPR */
	"\tstfd %0, 0(%2)\n"
	"\tlwz %3,0(%2)\n"
	"\tand %1, %3, %4\n"							/* get rounding mode */
	:"=r"(FPSCR),"=r"(ROUND)
	:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp),"r"(mask),"0"(FPSCR));
#else
	asm
	{
		mffs FPSCR									/* load FPSCR in2 GPR */
		stfd FPSCR,      0(FPSCR_MemPtr)
		lwz  FPSCR_Temp, 4(FPSCR_MemPtr)
		and  ROUND,      FPSCR_Temp,     mask		/* get rounding mode */
	}
#endif
	return ROUND;									/* return rounding mode */
}

/*
FESETROUND
DESCRIPTION: fesetround function establishes the rounding direction represented by its argument round.  If the argument
is not not equal to the value of a rounding direction macro, the rounding direction is not changed.
*/
_EWL_IMP_EXP_C int_t fesetround (int_t excepts )
{
	if (excepts <= 0x4) 								/* verify argument is a rounding mode */
	{
		register fenv_t 	EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
		register f32_t		FPSCR;
		register int_t 		FPSCR_Temp1;
		register int_t 		FPSCR_Temp2;
		f64_t				FPSCR_Mem;
		register f64_t		*FPSCR_MemPtr = &FPSCR_Mem;
#if _GCCPORT_
__asm(
	"\tmffs %0\n"									/* load FPSCR in2 GPR */
	"\tstfd %0,0(%2)\n"
	"\tlwz %3, 4(%2)\n"
	"\tor %1, %3, %4\n"								/* raise exceptions */
	"\tstw %1,4(%2)\n"
	"\tlfd %0, 0(%2)\n"
	"\tmtfsf 255, %0\n"								/* load back in2 FPSCR */
	:"=r"(FPSCR),"=r" (FPSCR_Temp2)
	:"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1), "r"(EXCEPTIONS),"0"(FPSCR),"1"(FPSCR_Temp2)
	);
#else
		asm
		{
		mffs  FPSCR									/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)
		or    FPSCR_Temp2, FPSCR_Temp1,EXCEPTIONS	/* raise exceptions */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,	   0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR					/* load back in2 FPSCR */
	}
#endif
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	} else {	/* argument is not a rounding mode! return any # */
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
}

/*
FEGETENV
DESCRIPTION: The fegetenv function stores the current
floating-point environment in the object pointed to by flagp.
*/
_EWL_IMP_EXP_C void fegetenv (fexcept_t *flagp)
{
	register fexcept_t 		*FLAG_Ptr = flagp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register fenv_t    		EXCEPTIONS 	= 	FE_ALL_EXCEPT;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;

#if _GCCPORT_
__asm(
	"\tmffs %0\n"
	"\tstfd %0, 0(%2)\n"
	"\tlwz %1,4(%2)\n"
	"\tand %1, %1, %3\n"							/* we need to mask with fe_all exept */
	"\tstw %1,0(%4)\n"								/* save in2 obj pointed 2 by flagp */

	"\tlwz %5, 4(%4)\n"
	:"=r"(FPSCR),"=r"(FPSCR_Temp)
	:"0"(FPSCR),"1"(FPSCR_Temp), "r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"r"(*FLAG_Ptr),"r"(FPSCR_Temp1)
	);
#else
	asm
	{
		mffs FPSCR
		stfd FPSCR,    0(FPSCR_MemPtr)
		lwz FPSCR_Temp,4(FPSCR_MemPtr)
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS	/* we need to mask with fe_all exept */
		stw FPSCR_Temp,0(FLAG_Ptr)					/* save in2 obj pointed 2 by flagp */

		lwz FPSCR_Temp1, 4(FLAG_Ptr)
	}
#endif
}

/*
FEHOLDEXCEPT
DESCRIPTION: The feholdexcept function saves the current
floating-point environment in the object pointed to by envp,
clears the floating-point status flags, and then installs a
non-stop (continues on floating-point exceptions) mode,
if available, for all floating-point exceptions.
*/
MISRA_EXCEPTION_RULE_16_7()
_EWL_IMP_EXP_C int_t feholdexcept (fenv_t *envp)
{
	register const fenv_t   *ENV_Ptr = envp;
	register int_t 	   		mask = 0x4; 			/* clear everything except rounding mode */
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	FE_ALL_EXCEPT;

#if _GCCPORT_
__asm(
		"\tmffs  %0\n"								/* load FPSCR in2 GPR */
		"\tstfd  %0,0(%3)\n"
		"\tlwz  %1,  4(%3)\n"
		"\tand   %1, %1, %4\n"						/* we need to mask with fe_all exept */
		"\tstw   %1,  0(%5)\n"						/* save FPSCR to obj pnted 2 by envp */
		"\tand %2, %1, %6\n"						/* clear fp environment bits in FPSCR */
		"\tstw %7, 4(%3)\n"
		"\tlfd %0, 0(%3)\n"
		"\tmtfsf 255,   %0\n"						/* load into FPSCR */
		:"=r"(FPSCR), "=r"(FPSCR_Temp),"=r"(FPSCR_Temp1)
		:"r"(*FPSCR_MemPtr),"r"(EXCEPTIONS),"r"(*ENV_Ptr),"r"(mask),"r"(FPSCR_Temp2),"1"(FPSCR_Temp),"0"(FPSCR)
		);
#else
	asm
	{
		mffs  FPSCR									/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp,  4(FPSCR_MemPtr)
		and   FPSCR_Temp, FPSCR_Temp, EXCEPTIONS	/* we need to mask with fe_all exept */
		stw   FPSCR_Temp,  0(ENV_Ptr)				/* save FPSCR to obj pnted 2 by envp */
		and   FPSCR_Temp1, FPSCR_Temp, mask			/* clear fp environment bits in FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR					/* load into FPSCR */
	}
#endif

	return -1;										/* "non-stop" was not installed */
}


/*
FESETENV
DESCRIPTION: The fesetenv function establishes the floating-point
environment represented by the oject pointed to by envp.  The argument
envp shall point to an object set by a call to fegetenv or feholdenv,
or equal a floating-point macro.  Note that fesetenv merely installs
the state of the floating-point status flags represented through its
argument, and does not raise these floating-point exceptions.
*/
MISRA_EXCEPTION_RULE_16_7()
_EWL_IMP_EXP_C void fesetenv (const fenv_t *envp)
{
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register f32_t	   		FPSCR;
	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;
	register int_t			FPSCR_Temp;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;

#if _GCCPORT_
__asm(
		"\tmffs  %0\n"								/* load FPSCR in2 GPR */
		"\tstfd  %0,     0(%2)\n"
		"\tlwz %1,4(%2)\n"							/* store whats important */
		"\tand %1, %1, %4\n"						/* remove the FE exceptions */
		"\tor 	%1, %5, %6\n"
		"\tstw   %1, 4(%2)\n"
		"\tlfd %0, 0(%2)\n"
		"\tmtfsf 255, %0\n"							/* load back into FPSCR */

		"\tmffs  %0\n"								/* reload FPSCR in2 GPR */
		"\tstfd  %0,0(%2)\n"
		"\tlwz   %3,  4(%2)\n"						/* remove the FE exceptions */
		:"=r"(FPSCR),"=r"(FPSCR_Temp1)
		:"0"(FPSCR), "1"(FPSCR_Temp1),"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp2),"r"(EXCEPTIONS),"r"(FPSCR_Temp),"r"(ENV)
	);
#else
	asm
	{
		mffs  FPSCR									/* load FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz FPSCR_Temp1,4(FPSCR_MemPtr)				/* store whats important */
		and   FPSCR_Temp1, FPSCR_Temp1, EXCEPTIONS	/* remove the FE exceptions */
		or 	  FPSCR_Temp1, FPSCR_Temp, ENV
		stw   FPSCR_Temp1, 4(FPSCR_MemPtr)
		lfd   FPSCR,     0(FPSCR_MemPtr)
		mtfsf fieldmask, FPSCR						/* load back into FPSCR */

		mffs  FPSCR									/* reload FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp2,  4(FPSCR_MemPtr)			/* remove the FE exceptions */

	}
#endif
}

/*
FEUPDATEENV
DESCRIPTION: The feupdateenv function saves the currently raised
floating-point exceptions in its automatic storage, installs the
floating-point environment represented by the object pointed to
by envp, and then raises the saved floating-point exceptions.
The argument envp shall point to an object set by a call to feholdexcept
or fegetenv, or equal a floating-point macro.
*/

_EWL_IMP_EXP_C void feupdateenv (const fenv_t *envp)
{
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register const fenv_t   *ENV_Ptr = envp;
	register f32_t	   		FPSCR;
	register int_t 	   		FPSCR_Temp1;
	register int_t 	   		FPSCR_Temp2;
	register int_t 	   		FPSCR_Temp3;

	f64_t			   		FPSCR_Mem;
	register f64_t    		*FPSCR_MemPtr = &FPSCR_Mem;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

#if _GCCPORT_
__asm(
		"\tmffs  %0\n"								/* load FPSCR in2 GPR */
		"\tstfd  %0,     0(%2)\n"
		"\tlwz %3,4(%2)\n"							/* store whats important */
		"\tand %1, %3, %4\n"						/* remove the FE exceptions */
		"\tor 	%1, %5, %6\n"
		"\tstw   %1, 4(%2)\n"
		"\tlfd %0, 0(%2)\n"
		"\tmtfsf 255, %0\n"							/* load back into FPSCR */

		"\tmffs  %0\n"								/* reload FPSCR in2 GPR */
		"\tstfd  %0,0(%2)\n"
		"\tlwz   %7,  4(%2)\n"						/* remove the FE exceptions */
		:"=r"(FPSCR),"=r"(FPSCR_Temp2)
		:"0"(FPSCR), "1"(FPSCR_Temp2),"r"(*FPSCR_MemPtr),"r"(FPSCR_Temp1),"r"(EXCEPTIONS),"r"(ENV),"r"(FPSCR_Temp3)
	);
#else
	asm
	{
		mffs  FPSCR									/* load FPSCR in2 GPR */
		stfd  FPSCR,       0(FPSCR_MemPtr)
		lwz   FPSCR_Temp1, 4(FPSCR_MemPtr)			/* store whats important */
		and FPSCR_Temp2, FPSCR_Temp1, EXCEPTIONS	/* remove the FE exceptions */
		or    FPSCR_Temp2, FPSCR_Temp2,ENV			/* update FPSCR */
		stw   FPSCR_Temp2, 4(FPSCR_MemPtr)
		lfd   FPSCR,       0(FPSCR_MemPtr)
		mtfsf fieldmask,   FPSCR					/* load into FPSCR */

		mffs  FPSCR									/* reload FPSCR in2 GPR */
		stfd  FPSCR,     0(FPSCR_MemPtr)
		lwz   FPSCR_Temp3,  4(FPSCR_MemPtr)			/* store whats important */

		/* we need to raise exceptions in stored in FPSCR_Temp1 */
	}
#endif
}

#elif __SPE__

/*
FECLEAREXCEPT
DESCRIPTION: The feclearexcept function clears the supported
floating-point exceptions represented by it's argument.
*/
_EWL_IMP_EXP_C void feclearexcept ( int_t excepts )
{
	register uint32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	asm
	{
		mfspr r,spefscr
		andc  r,r,EXCEPTIONS
		mtspr spefscr, r
	}
}

/*
FEGETEXCEPTFLAG
DESCRIPTION: The fegetexceptflag function stores an implementation-defined representation of the states
of the floating-point status flags indicated by the argument excepts in the object pointed to by the
argument flagp.
*/
_EWL_IMP_EXP_C void fegetexceptflag (fexcept_t *flagp, int_t excepts)
{
	register uint32_t 		r;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	asm
	{
		mfspr 	r,spefscr
		rlwinm	r,r,0,10,14 	/* spefscr & FE_ALL_EXCEPT */
		and		r,r,EXCEPTIONS
		stw		r,0(FLAG_Ptr)
	}
}

/*
FERAISEEXCEPT
DESCRIPTION: The feraiseexcept function raises the supported floating-point exceptions represented by
the argument.  The order in which these floating-point exceptions are raised is unspecified, except
as stated in ISO/IEC 9899:1999 F.7.6.  Whether the feraiseexcept function additionally raises the
"inexact" floating-point exception whenever it raise the "overflow" or "underflow" floating-point
exception is implementation-defined.
*/
_EWL_IMP_EXP_C void feraiseexcept (int_t excepts )
{
	register uint32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	asm
	{
		mfspr	r,spefscr
		or		r,r,EXCEPTIONS
		mtspr 	spefscr, r
	}

			/* we need to raise exceptions in SPEFSCR */

}

/*
FESETEXCEPTFLAG
DESCRIPTION: The fesetexceptflag function sets the floating-point
status flags indicated by the argument excepts to the states stored
in the object pointed to by flagp.  The value of *flagp
shall have been set by a previous call to fegetexceptflag whose
2nd argument represented at least those floating-point exceptions
represented by the argument excepts.  This function does not raise
floating-point exceptions, but only set the state of the flags.
*/
_EWL_IMP_EXP_C void fesetexceptflag (const fexcept_t *flagp, int_t excepts)
{
	asm
	{
		mfspr    r9,spefscr
		lwz      r11,0(r3)
		rlwinm   r0,r4,0,10,14
		andc     r0,r9,r0
		and      r4,r4,r11
		or       r0,r0,r4
		mtspr    spefscr,r0
	}
}

/*
FETESTEXCEPT
DESCRIPTION: The fetestexcept function determines which of a specified subset of the floating-point
exception flags are currently set.  The excepts argument specifies the floating-point status flags
to be queried.
*/
_EWL_IMP_EXP_C int_t fetestexcept(int_t excepts)
{
	register uint32_t 		r;
	register fenv_t			EXCEPTIONS = (uint32_t)excepts & FE_ALL_EXCEPT;
	asm
	{
		mfspr r,spefscr
		and   r,r,EXCEPTIONS
	}
	return r;				/* return result of test */
}

/*
FEGETROUND
DESCRIPTION: The fegetround function returns the value of the rounding direction macro representing
the current rounding direction or a negative number if there is no such rounding macro or the current
rounding direction is not determinable.
*/
_EWL_IMP_EXP_C int_t fegetround (void)
{
	register uint32_t r;
	asm {.nonvolatile; mfspr r,spefscr;}
	return (r & 3);			/* return rounding mode */
}

/*
FESETROUND
DESCRIPTION: fesetround function establishes the rounding direction represented by its argument round.
If the argument is not not equal to the value of a rounding direction macro, the rounding direction is
not changed.
*/
_EWL_IMP_EXP_C int_t fesetround (int_t excepts )
{
	register uint32_t r;

	if (excepts <= 0x4) 								/* verify argument is a rounding mode */
	{
		asm
		{
			mfspr	r,spefscr
			rlwinm	r,r,0,0,29
			rlwimi	r,r3,0,30,31
			mtspr	spefscr,r
		}
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}
	else {												/* argument is not a rounding mode! return any # */
		MISRA_EXCEPTION_RULE_14_7()
		return -1;
	}
}

/*
FEGETENV
DESCRIPTION: The fegetenv function stores the current
floating-point environment in the object pointed to by flagp.
*/
_EWL_IMP_EXP_C void fegetenv (fexcept_t *flagp)
{
	register uint32_t 		r;
	register fexcept_t 		*FLAG_Ptr = flagp;
	register fenv_t			EXCEPTIONS = FE_ALL_EXCEPT;
	asm
	{
		mfspr 	r,spefscr
		and		r,r,EXCEPTIONS
		stw		r,0(FLAG_Ptr)
	}
}

/*
FEHOLDEXCEPT
DESCRIPTION: The feholdexcept function saves the current
floating-point environment in the object pointed to by envp,
clears the floating-point status flags, and then installs a
non-stop (continues on floating-point exceptions) mode,
if available, for all floating-point exceptions.
*/
_EWL_IMP_EXP_C int_t feholdexcept (fenv_t *envp)
{
	register uint32_t 		r;
	register const fenv_t   *ENV_Ptr = envp;
	register fenv_t			EXCEPTIONS = FE_ALL_EXCEPT;
	register int_t 	   		mask = 0x3; 			/* clear everything except rounding mode */

	asm
	{
		mfspr	r,spefscr
		and		r,r,EXCEPTIONS
		stw		r,0(ENV_Ptr)
		and		r,r,mask
		mtspr	spefscr,r
	}
	return -1;				/* "non-stop" was not installed */
}

/*
FESETENV
DESCRIPTION: The fesetenv function establishes the floating-point
environment represented by the oject pointed to by envp.  The argument
envp shall point to an object set by a call to fegetenv or feholdenv,
or equal a floating-point macro.  Note that fesetenv merely installs
the state of the floating-point status flags represented through its
argument, and does not raise these floating-point exceptions.
*/
_EWL_IMP_EXP_C void fesetenv (const fenv_t *envp)
{
	register uint32_t 		r;
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

	asm
	{
		mfspr	r,spefscr
		and		r,r,EXCEPTIONS
		or		r,r,ENV
		mtspr	spefscr, r
	}
}

/*
FEUPDATEENV
DESCRIPTION: The feupdateenv function saves the currently raised
floating-point exceptions in its automatic storage, installs the
floating-point environment represented by the object pointed to
by envp, and then raises the saved floating-point exceptions.
The argument envp shall point to an object set by a call to feholdexcept
or fegetenv, or equal a floating-point macro.
*/

_EWL_IMP_EXP_C void feupdateenv (const fenv_t *envp)
{
	register uint32_t 		r;
	register fexcept_t 		save;
	register fenv_t    		ENV = *envp & FE_ALL_EXCEPT;
	register fenv_t    		EXCEPTIONS 	= 	~FE_ALL_EXCEPT;

	asm
	{
		mfspr	save,spefscr
		mr		r,save
		and		r,r,EXCEPTIONS
		or		r,r,ENV
		mtspr	spefscr, r
	}

	/* we need to raise exceptions in stored "save" */
	feraiseexcept(save);
}

#endif /* _No_Floating_Point_Regs */
#endif /* _EWL_C99 */
