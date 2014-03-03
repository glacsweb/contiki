/* EWL
 * Copyright © 2010 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/29 07:09:00 $
 * $Revision: 1.3 $
 */

#include <extras/sys/stat.h>
#include <syscall.h>
#include <errno.h>

#if _EWL_OS_DISK_FILE_SUPPORT 

typedef enum __io_results_stat
{
	__no_io_error_stat,
	__io_error_stat,
	__io_EOF_stat
} __io_results_stat;

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
typedef errno_t check_error_t;
#else
typedef int check_error_t;
#endif

/*
 *   Make a new directory given its pathname. Open mode is ignored.
 */   
_EWL_IMP_EXP_C int _EWL_CDECL mkdir(const char *path, mode_t mode) _EWL_CANT_THROW
{
	int retval;
	check_error_t error;

	error = __syscall(SYSCALL_mkdir, &retval, path, mode);
	if (error > 0) {
		errno = error;
	}

	if (retval == 0) {
		return __no_io_error_stat;
	}
  
	return __io_error_stat;
}

/*
 *   Remove a directory given its pathname. 
 */
_EWL_IMP_EXP_C int _EWL_CDECL rmdir(const char *path) _EWL_CANT_THROW
{
	int retval;
	check_error_t error;

	error = __syscall(SYSCALL_rmdir, &retval, path);
	if (error > 0) {
		errno = error;
	}

	if (retval == 0) {
		return __no_io_error_stat;
	}
  
	return __io_error_stat;
}

#endif //_EWL_OS_DISK_FILE_SUPPORT 