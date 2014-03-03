/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:27 $
 * $Revision: 1.3 $
 */

#ifndef _EWL_WSTDIO_H
#define _EWL_WSTDIO_H

#include <ansi_parms.h>
#include <file_struc.h>
#include <va_list.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL putwc(wchar_t, FILE *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL fputwc(wchar_t, FILE *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL getwc(FILE *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL fgetwc(FILE *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int      _EWL_CDECL fputws(const wchar_t * _EWL_RESTRICT, FILE * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t *_EWL_CDECL fgetws(wchar_t * _EWL_RESTRICT, int, FILE * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL putwchar(wchar_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int      _EWL_CDECL fwide(FILE *, int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t  _EWL_CDECL getwchar(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#if _GCCPORT_
	__attribute__((weak)) wchar_t _EWL_CANT_THROW _EWL_CDECL getwchar(void) /* Moved _EWL_CANT_THROW attribute */
		{ return getwc(stdin); }
#else
	_EWL_INLINE wchar_t _EWL_CDECL getwchar(void) _EWL_CANT_THROW
		{ return getwc(stdin); }
#endif

_EWL_END_NAMESPACE_STD

#endif /* _EWL_WSTDIO_H */
