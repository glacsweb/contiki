/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:34 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WSTRING_H
#define _EWL_WSTRING_H

#include <ansi_parms.h>
#include <size_t.h>
#include <wchar_t.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C size_t _EWL_CDECL		(wcslen)(const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL 	(wcscpy)(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL 	wcsncpy(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL		wcscat(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL		wcsncat(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL 			wcscmp(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL			wcsncmp(const wchar_t *, const wchar_t *, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int _EWL_CDECL 			wcscoll(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C size_t _EWL_CDECL 		wcsxfrm(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C size_t _EWL_CDECL		wcsspn(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C size_t _EWL_CDECL		wcscspn(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL		wcstok(wchar_t * _EWL_RESTRICT, const wchar_t * _EWL_RESTRICT, wchar_t ** _EWL_RESTRICT) _EWL_CANT_THROW;
#if !defined(__cplusplus)
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL		wcschr(const wchar_t *, wchar_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL 	wcspbrk(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL 	wcsrchr(const wchar_t *, wchar_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C wchar_t * _EWL_CDECL 	wcsstr(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
#else
	_EWL_IMP_EXP_C const wchar_t * _EWL_CDECL wcschr(const wchar_t *, wchar_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C const wchar_t * _EWL_CDECL wcspbrk(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C const wchar_t * _EWL_CDECL wcsrchr(const wchar_t *, wchar_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C const wchar_t * _EWL_CDECL wcsstr(const wchar_t *, const wchar_t *) _EWL_CANT_THROW;
#endif

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	_EWL_IMP_EXP_C size_t _EWL_CDECL		(wcsnlen_s)(const wchar_t *, size_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C errno_t _EWL_CDECL		(wcscpy_s)(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C errno_t _EWL_CDECL		wcsncpy_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, rsize_t) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C errno_t _EWL_CDECL		wcscat_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C errno_t _EWL_CDECL		wcsncat_s(wchar_t * _EWL_RESTRICT, rsize_t, const wchar_t * _EWL_RESTRICT, rsize_t) _EWL_CANT_THROW;
#endif /* __STDC_WANT_LIB_EXT1__ */

_EWL_END_EXTERN_C

	#if defined(__cplusplus)

		inline
		wchar_t* _EWL_CDECL
		wcschr(wchar_t* s, wchar_t c) _EWL_CANT_THROW
		{
			return const_cast<wchar_t*>(wcschr(static_cast<const wchar_t*>(s), c));
		}

		inline
		wchar_t* _EWL_CDECL
		wcspbrk(wchar_t* s1, const wchar_t* s2) _EWL_CANT_THROW
		{
			return const_cast<wchar_t*>(wcspbrk(static_cast<const wchar_t*>(s1), s2));
		}

		inline
		wchar_t* _EWL_CDECL
		wcsrchr(wchar_t* s, wchar_t c) _EWL_CANT_THROW
		{
			return const_cast<wchar_t*>(wcsrchr(static_cast<const wchar_t*>(s), c));
		}

		inline
		wchar_t* _EWL_CDECL
		wcsstr(wchar_t* s1, const wchar_t* s2) _EWL_CANT_THROW
		{
			return const_cast<wchar_t*>(wcsstr(static_cast<const wchar_t*>(s1), s2));
		}

	#endif

_EWL_END_NAMESPACE_STD

#endif /* _EWL_WSTRING_H */
