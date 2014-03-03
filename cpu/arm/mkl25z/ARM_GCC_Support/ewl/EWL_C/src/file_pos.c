/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/07/07 07:39:08 $
 * $Revision: 1.15 $
 */

/*
 *	Routines
 *	--------
 *		ftell
 *		fseek
 *
 *		fgetpos
 *		fsetpos
 *
 *		rewind
 */

#include <ansi_parms.h>

#include <errno.h>
#include <ansi_files.h>
#include <buffer_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

MISRA_EXCEPTION_RULE_16_2()

/*
	 XXXdbg - declare _ftell() and _fseek() the way they should have been
	          in the first place and map ftell() and fseek() to use the
						underscore versions.  This is necessary for systems (like
						the BeOS) where fpos_t is really a 64 bit quantity.
*/

fpos_t _EWL_CDECL _ftell(FILE * _EWL_RESTRICT _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t charsInUndoBuffer = 0U;
	fpos_t	position;
#if (__dest_os == __starcore)
  __pos_proc pos_proc = file->position_proc;
  uint32_t offset = 0L;
#endif

	if ( !(((__file_kinds)file->mode.file_kind == __disk_file) || ((__file_kinds)file->mode.file_kind == __console_file)) || file->state.error) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = EFPOS;
		position = (fpos_t)-1;
		MISRA_EXCEPTION_RULE_14_7()
		return position;
	}

	if (file->state.io_state == (uint_t)__neutral) {
		MISRA_EXCEPTION_RULE_14_7()
		return(file->position);
	}

	MISRA_EXCEPTION_RULE_17_3()
	position = file->buffer_pos + (file->buffer_ptr - file->buffer);

	if (file->state.io_state >= (uint_t)__rereading) {
		charsInUndoBuffer = (uchar_t)((file->state.io_state - (uchar_t)__rereading) + 1U);
		position -= charsInUndoBuffer;
	}


#if	((__dest_os == __win32_os) || (__dest_os == __dolphin_os))

	if (!file->mode.binary_io) {

	/*	n is the number of characters before the current location.
		We can calculate that by buffer_ptr - buffer. However if
		there are characters in the undo buffer then we need to not
		consider those, so we subtract charsInUndoBuffe from n. */
		ptrdiff_t	n = file->buffer_ptr - file->buffer - charsInUndoBuffer;
		uchar_t *	p = file->buffer;

		while (n--) {
			if (*p++ == '\n') {
				position++;
			}
		}
	}
#elif (__dest_os == __starcore)

	if (!file->mode.binary_io && file->state.io_state == __writing) {

		/* for text files, the debugger syscall replaces \n with \n\r at write.
       they also take care of this in the lseek syscall. In order to get
       correct results, we need to flush the buffer and go through the
       syscall for ftell. */
		if (__flush_buffer(file, NULL)) {
			set_error(file);
			return(fpos_t)(-1L);
		}

		if (pos_proc) {
			if ((*pos_proc)(file->handle, &offset, SEEK_CUR, file->ref_con)) {
				set_error(file);
				MISRA_EXCEPTION_RULE_20_5()
				errno = EFPOS;
				return(fpos_t)(-1L);
			}
			else {
				position = offset;
			}
		}
	}

#endif

	return(position);

	MISRA_EXCEPTION_RULE_16_7()
}

int32_t _EWL_CDECL ftell(FILE * file)
{
	int32_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	retval = (int32_t)_ftell(file);
  	__end_critical_region(crtrgn);

	return(retval);
}

int_t _EWL_CDECL fgetpos(FILE * _EWL_RESTRICT file, fpos_t * _EWL_RESTRICT pos)
{
	int_t result;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	*pos = _ftell(file);

  	__end_critical_region(crtrgn);

	result = (*pos == (fpos_t)-1);
	return result;
}

int_t _EWL_CDECL _fseek(FILE * _file, fpos_t offset, int_t mode)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	fpos_t		position;
	__pos_proc	pos_proc;


	if (!(file->mode.file_kind == (uint_t)__disk_file ) || file->state.error) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = EFPOS;
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
	}

	if (file->state.io_state == (uint_t)__writing) {
		if (__flush_buffer(_file, NULL) != (int_t)__no_io_error) {
			set_error(file);
			MISRA_EXCEPTION_RULE_20_5()
			errno = EFPOS;
			MISRA_EXCEPTION_RULE_14_7()
			return(-1);
		}
	}

	if (mode == SEEK_CUR) {
		mode = SEEK_SET;
		if ((position = _ftell(_file)) == (fpos_t)-1) {
			position = 0U;
		}
		offset += position;
	}

#if	!(__dest_os	== __win32_os) /* this optimization breaks C++ ifstream */
	if ((mode != SEEK_END) && (file->mode.io_mode != (uint_t)__read_write_mode) &&
			((file->state.io_state == (uint_t)__reading) || (file->state.io_state == (uint_t)__rereading)))
	{	/* When in read mode, check for a position that is within the bounds of the current buffer */
		if ((offset >= file->position) || (offset < file->buffer_pos)) {
			/* not within buffer */
			file->state.io_state = (uchar_t)__neutral;
		} else {
			/* within the buffer, reset the buffer_ptr and buffer_len */
			file->buffer_ptr = file->buffer + (offset - file->buffer_pos);
			file->buffer_len = file->position - offset;
			file->state.io_state = (uchar_t)__reading;
		}
	} else {
		/* not reading */
		file->state.io_state = (uchar_t)__neutral;
	}

	if (file->state.io_state == (uchar_t)__neutral) {
		pos_proc = file->position_proc;
		if (pos_proc != 0) {
#if __dest_os == __starcore
			if ((*pos_proc)(file->handle, &offset, mode, file->ref_con))
#else
			if ((*pos_proc)(file->handle, &offset, mode))
#endif
			{
				set_error(file);
				MISRA_EXCEPTION_RULE_20_5()
				errno = EFPOS;
				MISRA_EXCEPTION_RULE_14_7()
				return(-1);
			}
		}

		file->state.eof      = 0U;
		file->position       = offset;
		file->buffer_len     = 0U;
	}

#else
	if (((pos_proc = file->position_proc) != 0) && (*pos_proc)(file->handle, &offset, mode)) {
		set_error(file);
		MISRA_EXCEPTION_RULE_20_5()
		errno = EFPOS;
		return(-1);
	}

	file->state.io_state = (int)__neutral;
	file->state.eof      = 0;
	file->position       = offset;
	file->buffer_len     = 0;
#endif

	return(0);
}

int_t _EWL_CDECL fseek(FILE * file, int32_t offset, int_t mode)
{
	fpos_t real_offset = (fpos_t)offset;
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	retval = _fseek(file, real_offset, mode);

  	__end_critical_region(crtrgn);

	return(retval);
}

int_t _EWL_CDECL fsetpos(FILE * file, const fpos_t * pos)
{
	int_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	retval = _fseek(file, *pos, SEEK_SET);

  	__end_critical_region(crtrgn);

	return(retval);
}



void _EWL_CDECL rewind(FILE * file)
{
	MISRA_EXCEPTION_RULE_11_4()
	((struct _FILE *)file)->state.error = 0U;				/* give the seek some hope of success */
	(void)fseek(file, 0L, SEEK_SET);
	MISRA_EXCEPTION_RULE_11_4()
	((struct _FILE *)file)->state.error = 0U;				/* the standard says we have to do this
										 * even if the seek fails (though it's
										 * admittedly not likely to
										 */
}
