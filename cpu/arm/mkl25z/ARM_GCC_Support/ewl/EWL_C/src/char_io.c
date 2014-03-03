/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/12 07:07:09 $
 * $Revision: 1.8 $
 */

/*
 *	Routines
 *	--------
 *		fgetc
 *		getc
 *		getchar
 *
 *		fgets
 *		gets
 *		gets_s
 *
 *		ungetc
 *
 *		fputc
 *		putc
 *		putchar
 *
 *		fputs
 *		puts
 *
 *		__get_char
 *		__put_char
 *
 *		__ungotten
 */

#include <ansi_parms.h>

#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <ansi_files.h>
#include <buffer_io.h>
#include <char_io.h>
#include <misc_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if !defined(_Old_DSP_IO_Interface)
int_t _EWL_CDECL __get_char(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__io_states	state;
	__io_results ioresult;

	file->buffer_len = 0U;

	if (file->state.error || ((__file_kinds)file->mode.file_kind == __closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	state = (__io_states)file->state.io_state;

	if ((state == __writing) || !(file->mode.io_mode & (uchar_t)__read_mode)) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (state >= __rereading) {
		file->state.io_state--;

		if (state == __rereading) {
			file->buffer_len = file->saved_buffer_len;
		}

		MISRA_EXCEPTION_RULE_14_7()
		return (int_t)(file->ungetc_buffer[(int_t)state - (int_t)__rereading]);

	} else {
		file->state.io_state = (uchar_t)__reading;
	}

	ioresult = (__io_results)__load_buffer(_file, NULL, __align_buffer);

	if (ioresult || !file->buffer_len) {
		if (ioresult == __io_error) {
			set_error(file);
		} else {
			set_eof(file);
		}

		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	file->buffer_len--;

	return (int_t)(*file->buffer_ptr++);
}
#endif /* __get_char conditionals */

int_t _EWL_CDECL fgetc(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdin) {
		crtrgn = stdin_access;
	} else if (_file == stdout) {
		crtrgn = stdout_access;
	} else if (_file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	retval = __getc(file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (getc)(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdin) {
		crtrgn = stdin_access;
	} else if (_file == stdout) {
		crtrgn = stdout_access;
	} else if (_file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	retval = __getc(file);
  	__end_critical_region(crtrgn);

  	return(retval);
}

#if !defined(_Old_DSP_IO_Interface)
MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (getchar)(void)
{
	int_t retval;

  	__begin_critical_region(stdin_access);
	retval = getchar();
  	__end_critical_region(stdin_access);
  	return(retval);
}
#endif /* getchar conditionals */

char_t * _EWL_CDECL fgets(char_t * _EWL_RESTRICT s, int_t n, FILE * _EWL_RESTRICT _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	char_t *	p = s;
	int_t			c;

	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdin) {
		crtrgn = stdin_access;
	} else if (_file == stdout) {
		crtrgn = stdout_access;
	} else if (_file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}

	if (--n < 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

  	__begin_critical_region(crtrgn);

	if (n) {
		MISRA_EXCEPTION_RULE_12_4()
		do {
			c = __getc(file);
			if (c == (int_t)EOF) {
				if (file->state.eof && (p != s)) {
					break;
				} else {
  					__end_critical_region(crtrgn);
					MISRA_EXCEPTION_RULE_14_7()
					return(NULL);
				}
			}
			*p++ = (char_t)c;
		} while ((c != '\n') && --n);
	}

  	__end_critical_region(crtrgn);

	*p = 0;

	return(s);
}

char_t * _EWL_CDECL gets(char_t * s)
{
	char_t *p = s;
	int_t	c;
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *	file = (struct _FILE *)stdin;
  	__begin_critical_region(stdin_access);

	MISRA_EXCEPTION_RULE_14_6()
	for (;;) {
		c = __getc(file);

		if (c == (int_t)EOF) {
			if (file->state.eof && (p != s)) {
				break;
			} else {
  				__end_critical_region(stdin_access);
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}
		} else if (c == '\n') {
			break;
		} else {}

		*p++ = (char_t)c;
	}
  	__end_critical_region(stdin_access);

	*p = 0;

	return(s);
}

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
char_t * _EWL_CDECL gets_s(char_t * s, rsize_t n)
{
	char_t *	p = s;
	int_t			c;
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *	file = (struct _FILE *)stdin;

	if ((n < 1U) || (s == NULL) || (n > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		if ((s != NULL) && (n > 0U) && (n <= (rsize_t)RSIZE_MAX)) {
			s[0] = 0;
		}
		__begin_critical_region(stdin_access);
		for (;;) {
			c = __getc((struct _FILE*)file);
			if ((c == (int_t)EOF) || (c == '\n')) {
				break;
			}
		}
		__end_critical_region(stdin_access);
		MISRA_EXCEPTION_RULE_14_7()
		return NULL;
	}

  	__begin_critical_region(stdin_access);

	MISRA_EXCEPTION_RULE_14_6()
	for (;n;n--) {
		c = __getc(file);

		if (c == (int_t)EOF) {
			if (file->state.eof && (p != s)) {
				break;
			} else {
  				__end_critical_region(stdin_access);
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}
		} else if (c == '\n') {
			break;
		} else if (n == 1U) {
			MISRA_EXCEPTION_RULE_11_4()
			(void)__ungetc(c, (FILE *)file);
			break;
		} else {}

		*p++ = (char_t)c;
	}
  	__end_critical_region(stdin_access);

	*p = 0;

	return(s);
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

int_t _EWL_CDECL ungetc(int_t c, FILE * file)
{
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	c = __ungetc(c, file);
  	__end_critical_region(crtrgn);

	return(c);
}

int_t _EWL_CDECL __ungetc(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	uchar_t state = file->state.io_state;

#if _EWL_WIDE_CHAR  && _EWL_C99
	if (fwide(_file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99*/

	if ((state == (uchar_t)__writing) || (state == (((uchar_t)__rereading + (uchar_t)__ungetc_buffer_size) - 1U)) || (c == (int_t)EOF)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (state < (uchar_t)__rereading) {
		file->saved_buffer_len = file->buffer_len;
		file->buffer_len       = 0U;
		file->state.io_state = (state = (uchar_t)__rereading);
	} else {
		state = ++file->state.io_state;
	}

	file->ungetc_buffer[state - (uchar_t)__rereading] = (uchar_t)c;
	file->state.eof = 0U;

	return c;
}

int_t _EWL_CDECL __ungotten(const FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	if (file->state.io_state < (uint_t)__rereading) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	return (int_t)(file->ungetc_buffer[(int_t)file->state.io_state - (int_t)__rereading]);
}

int_t _EWL_CDECL __put_char(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__file_kinds   kind = (__file_kinds)file->mode.file_kind;

	file->buffer_len = 0U;

	if (file->state.error || (kind == __closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_OS_DISK_FILE_SUPPORT
	if (kind == __console_file) {
		__stdio_atexit();
	}
#endif

	if (file->state.io_state == (uchar_t)__neutral) {
		if (file->mode.io_mode & (uchar_t)__write_mode) {
#if _EWL_OS_DISK_FILE_SUPPORT
			if (file->mode.io_mode & (uchar_t)__append_mode) {
				if (_fseek(file, 0u, SEEK_END)) {
					MISRA_EXCEPTION_RULE_14_7()
					return(0);
				}
			}
#endif

			file->state.io_state = (uchar_t)__writing;
			__prep_buffer(_file);
		}
	}

	if (file->state.io_state != (uchar_t)__writing) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	MISRA_EXCEPTION_RULE_17_3()
	if ((file->mode.buffer_mode == (uint_t)_IOFBF) || ((file->buffer_ptr - file->buffer) == (int_t)file->buffer_size)) {
		if (__flush_buffer(_file, NULL)) {
			set_error(file);
			MISRA_EXCEPTION_RULE_14_7()
			return(EOF);
		}
	}

	file->buffer_len--;
	*file->buffer_ptr++ = (uchar_t)c;

	if (file->mode.buffer_mode != (uint_t)_IOFBF) {
		if ((file->mode.buffer_mode == (uint_t)_IONBF) || (c == '\n')) {
			if (__flush_buffer(_file, NULL)) {
				set_error(file);
				MISRA_EXCEPTION_RULE_14_7()
				return(EOF);
			}
		}

		file->buffer_len = 0U;
	}

	return c;
}

int_t _EWL_CDECL fputc(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
 	critical_regions crtrgn;

	if (_file == stdout)
	{
		crtrgn = stdout_access;
	}
	else if (_file == stderr)
	{
		crtrgn = stderr_access;
	}
	else
	{
		crtrgn = files_access;
	}
 	__begin_critical_region(crtrgn);
	retval = __putc(c, file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (putc)(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (_file == stdout)
	{
		crtrgn = stdout_access;
	}
	else if (_file == stderr)
	{
		crtrgn = stderr_access;
	}
	else
	{
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	retval = __putc(c, file);
  	__end_critical_region(crtrgn);
  	return(retval);
}

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (putchar)(int_t c)
{
	int_t retval;
  	__begin_critical_region(stdout_access);
  	MISRA_EXCEPTION_RULE_11_4()
	retval = __putc(c,(struct _FILE *)stdout);
  	__end_critical_region(stdout_access);
  	return(retval);
}

int_t _EWL_CDECL fputs(const char_t * _EWL_RESTRICT s, FILE * _EWL_RESTRICT file)
{
	int_t	c;
	int_t retval = 0;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdout)
	{
		crtrgn = stdout_access;
	}
	else if (file == stderr)
	{
		crtrgn = stderr_access;
	}
	else
	{
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);
	while ((c = *s++) != 0)
	{
		MISRA_EXCEPTION_RULE_11_4()
		if (__putc(c, (struct _FILE *)file) == EOF)
		{
			retval = EOF;
			break;
		}
		}
  	__end_critical_region(crtrgn);
	return(retval);
}

int_t _EWL_CDECL puts(const char_t * s)
{
	int_t	c;
	int_t   retval = 0;
	FILE *	file = stdout;

  	__begin_critical_region(stdout_access);
	while ((c = *s++) != 0)
	{
		MISRA_EXCEPTION_RULE_11_4()
		if (__putc(c, (struct _FILE *)file) == EOF)
		{
			retval = EOF;
			break;
		}
	}

	if (retval != (int_t)EOF)
	{
		MISRA_EXCEPTION_RULE_11_4()
		retval = __putc('\n', (struct _FILE *)file);
	}

  	__end_critical_region(stdout_access);
	return(retval);
}
