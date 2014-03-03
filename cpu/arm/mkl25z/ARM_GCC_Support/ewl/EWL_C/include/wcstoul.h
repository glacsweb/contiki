/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:55:32 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCSTOUL_H
#define _EWL_WCSTOUL_H

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

#ifdef __CWCC__
	#if __CWCC__ >= 0x3200
		#pragma implements_std_header("wchar.h")
	#endif /* __CWCC__ >= 0x3200 */
#endif /* __CWCC__ */

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C  int _EWL_CDECL		watoi(const wchar_t * ) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C long _EWL_CDECL		watol(const wchar_t * ) _EWL_CANT_THROW;

    extern unsigned long _EWL_CDECL		__wcstoul(	int	base,
										int		max_width,
										__std(wint_t) (*ReadProc)(void *, __std(wint_t), int),
										void * ReadProcArg,
										int	* chars_scanned,
										int	* negative,
										int	* overflow) _EWL_CANT_THROW;

#if _EWL_LONGLONG
	extern unsigned long long _EWL_CDECL __wcstoull(	int	base,
										 int		max_width,
										 __std(wint_t) (*ReadProc)(void *, __std(wint_t), int),
										 void * ReadProcArg,
										 int	* chars_scanned,
										 int	* negative,
										 int	* overflow) _EWL_CANT_THROW;
#endif /*_EWL_LONGLONG*/

_EWL_END_EXTERN_C

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C unsigned long _EWL_CDECL				wcstoul(const wchar_t * _EWL_RESTRICT, wchar_t ** _EWL_RESTRICT, int) _EWL_CANT_THROW;
	#if _EWL_C99
	#if _EWL_LONGLONG
		_EWL_IMP_EXP_C unsigned long long _EWL_CDECL	wcstoull(const wchar_t * _EWL_RESTRICT, wchar_t ** _EWL_RESTRICT, int) _EWL_CANT_THROW;
		_EWL_IMP_EXP_C long long _EWL_CDECL 			wcstoll(const wchar_t * _EWL_RESTRICT, wchar_t ** _EWL_RESTRICT, int) _EWL_CANT_THROW;
	#endif /* _EWL_LONGLONG */
	#endif /* _EWL_C99 */
	_EWL_IMP_EXP_C long _EWL_CDECL						wcstol(const wchar_t * _EWL_RESTRICT, wchar_t ** _EWL_RESTRICT, int) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_WIDE_CHAR  */
#endif /* _EWL_WCSTOUL_H */
