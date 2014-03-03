/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/27 13:22:30 $
 * $Revision: 1.8 $
 */

/*
 *	Routines
 *	--------
 *		clearerr
 *
 *		feof
 *		ferror
 *
 *		perror
 *
 *		__stdio_atexit
 */

#include <ansi_parms.h>

#include <errno.h>
#include <file_io.h>
#include <misc_io.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <abort_exit.h>
#include <string.h>
#include <ansi_files.h>
#endif

#include <ewl_misra_types.h>

void _EWL_CDECL clearerr(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	file->state.eof   = 0u;
	file->state.error = 0u;
}

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (feof)(FILE * file)
{
	MISRA_EXCEPTION_RULE_11_4()
	int_t res = feof((struct _FILE *)file) != 0U;
	return res;

	MISRA_EXCEPTION_RULE_16_7()
}

MISRA_QUIET_MACRO_FUNCDEF()
int_t _EWL_CDECL (ferror)(FILE * file)
{
	MISRA_EXCEPTION_RULE_11_4()
	int_t res = (int_t)ferror((struct _FILE *)file);
	return res;

	MISRA_EXCEPTION_RULE_16_7()
}

#if _EWL_OS_DISK_FILE_SUPPORT
void _EWL_CDECL perror(const char_t * s)
{
	char_t	errstr[__max_errstr];

	if (s && *s) {
		(void)fprintf(stderr, "%s: ", s);
	}

	MISRA_EXCEPTION_RULE_20_5()
	(void)fprintf(stderr, "%s\n", __strerror(errno, errstr));
}

void _EWL_CDECL __stdio_atexit(void)
{
	_EWL_LOCALDATA(__stdio_exit) = __close_all;
}
#endif
