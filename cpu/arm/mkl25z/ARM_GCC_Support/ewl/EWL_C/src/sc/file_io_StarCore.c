/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/03 10:38:55 $
 * $Revision: 1.9 $
 */

/*
 *	Routines
 *	--------
 *		__open_file
 *		__open_temp_file
 *		__read_file
 *		__write_file
 *		__position_file
 *		__flush_file
 *		__close_file
 *		__temp_file_name
 *		__delete_file
 *		__rename_file
 *
 *		__wopen_file
 *		__wtemp_file_name
 *		__wdelete_file
 *		__wrename_file
 */

#include <ansi_parms.h>
#include <errno.h>
#include <file_io.h>
#include <stdlib.h>
#include <string.h>
#include <syscall.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

#if _lint
#define DEFFILEMODE 0x1FF
#else
#define DEFFILEMODE 0777
#endif

#define __O_RDONLY		0x0000u		/* open the file in read only mode */
#define __O_WRONLY		0x0001u		/* open the file in write only mode */
#define __O_RDWR		0x0002u		/* open the file in read/write mode */
#define __O_APPEND		0x0004u		/* open the file in append mode */
#define __O_CREAT		0x0008u		/* create the file if it doesn't exist */
#define __O_TRUNC		0x0010u		/* truncate the file after opening it */
#define __O_BINARY		0x0020u		/* ascii/binary file */

#define io_result(ioResult) ((ioResult==0) ? (int_t) __no_io_error : (int_t) __io_error)
#define io_result_test(ioResult) ((ioResult) ? (int_t) __no_io_error : (int_t) __io_error)

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
typedef errno_t check_error_t;
#else
typedef int check_error_t;
#endif

typedef struct _temp_file_info temp_file_info;

/* The _temp_file_info struct contains enough information to identify a temporary file */
/* based on its __file_handle, and enough information to delete the temporary file when */
/* it is closed (for example, the path to the file passed to remove()) */
struct _temp_file_info
{
	__file_handle handle;
	char_t temp_name[L_tmpnam];
	struct _temp_file_info * next_struct;
	struct _temp_file_info * prev_struct;
};

static temp_file_info *temp_info_anchor = NULL;

static temp_file_info *find_temp_info(__file_handle handle)
{
	temp_file_info *p = temp_info_anchor;

	while (p != NULL) {
		if (p->handle == handle) {
			return(p);
		}
		p = p->next_struct;
	}

	return NULL;
}

int_t	__open_file(const char_t *name, __file_modes mode, __file_handle *handle)
{
	uint_t open_flag = 0u;
	int_t  result;
	check_error_t error;

	switch (mode.open_mode) {
		case __create_or_truncate:  open_flag |= __O_TRUNC;
									MISRA_EXCEPTION_RULE_15_2()
		case __create_if_necessary: open_flag |= __O_CREAT;
									MISRA_EXCEPTION_RULE_15_2()
		case __must_exist: break;
		default:           break;
	}
	if (mode.io_mode & (uchar_t)__append_mode) {
		open_flag |= __O_APPEND;
	}
	switch (mode.io_mode & (uchar_t)__read_write_mode) {
		case __read_mode:  open_flag |= __O_RDONLY; break;
		case __write_mode: open_flag |= __O_WRONLY; break;
		case __read_write_mode: open_flag |= __O_RDWR; break;
		default:           break;
	}
	if (mode.binary_io == 1u) { open_flag |= __O_BINARY; }

	error = __syscall(SYSCALL_open, &result, name, open_flag, DEFFILEMODE);
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}

	*handle = (__file_handle)result;

	return io_result_test(result>0);
}

int_t __open_temp_file(__file_handle * handle)
{
	int_t ioResult;
	temp_file_info *info;
	char_t temp_name[L_tmpnam];

	/* Get a temporary filename and attempt to open the file */
	__temp_file_name(temp_name, NULL);
	ioResult = __open_file(temp_name, __temp_file_mode, handle);

	if (ioResult == (int_t)__no_io_error) {
		/* Get space to keep track of the new temporary file */
		MISRA_EXCEPTION_RULE_20_4()
		info = malloc(sizeof(temp_file_info));

		if (info == NULL) {
			/* Space unavailable, so close and delete the file */
			(void)__close_file(*handle);
			(void)__delete_file(temp_name);
			return((int_t)__io_error);
		}

		/* Fill in the tracking information for the temporary file */
		info->handle = *handle;
		strcpy(info->temp_name, temp_name);
		info->next_struct = temp_info_anchor;
		info->prev_struct = NULL;

		/* Link the new tracking information in to the master list */
		if (temp_info_anchor != NULL) {
			temp_info_anchor->prev_struct = info;
		}
		temp_info_anchor = info;
	}

	return ioResult;
}


/* Read information from a file */
MISRA_EXCEPTION_RULE_16_7()
int_t __read_file(__file_handle handle, uchar_t *buffer, size_t *count, __ref_con ref_con)
{
#pragma unused(ref_con)
	MISRA_QUIET_UNUSED_ARGS()
	__io_results ioResult;
	int_t actual;
	check_error_t error;

	error = __syscall(SYSCALL_read, &actual, handle, buffer, *count);
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}

	/* Return number of bytes actually read */
	*count = (size_t)actual;

	/* If bytes read, return no error.  If none read, return EOF.  If error, return error. */
	if (actual > 0) {
		ioResult = __no_io_error;
	} else if (actual == 0) {
		ioResult = __io_EOF;
	} else {
		ioResult = __io_error;
	}

	return (int_t)ioResult;
}

/* Write information to a file */
MISRA_EXCEPTION_RULE_16_7()
int_t __write_file(__file_handle handle, uchar_t *buffer, size_t *count, __ref_con ref_con)
{
#pragma unused(handle, buffer, ref_con)
	MISRA_QUIET_UNUSED_ARGS()
	__io_results ioResult;
	size_t actual;
	check_error_t error;

	error = __syscall(SYSCALL_write, &actual, handle, buffer, *count);
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}

	/* Set error if all bytes in the buffer were not written */
	if (actual >= *count) {
		ioResult = __no_io_error;
	} else {
		ioResult = __io_error;
	}

	/* Return number of bytes actually written */
	*count = actual;

	return (int_t)ioResult;
}

/* Set a new position in the file */
MISRA_EXCEPTION_RULE_16_7()
int_t __position_file(__file_handle handle, uint32_t *position, int_t mode, __ref_con ref_con)
{
#pragma unused(ref_con)
	MISRA_QUIET_UNUSED_ARGS()
	int32_t offset;
	uint32_t actual;
	check_error_t error;

	/* Even though position is passed in as unsigned long, it's really a signed value */
	offset = *((int32_t *) position);

	error = __syscall(SYSCALL_lseek, &actual, handle, offset, mode);
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}
	/* assuming here that syscall returns the new absolute position */
	actual = actual;	/* !!! COMPUTE NEW ABSOLUTE POSITION */

	/* Return new absolute position in the file */
	*position = actual;

	return io_result_test((int32_t)actual>=0);
}

/* Given a file handle, close the file */
/* If the file was created with tmpfile(), additionally delete the file */
int_t __close_file(__file_handle handle)
{
	int_t retval, ioResult;
	temp_file_info *info;
	temp_file_info *p;
	check_error_t error;

	if ((handle==0u) || (handle==1u) || (handle==2u)) {
		return (int_t)__no_io_error;
	}

	/* Find out if the file to close is a temporary file */
	info = find_temp_info(handle);

	/* Close the file */
	error = io_result (__syscall(SYSCALL_close, &retval, handle));
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}
	ioResult = io_result_test(retval==0);

	/* If the file was a temporary file, delete it and remove the tracking information */
	if (info != NULL) {
		/* Delete the file only if the previous close operation succeeded */
		if (ioResult == (int_t)__no_io_error) {
			ioResult = __delete_file(info->temp_name);
		}

		/* Unlink the tracking information from the master list */
		if (temp_info_anchor == info) {
			temp_info_anchor = info->next_struct;
			if (temp_info_anchor != NULL) {
				temp_info_anchor->prev_struct = NULL;
			}
		} else {
			p = info->next_struct;
			if (p != NULL) {
				p->prev_struct = info->prev_struct;
			}
			if (info->prev_struct != NULL) {
				info->prev_struct->next_struct = info->next_struct;
			}
		}

		/* Dispose of the temporary file tracking information space */
		MISRA_EXCEPTION_RULE_20_4()
		free(info);
	}

	return ioResult;
}

/* Create a unique temporary filename for tmpnam() or tmpfile() */
/* The best way to generate a temporary filename is to combine the current thread ID */
/* on a multithreaded system and also timestamp, then avoid collisions with files which */
/* may already have that unique signature */
MISRA_EXCEPTION_RULE_16_7()
void __temp_file_name(char_t *name_str, void *ptr)
{
	#pragma unused(ptr)
	MISRA_QUIET_UNUSED_ARGS()

	int_t ioResult;
	__file_handle handle;
	uint32_t count;
	int_t  i, n;
	char_t *name_ptr;
	char_t temp_name[] = "temp00000000";

	__file_modes __temp_name_mode =
#if _EWL_WIDE_CHAR
		{(uchar_t)__must_exist, (uchar_t)__read_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, (uchar_t)__unoriented, 1u};
#else
		{(uchar_t)__must_exist, (uchar_t)__read_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, 1u};
#endif /* _EWL_WIDE_CHAR */

	static uint32_t counter;

	/* Simplistic method starts a filename at temp00000000 and increases the number */
	/* as hexadecimal until a unique name is found */
	do {
		/* Try next number */
		count = counter++;

		/* Find the end of the temporary filename (the end of the hex number) */
		name_ptr = &temp_name[strlen(temp_name)];

		/* Convert the count number to hex and put it in the filename */
		for (i = 8; i--;) {
			MISRA_EXCEPTION_RULE_10_3()
			n = (int_t)(count & 0x0Fu);
			count >>= 4;
			if (n < 10) {
				n += '0';
			} else {
				n += 'A' - 10;
			}
			*--name_ptr = (char_t)n;
		}

		/* Attempt to open the temporary file to see if it already exists */
		/* Open mode is __must_exist, so an open success means the filename is NOT unique */
		/* !!! THERE MAY BE BETTER PLATFORM-SPECIFIC WAYS TO TEST FOR UNIQUENESS HERE !!! */
		ioResult = __open_file(temp_name, __temp_name_mode, &handle);
		if (ioResult == (int_t)__no_io_error) {
			(void)__close_file(handle);
		}
	} while (ioResult == (int_t)__no_io_error);

	/* Return the unique temporary filename */
	strcpy(name_str, temp_name);
}

/* Delete a file given its pathname */
int_t __delete_file(const char_t *name)
{
	int_t retval;
	check_error_t error;

	error = __syscall(SYSCALL_unlink, &retval, name);
	if (error > 0) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = error;
	}

	return io_result_test(retval==0);
}

/* Rename a file given its existing name and its desired new name */
int_t __rename_file(const char_t *old_name, const char_t *new_name)
{
	int_t ioResult, retval;

	ioResult = __syscall(SYSCALL_rename, &retval, old_name, new_name);

	return io_result(ioResult);
}

#pragma mark -

#if _EWL_WFILEIO_AVAILABLE

#include <mbstring.h>
#include <wstring.h>

#define MAXPATH 1024

static void __ewl_wcs_to_utf_8(const wchar_t *name, char_t *utf_8)
{
	int_t i;
	int_t size;
	int_t total_size;
	char_t *converted;

	total_size = 0;
	converted = utf_8;
	for (i = 0; (i < wcslen(name)) && (total_size < MAXPATH); i++) {
		if (total_size < MAXPATH - 3) {
			size = __unicode_to_UTF8(converted, name[i]);
			converted += size;
			total_size += size;
		}
	}
	*converted = 0;
}

int_t	__wopen_file(const wchar_t *name, __file_modes mode, __file_handle *handle)
{
	char_t utf_8_name[MAXPATH];

	__ewl_wcs_to_utf_8(name, utf_8_name);
	return __open_file(utf_8_name, mode, handle);
}

void __wtemp_file_name(wchar_t *name_str, void *)
{
	char_t name[MAXPATH];
	char_t *char_name;

	char_name = name;
	__temp_file_name(char_name, NULL);

	do {
		*name_str = *char_name;
		name_str++;
		char_name++;

	} while (*char_name != 0);
}

int_t __wdelete_file(const wchar_t *name)
{
	char_t utf_8_name[MAXPATH];

	__ewl_wcs_to_utf_8(name, utf_8_name);
	return __delete_file(utf_8_name);
}

int_t __wrename_file(const wchar_t *old_name, const wchar_t *new_name)
{
	char_t old_name_utf_8[MAXPATH];
	char_t new_name_utf_8[MAXPATH];

	__ewl_wcs_to_utf_8(old_name, old_name_utf_8);
	__ewl_wcs_to_utf_8(new_name, new_name_utf_8);
	return __rename_file(old_name_utf_8, new_name_utf_8);
}

#endif /* _EWL_WFILEIO_AVAILABLE */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */
