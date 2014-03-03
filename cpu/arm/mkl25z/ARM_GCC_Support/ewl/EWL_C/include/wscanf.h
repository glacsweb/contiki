/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:34 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WSCANF_H
#define _EWL_WSCANF_H

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

	_EWL_IMP_EXP_C int _EWL_CDECL fwscanf(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL wscanf(const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL swscanf(const wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vswscanf(const wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vfwscanf(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vwscanf(const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	_EWL_IMP_EXP_C int _EWL_CDECL fwscanf_s(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL wscanf_s(const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL swscanf_s(const wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, ...) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vswscanf_s(const wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vfwscanf_s(FILE * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL vwscanf_s(const wchar_t * _EWL_RESTRICT, va_list) _EWL_CANT_THROW;
#endif /* __STDC_WANT_LIB_EXT1__ */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD


#endif /* _EWL_WSCANF_H */
