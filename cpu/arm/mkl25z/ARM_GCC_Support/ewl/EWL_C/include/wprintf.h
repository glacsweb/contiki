/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:34 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WPRINTF_H
#define _EWL_WPRINTF_H

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

	_EWL_IMP_EXP_C int _EWL_CDECL fwprintf(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL wprintf(const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL swprintf(wchar_t * _EWL_RESTRICT, size_t, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vfwprintf(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vswprintf(wchar_t * _EWL_RESTRICT, size_t, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vwprintf(const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	_EWL_IMP_EXP_C int _EWL_CDECL fwprintf_s(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL wprintf_s(const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL snwprintf_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL swprintf_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vfwprintf_s(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vsnwprintf_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vswprintf_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vwprintf_s(const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
#endif /* __STDC_WANT_LIB_EXT1__ */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_WPRINTF_H */

