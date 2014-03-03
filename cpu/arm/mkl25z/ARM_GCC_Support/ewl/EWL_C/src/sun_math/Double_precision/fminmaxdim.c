/* $Id: fminmaxdim.c,v 1.5 2010/05/12 07:07:15 anczane1 Exp $ */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <fminmaxdim_gcc.c>
#else

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL fmax ( f64_t x, f64_t y ) _EWL_CANT_THROW
 {
	if(x >= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	else if(x < y) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else
	{ /* since x is neither ge or l y they are unordered */
		if(isnan(x)) {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
 }
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL fmin ( f64_t x, f64_t y ) _EWL_CANT_THROW
 {
	if(x >= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else if(x < y) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	else
	{ /* since x is neither ge or l y they are unordered */
		if(isnan(x)) {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
	}
 }
_EWL_IMP_EXP_C f64_t _EWL_MATH_CDECL fdim ( f64_t x, f64_t y ) _EWL_CANT_THROW
{
	/* range error may occur - not specified */
	if(x > y) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x-y);
	}
	else if(x <= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0.0;
	}
	else
	{
		if(isnan(x)) {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
	}
}
#endif /* _EWL_C99 */
#endif /* _EWL_FLOATING_POINT */

#endif
