/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/01/29 13:41:59 $
 * $Revision: 1.2 $
 */

/*	Define inline functions as extern-able functions */

#include <ansi_parms.h>
#include <x86/crtl.h>

#undef	_MSL_USE_INLINE
#define	_MSL_USE_INLINE 1
#undef	_MSL_INLINE
#define	_MSL_INLINE _MSL_DO_NOT_INLINE
#define _MSL_FENV_IMPL 1

#undef	_MSL_MATH_CDECL
#define	_MSL_MATH_CDECL __cdecl

#pragma k63d off
#pragma sse off
#pragma sse2 off

#pragma cplusplus on
static int procflags = __detect_cpu_instruction_set();
#pragma cplusplus reset

#include <fenv.h>

fenv_t  _FE_DFL_ENV =  0x0000037f;

/* Change record:
 * EJS 020124 Created
 * JWW 020308 Use new _MSL_DO_NOT_INLINE macro for bringing inlined functions into the library
 * EJS 031217 Add SSE support for exception and precision control
 */