/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/04 08:03:12 $
 * $Revision: 1.7 $
 */

/*
 *	Routines
 *	--------
 *		__read_console
 *		__write_console
 *		__close_console
 */

#include <ansi_parms.h>
#include <stdlib.h>
#include <string.h>
#include <console_io.h>
#include <syscall.h>

#include <ewl_misra_types.h>

#if _EWL_CONSOLE_SUPPORT

#define NO_USER_INPUT -2

/* Read information from a file */
MISRA_EXCEPTION_RULE_16_7()
int_t __read_console(__file_handle handle, uchar_t *buffer, size_t *count, __ref_con ref_con)
{
#pragma unused(ref_con)
	MISRA_QUIET_UNUSED_ARGS()
	int_t ioResult;
	int_t actual;

	do {
		(void)__syscall(SYSCALL_read, &actual, handle, buffer, *count);
	} while (actual == NO_USER_INPUT);

	/* Return number of bytes actually read */
	*count = (size_t)actual;

	/* If bytes read, return no error.  If none read, return EOF.  If error, return error. */
	if (actual > 0) {
		ioResult = (int_t)__no_io_error;
	} else {
		ioResult = (int_t)__io_EOF;
	}

	return ioResult;
}

/* Write information to a file */
MISRA_EXCEPTION_RULE_16_7()
int_t __write_console(__file_handle handle, uchar_t *buffer, size_t *count, __ref_con ref)
{
#pragma unused(ref)
	MISRA_QUIET_UNUSED_ARGS()
	int_t ioResult;
	int_t actual;

	(void)__syscall(SYSCALL_write, &actual, handle, buffer, *count);

	/* Set error if all bytes in the buffer were not written */
	if ((size_t)actual >= *count) {
		ioResult = (int_t)__no_io_error;
	} else {
		ioResult = (int_t)__io_error;
	}

	/* Return number of bytes actually written */
	*count = (size_t)actual;

	return ioResult;
}

/* Given a file handle, close the file */
/* If the file was created with tmpfile(), additionally delete the file */
MISRA_EXCEPTION_RULE_16_7()
int_t __close_console(__file_handle handle)
{
	#pragma unused(handle)
	MISRA_QUIET_UNUSED_ARGS()

	return (int_t)__no_io_error;
}

#endif /* _EWL_CONSOLE_SUPPORT */

