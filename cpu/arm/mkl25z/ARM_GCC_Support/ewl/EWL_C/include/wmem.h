/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:34 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WMEM_H
#define _EWL_WMEM_H

#include <ansi_parms.h>
#include <mem_funcs.h>
#include <cstddef>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL (wmemcpy)(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL (wmemmove)(wchar_t *, const wchar_t *, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL (wmemset)(wchar_t *, wchar_t, size_t) _EWL_CANT_THROW;
#if !defined(__cplusplus)
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL (wmemchr)(const wchar_t *, wchar_t, size_t) _EWL_CANT_THROW;
#else
	_EWL_IMP_EXP_C const wchar_t * _EWL_CDECL (wmemchr)(const wchar_t *, wchar_t, size_t) _EWL_CANT_THROW;
#endif
	_EWL_IMP_EXP_C 	   int   _EWL_CDECL (wmemcmp)(const wchar_t *, const wchar_t *, size_t) _EWL_CANT_THROW;

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	_EWL_IMP_EXP_C errno_t _EWL_CDECL (wmemcpy_s)(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, rsize_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C errno_t _EWL_CDECL (wmemmove_s)(wchar_t *, rsize_t, const wchar_t *, rsize_t) _EWL_CANT_THROW;
#endif /* __STDC_WANT_LIB_EXT1__ */

_EWL_END_EXTERN_C

	#if defined(__cplusplus)

		inline
		wchar_t* _EWL_CDECL
		wmemchr(wchar_t* s, wchar_t c, size_t n) _EWL_CANT_THROW
		{
			return const_cast<wchar_t*>(wmemchr(static_cast<const wchar_t*>(s), c, n));
		}

	#endif

_EWL_END_NAMESPACE_STD

#endif /* _EWL_WMEM_H */
