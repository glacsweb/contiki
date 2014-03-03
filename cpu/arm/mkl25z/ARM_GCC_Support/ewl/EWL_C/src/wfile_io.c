/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/21 12:33:24 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		_wremove
 *		_wrename
 *		_wtmpnam
 *		_wtmpnam_s
 *		_wfopen
 *		_wfopen_s
 *		_wfreopen
 *		_wfreopen_s
 *
 *		__whandle_reopen
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR
#if _EWL_WFILEIO_AVAILABLE

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wctype.h>
#include <wstring.h>
#include <ansi_files.h>
#include <buffer_io.h>
#include <file_io.h>
#include <misc_io.h>
#include <critical_regions.h>
#include <ewl_thread_local_data.h>

#if (__dest_os == __win32_os)
	#include <crtl.h>
#endif

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

int_t _EWL_CDECL _wremove(const wchar_t * name)
{
	return((__wdelete_file(name) == __no_io_error) ? 0 : -1);
}


int_t _EWL_CDECL _wrename(const wchar_t * old_name, const wchar_t * new_name)
{
	return((__wrename_file(old_name, new_name) == __no_io_error) ? 0 : -1);
}


wchar_t * _EWL_CDECL _wtmpnam(wchar_t * name)
{

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static wchar_t wtemp_name[L_tmpnam];
#endif

	if (name)
	{
		__wtemp_file_name(name, NULL);
		return(name);
	}
	else
	{

		__wtemp_file_name(_EWL_LOCALDATA(wtemp_name), NULL);
		return(_EWL_LOCALDATA(wtemp_name));
	}
}


errno_t _EWL_CDECL _wtmpnam_s(wchar_t *name, rsize_t maxsize)
{
	wchar_t wtemp_name[L_tmpnam];

	if ((name == NULL) || (maxsize < 1) || (maxsize > RSIZE_MAX))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__wtemp_file_name(wtemp_name, NULL);

	if (wcsnlen_s(wtemp_name, L_tmpnam) + 1 > maxsize)
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		name[0] = 0;
		return ERANGE;
	}

	wcscpy(name, wtemp_name);
	return ENOERR;
}


FILE * _EWL_CDECL _wfopen(const wchar_t * _EWL_RESTRICT name, const wchar_t * _EWL_RESTRICT mode)
{
	FILE * file;

	__begin_critical_region(files_access);

	file = __find_unopened_file();

	__end_critical_region(files_access);

	return(_wfreopen(name, mode, file));
}


errno_t _EWL_CDECL _wfopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT file, const wchar_t * _EWL_RESTRICT name, const wchar_t * _EWL_RESTRICT mode)
{
	FILE *newfile;

	if (file != NULL)
		*file = NULL;

	if ((file == NULL) || (name == NULL) || (mode == NULL))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__begin_critical_region(files_access);

	newfile = __find_unopened_file();

	__end_critical_region(files_access);

	return(_wfreopen_s(file, name, mode, newfile));
}


FILE * _EWL_CDECL _wfreopen(const wchar_t * _EWL_RESTRICT name, const wchar_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	__stdio_atexit();

	if (!file)
		return(NULL);

	fclose(file);
	clearerr(file);

	if (!__wget_file_modes(mode, &modes))
		return(NULL);

	__init_file(file, modes, 0, BUFSIZ);

	if (__wopen_file(name, modes, &file->handle))
	{
		file->mode.file_kind = __closed_file;
	    if (file->state.free_buffer)
	    	free(file->buffer);
		return(NULL);
	}
	if (modes.io_mode & __append_mode)
		fseek(file, 0, SEEK_END);

	return(file);
}


errno_t _EWL_CDECL _wfreopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT newfile, const wchar_t * _EWL_RESTRICT name, const wchar_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	if (newfile != NULL)
		*newfile = NULL;

	if ((newfile == NULL) || (mode == NULL) || (file == NULL))
	{
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__stdio_atexit();

	if (name == NULL)
	{
		if (!__wget_file_modes(mode, &modes))
			return ERANGE;

		file->mode.binary_io = modes.binary_io;
#if (__dest_os	== __win32_os)
		_HandleTable[file->handle]->translate = !(modes.binary_io);
#endif

		*newfile = file;
		return ENOERR;
	}

	fclose(file);
	clearerr(file);

	if (!__wget_file_modes(mode, &modes))
		return ERANGE;

	__init_file(file, modes, 0, BUFSIZ);

	if (__wopen_file(name, modes, &file->handle))
	{
		file->mode.file_kind = __closed_file;
		if (file->state.free_buffer)
			free(file->buffer);
		return ERANGE;
	}
	if (modes.io_mode & __append_mode)
		fseek(file, 0, SEEK_END);

	*newfile = file;
	return ENOERR;
}


FILE * _EWL_CDECL __whandle_open(__file_handle handle, const wchar_t * mode)
{
	FILE * file;

	__begin_critical_region(files_access);

	file = __whandle_reopen(handle, mode, __find_unopened_file());

	__end_critical_region(files_access);

	return(file);
}


FILE * _EWL_CDECL __whandle_reopen(__file_handle handle, const wchar_t * mode, FILE * file)
{
	__file_modes	modes;

	__stdio_atexit();

	if (!file)
		return(NULL);

	fclose(file);
	clearerr(file);

	if (!__wget_file_modes(mode, &modes))
		return(NULL);

	__init_file(file, modes, 0, BUFSIZ);

	file->handle = handle;

	return(file);
}


int _EWL_CDECL __wget_file_modes(const wchar_t * mode, __file_modes * modes)
{
	const wchar_t *	mode_ptr = mode;
	unsigned long	mode_str;
	unsigned char	open_mode, io_mode;

	modes->file_kind = __disk_file;
	modes->file_orientation = __unoriented;
	modes->binary_io = 0;

	mode_str = *mode_ptr++;

	switch (mode_str)
	{
		case 'r':
			open_mode = __must_exist;
			break;

		case 'w':
			open_mode = __create_or_truncate;
			break;

		case 'a':
			open_mode = __create_if_necessary;
			break;

		default:
			return(0);
	}


	modes->open_mode = open_mode;

	switch (*mode_ptr++)
	{
		case L'b':
			modes->binary_io = 1;

			if (*mode_ptr == L'+')
				mode_str = (mode_str << 8) | '+';

			break;

		case L'+':
			mode_str = (mode_str << 8) | '+';

			if (*mode_ptr == L'b')
				modes->binary_io = 1;

			break;
	}

	switch (mode_str)
	{
		case 'r':
			io_mode = __read_mode;
			break;

		case 'w':
			io_mode = __write_mode;
			break;

		case 'a':
			io_mode = __write_mode | __append_mode;
			break;

		case 'r+':
			io_mode = __read_write_mode;
			break;

		case 'w+':
			io_mode = __read_write_mode;
			break;

		case 'a+':
			io_mode = __read_write_mode | __append_mode;
			break;
	}

	modes->io_mode = io_mode;

	return(1);
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#endif	/* _EWL_WFILEIO_AVAILABLE */
#endif	/* _EWL_WIDE_CHAR */
