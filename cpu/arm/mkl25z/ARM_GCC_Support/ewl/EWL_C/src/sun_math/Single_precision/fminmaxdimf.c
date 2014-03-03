/* $Id: fminmaxdimf.c,v 1.2 2010/05/12 07:07:20 anczane1 Exp $ */

#include <ansi_parms.h>

#if _GCCPORT_
	#include <fminmaxdim_gcc.c>
#else

#include <fdlibm.h>
#if _EWL_FLOATING_POINT
#if _EWL_C99 || _EWL_C99_MATH_LEAK

_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fmaxf ( f32_t x, f32_t y ) _EWL_CANT_THROW
 {
#if !_EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
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
#else
	if(isnan(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else if (isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
	else {
		if(x >= y) {
			MISRA_EXCEPTION_RULE_14_7()
			return x;
		}
		else {
			MISRA_EXCEPTION_RULE_14_7()
			return y;
		}
	}
#endif
 }
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fminf ( f32_t x, f32_t y ) _EWL_CANT_THROW
 {
#if !_EWL_INCOMPLETE_IEEE_EXCEPTION_MODEL
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
#else
	if(isnan(x)) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else if (isnan(y)) {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}

	if(x >= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return y;
	}
	else {
		MISRA_EXCEPTION_RULE_14_7()
		return x;
	}
#endif
 }
_EWL_IMP_EXP_C f32_t _EWL_MATH_CDECL fdimf ( f32_t x, f32_t y ) _EWL_CANT_THROW
{
	if(x > y) {
		MISRA_EXCEPTION_RULE_14_7()
		return (x-y);
	}
	else if(x <= y) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0.0F;
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
