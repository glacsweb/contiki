/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/26 14:05:56 $
 * $Revision: 1.9 $
 */

#ifndef _EWL_STDIO_API_H
#define _EWL_STDIO_API_H

#include <ansi_parms.h>

#include <size_t.h>

#if _EWL_WIDE_CHAR
	#include <wchar_t.h>
	#include <file_struc.h>
#endif /* _EWL_WIDE_CHAR */

#include <null.h>
#include <eof.h>
#include <va_list.h>
#include <file_struc.h>

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

#pragma options align=native

_EWL_IMP_EXP_C size_t _EWL_CDECL	__fread(void *, size_t, size_t, FILE *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C size_t _EWL_CDECL	__fwrite(const void *, size_t, size_t, FILE *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C fpos_t _EWL_CDECL   	_ftell(FILE * _EWL_RESTRICT) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL     	_fseek(FILE *, fpos_t, int) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL		__get_char(FILE *) _EWL_CANT_THROW;
_EWL_IMP_EXP_C int _EWL_CDECL		__put_char(int, FILE *) _EWL_CANT_THROW;

#if _EWL_WIDE_CHAR
_EWL_IMP_EXP_C int _EWL_CDECL		__fwide(FILE *, int) _EWL_CANT_THROW;
#endif

void * _EWL_CDECL __FileWrite(void *, const char *, size_t) _EWL_CANT_THROW;
int    _EWL_CDECL __FileRead(void *, int, int) _EWL_CANT_THROW;

#if !_EWL_CONSOLE_FILE_IS_DISK_FILE && _EWL_CONSOLE_SUPPORT
int    _EWL_CDECL __ConsoleRead(void *, int, int) _EWL_CANT_THROW;
void * _EWL_CDECL __ConsoleWrite(void *, const char *, size_t) _EWL_CANT_THROW;
#endif /* !_EWL_CONSOLE_FILE_IS_DISK_FILE && _EWL_CONSOLE_SUPPORT */

int _EWL_CDECL __ungetc(int, FILE *) _EWL_CANT_THROW;

#if _EWL_WIDE_CHAR
	wchar_t _EWL_CDECL __getwc(FILE *) _EWL_CANT_THROW;
	wchar_t _EWL_CDECL __fgetwc(FILE *) _EWL_CANT_THROW;
	wchar_t _EWL_CDECL __getwchar(void) _EWL_CANT_THROW;
	wchar_t _EWL_CDECL __ungetwc(wchar_t, FILE *) _EWL_CANT_THROW;
	void * 	_EWL_CDECL __wFileWrite(void *, const wchar_t *, size_t) _EWL_CANT_THROW;

#if _EWL_C99
	wint_t 	__wFileRead(void *, wint_t, int) _EWL_CANT_THROW;
#endif /* _EWL_C99 */

#endif /* _EWL_WIDE_CHAR */

_MISRA_EXCEPTION_STDIO_MACROS()

#if _EWL_WIDE_CHAR
	#define __getc(_file) \
		((__fwide((FILE *)_file, -1) >= 0) ? EOF : ((_file)->buffer_len-- ? (int)  *(_file)->buffer_ptr++ : __std(__get_char)((FILE *)_file)))
	#define __putc(_c, _file) \
		((__fwide((FILE *)_file, -1) >= 0) ? EOF : ((_file)->buffer_len-- ? (int) (*(_file)->buffer_ptr++ = (unsigned char)(_c)) : __std(__put_char)(_c, (FILE *)_file)))
#else
	#define __getc(_file)  \
		((_file)->buffer_len-- ? (int)  *(_file)->buffer_ptr++ : __std(__get_char)((FILE *)_file))
	#define __putc(_c, _file) \
		((_file)->buffer_len-- ? (int) (*(_file)->buffer_ptr++ = (unsigned char)(_c)) : __std(__put_char)(_c, (FILE *)_file))
#endif /* _EWL_WIDE_CHAR && _EWL_C99 */

#if _EWL_OS_DISK_FILE_SUPPORT
	_EWL_IMP_EXP_C FILE * _EWL_CDECL __handle_open(__file_handle handle, const char * mode) _EWL_CANT_THROW;
	FILE * _EWL_CDECL __handle_reopen(__file_handle handle, const char * mode, FILE *) _EWL_CANT_THROW;
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#if __dest_os == __starcore
void   _EWL_CDECL __set_ref_con(FILE *, __ref_con ref_con) _EWL_CANT_THROW;
void   _EWL_CDECL __set_idle_proc(FILE *, __idle_proc idle_proc) _EWL_CANT_THROW;
#endif

#if _EWL_WFILEIO_AVAILABLE
	_EWL_IMP_EXP_C FILE * _EWL_CDECL __whandle_open(__file_handle handle, const wchar_t * mode) _EWL_CANT_THROW;
	FILE * _EWL_CDECL __whandle_reopen(__file_handle handle, const wchar_t * mode, FILE *) _EWL_CANT_THROW;
#endif /* _EWL_WFILEIO_AVAILABLE */

typedef struct
{
	char * CharStr;
	__std(size_t) MaxCharCount;
	__std(size_t) CharsWritten;
} __OutStrCtrl;

typedef struct
{
	const char * NextChar;
	int    NullCharDetected;
} __InStrCtrl;

#if _EWL_WIDE_CHAR
_EWL_IMP_EXP_C __std(size_t) _EWL_CDECL	__fwrite(const void *, __std(size_t), __std(size_t), FILE *) _EWL_CANT_THROW;
#endif

void * _EWL_CDECL __StringWrite(void *, const char *, size_t) _EWL_CANT_THROW;
int    _EWL_CDECL __StringRead(void *, int, int) _EWL_CANT_THROW;

enum __ReadProcActions
{
	__GetAChar,
	__UngetAChar,
	__TestForError
};

#if _EWL_WIDE_CHAR
typedef struct
{
	wchar_t * wCharStr;
	size_t MaxCharCount;
	size_t CharsWritten;
} __wOutStrCtrl;

typedef struct
{
	wchar_t * wNextChar;
	int    wNullCharDetected;
} __wInStrCtrl;

enum __WReadProcActions
{
	__GetAwChar,
	__UngetAwChar,
	__TestForwcsError
};

void * _EWL_CDECL __wStringWrite(void *, const wchar_t *, size_t) _EWL_CANT_THROW;
wint_t __wStringRead(void *, wint_t, int) _EWL_CANT_THROW;

#endif /* _EWL_WIDE_CHAR */

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#pragma options align=reset

#endif /* _EWL_STDIO_API_H */
