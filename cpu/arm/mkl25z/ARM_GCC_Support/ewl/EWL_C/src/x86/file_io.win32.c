/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/03/25 10:37:17 $
 * $Revision: 1.6 $
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
 *		__wopen_temp_file
 *		__wtemp_file_name
 *		__wdelete_file
 *		__wrename_file
 */

#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <file_io.h>
#include <extras/fcntl.h>
#include <tchar.h>
#include <x86/crtl.h>
#include <x86/startup.win32.h>
#include <errno.h>

#if __dest_os==__wince_os
 #ifdef UNICODE
  #define _itot _itow
 #else
  #define _itot __EWL_itoa 			/*- cc 010605 -*/
 #endif
#endif

#define _EWL_ALLOW_SHARED_WRITING  1	/* Set this value to 1 for file opening with shared writing */	/*- mm 020405 -*/
#define _READWRITEMASK  3				/*- mm 980909 -*/

/*- KO 961120 -*/
/* One of these structs is used for every temporary file that is opened. */
typedef struct {
	__file_handle	mFileHandle;
	 TCHAR			*mFileName;
} temp_file_info;

static temp_file_info	temp_info[FOPEN_MAX];
#ifdef __INTEL__
extern unsigned long mytime();
#endif

static temp_file_info * find_temp_info(__file_handle inHandle)
{
	int theCount;
	temp_file_info * theTempFileStruct;

	for (theCount = 0, theTempFileStruct = temp_info;
	     theCount < FOPEN_MAX; theCount++, theTempFileStruct++) {

		if (theTempFileStruct->mFileHandle == inHandle)
			return(theTempFileStruct);
	}

	return(0);
}


int _EWL_CDECL __EWL_GetHandle()
{
    int i;

    for (i=0; i<256; i++)
    {
        if (_HandPtr == NUM_HANDLES)
        {
            _HandPtr = 3;
        }
        if (_HandleTable[_HandPtr] == 0)
        {
            return _HandPtr++;
        }
        _HandPtr++;
    }
    return -1;
}


char* _EWL_CDECL __EWL_getcwd(char * path, int maxlen)						/*- mm 980518 -*/ /*- cc 010517 -*/
{
    int len;

    if (path == NULL)
    {
        errno = EINVAL;
            return NULL;
        }

    len = GetCurrentDirectory(maxlen, path);

    if (len > maxlen)
    {
        errno = ERANGE;
        return NULL;
    }

    return path;
}

/*-	EJS 020218: rewrote */
char * _EWL_CDECL __EWL_fullpath(char * absPath, const char * relPath, size_t maxLength) {
	char 	*full;
	char 	*ptr;
	int		len;

	/*	Get memory. */
	full = absPath;
	if (!full) {
		full = (char*) malloc(maxLength);
		if (!full) {
			return 0L;
		}
	} else {
		full = absPath;
	}

	/* 	Make Win32 do the dirty work. */
	len = GetFullPathName(relPath, maxLength-1, full, &ptr);
	if (len == 0) {
		__set_errno(GetLastError());
		if (!absPath) free(full);
		return 0L;
	} else if (len >= maxLength) {
		__set_errno(ERROR_BUFFER_OVERFLOW);
		if (!absPath) free(full);
		return 0L;
	}

	return full;
}

long _EWL_CDECL __EWL_lseek(int fildes, long offset,int whence)				/*- mm 980518 -*/	/*- cc 010601 -*/
{
	HANDLE h;
	DWORD method;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }

	h = _HandleTable[fildes]->handle;

	switch(whence)
	{
	case SEEK_SET:
	    method = FILE_BEGIN;
		break;

	case SEEK_CUR:
	    method = FILE_CURRENT;
		break;

	case SEEK_END:
		method = FILE_END;
		break;
	}

    return SetFilePointer(h, offset, NULL, method);
}


int _EWL_CDECL __EWL_write(int fildes, const void *buf, size_t count)				/*- mm 980518 -*/  /*- mm 000607 -*/	/*- cc 010601 -*/
{
    DWORD wrote;
	HANDLE h;
	int t;
	char *bp = 0;
	int status;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }

	h = _HandleTable[fildes]->handle;
	t = _HandleTable[fildes]->translate;

    if (t)
    {
         int i, n;
         char *cp;

         n = 0;
         for (i=0; i<count; i++)
         {
             if (((char*)buf)[i] == '\n')    /*- mm 000607 -*/
             {
                 n++;
             }
         }
         bp = (char *)malloc(count + n);
         cp = bp;
         for (i=0; i<count; i++)
         {
             if (((char*)buf)[i] == '\n')  /*- mm 000607 -*/
             {
                 *cp++ = '\r';
             }
             *cp++ = ((char*)buf)[i];   /*- mm 000607 -*/
         }
         count += n;
         buf = bp;
    }

    // Do the write operation

    if (_HandleTable[fildes]->append)
    {
        __EWL_lseek(fildes, 0, SEEK_END);
    }
#ifdef _WINSIOUX                    /*- mm 980119 -*/

    if ((fildes == 1) || (fildes == 2))
    {
    	WriteCharsToConsole(buf, count);
    	status = 1;
    	wrote  = count;
    }
    else
#endif /* _WINSIOUX  */             /*- mm 980119 -*/

    status = WriteFile(h, buf, (DWORD)count, &wrote, 0);	/*- mm 000607 -*/

    // Free the local buffer if we allocated one

    if (bp)
    {
        free(bp);
    }

    // And return status

    if (status)
	{
	    return wrote;
	}
	else
	{
		// get error code
		__set_errno(GetLastError());

	    return 0;
	}
}


int _EWL_CDECL __EWL_close(int fildes)										/*- mm 980518 -*/ /*- cc 010517 -*/	/*- cc 010601 -*/
{
	HANDLE h;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
        return -1;
    }

	h = _HandleTable[fildes]->handle;

    if (CloseHandle(h))
    {
        free(_HandleTable[fildes]);
        _HandleTable[fildes] = 0;

        return 0;
    }
    else
    {
		// get error code
		__set_errno(GetLastError());

        return -1;
    }
}


int _EWL_CDECL __EWL_read(int fildes, void * buf, size_t count)	/*- mm 980518 -*/ /*- mm 000607 -*/	/*- cc 010601 -*/
{
    DWORD read;
	HANDLE h;
	int t;
	int ReadResult;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }
	h = _HandleTable[fildes]->handle;
	t = _HandleTable[fildes]->translate;

#ifdef _WINSIOUX                    /*- mm 980119 -*/
	if (fildes == 0)
		ReadResult = ReadCharsFromConsole(buf, (DWORD)count, &read);   /*- mm 000607 -*/
	else
#endif  /* if _WINSIOUX */			/*- mm 980119 -*/
		ReadResult = ReadFile(h, buf, (DWORD)count, &read, 0);	/*- mm 000607 -*/
    if (ReadResult)
	{
	    if (read && t)
	    {
	        char *cp, *op;
	        int i, n;

	        cp = op = buf;
	        n = 0;
	        for(i=0; i<read; i++)
	        {
	            // trun CRLF into just LF

	            if (cp[0] == '\r' && i < (read-1) && cp[1] == '\n')  /*- mm 000222 -*/
	            {
	                cp++;
	                n++;
	            }
	            else
	            {
	                *op++ = *cp++;
	            }
	        }

	        // If buffer ended in a CR and the read did not come up short due to hitting the
	        // end of file, pretend we did not see that CR and back up the file pointer.

	        if (read == count && read > 1 && ((char*)buf)[read-1] == '\r') /*- mm 000607 -*/
	        {
	            __EWL_lseek(fildes, -1, SEEK_CUR);
	            n++;
	        }
	        read -= n;
	    }
	    return read;
	}
	else
	{
		// get error code
		__set_errno(GetLastError());

	    return 0;
	}
}

int _EWL_CDECL __EWL_open(const char *path, int flags)	/*- mm 980507 -*/ /*- mm 980710 -*/	/*- cc 010601 -*/
{
    DWORD Access;
	DWORD Share;
	DWORD Create;
	int h;

	/* Ensure that exactly one of O_RDONLY, O_RDWR, O_WRONLY is set */   /*- mm 980923 -*/
	if ((((flags & O_RDONLY)!=0) + ((flags & O_RDWR)!=0) + ((flags & O_WRONLY)!=0)) != 1)
		return (-1);
	/* Reject POSIX undefined combination */
	if ((flags & O_RDONLY) && (flags & O_TRUNC))
		return (-1);
	/* Reject as undefined exclusive without create */  /*- mm 990204  -*/
	if ((flags & O_EXCL) && !(flags & O_CREAT))
		return (-1);

	/* Make sure we have a handle available */

	h = __EWL_GetHandle(); /*- cc 010605 -*/
	if (h == -1)
	{
	    return -1;
	}
	_HandleTable[h] = (FileStruct *)malloc(sizeof(FileStruct));
	if (!_HandleTable[h])
	{
	    return -1;
	}

	/* Set up the translate mode flag */

	_HandleTable[h]->translate = (flags & O_BINARY) == 0;
	_HandleTable[h]->append = 0;

#if (_EWL_ALLOW_SHARED_WRITING == 1)									/*- mm 020405 -*/
	Share = FILE_SHARE_WRITE | FILE_SHARE_READ;		/*- mm 000524 -*/	/*- mm 020405 -*/
#else																	/*- mm 020405 -*/
	Share = FILE_SHARE_READ;						/*- mm 000524 -*/	/*- mm 020405 -*/
#endif /* _EWL_ALLOW_SHARED_WRITING */									/*- mm 020405 -*/
	/* Get the access flags */

	if (flags & O_RDONLY)
	{
	    Access = GENERIC_READ;
	}
	else if (flags & O_WRONLY)
	{
	    Access = GENERIC_WRITE;
	}
	else
	{
	    Access = GENERIC_READ | GENERIC_WRITE;
	}

	/* Get the create flags */

	if (flags & O_CREAT)
	{
	    Create = OPEN_ALWAYS;   			/*- mm 991210 -*/
		if (flags & O_TRUNC)				/*- mm 991210 -*/
			Create = CREATE_ALWAYS; 		/*- mm 991210 -*/
	    if (flags & O_EXCL)					/*- vss 990129 -*/
	    {
	    	Create = CREATE_NEW;
	    }
	}
	else if (flags & O_TRUNC)
	{
	    Create = TRUNCATE_EXISTING;
	}
	else
	{
	    Create = OPEN_EXISTING;
	}


	/* Do the open or create */

    _HandleTable[h]->handle = CreateFile((LPCTSTR)path, (DWORD)Access, (DWORD)Share,
        (LPSECURITY_ATTRIBUTES)0, (DWORD)Create, (DWORD)0, (HANDLE)0);

	/* See if we suceeded */

	if (_HandleTable[h]->handle != INVALID_HANDLE_VALUE)
	{
		/* if appending, position to the end of the file */

	    if (flags & O_APPEND)
	    {
	        __EWL_lseek(h, 0, SEEK_END);
	        _HandleTable[h]->append = 1;
	    }

	    /* and return the handle */

	    return h;
	}

	/* Open failed, free the FileStruct assocation with the handle */

	free(_HandleTable[h]);
	_HandleTable[h] = 0;

	/* get error code */
	__set_errno(GetLastError());

	/* And return failure */

	return -1;
}


int	_EWL_CDECL __open_file(const char * name, __file_modes mode, __file_handle * handle)
{
		int h;
		int flag = 0;

		if ((mode.io_mode & _READWRITEMASK) == __read_mode) flag |= O_RDONLY;	/*- mm 980909 -*/
		if ((mode.io_mode & _READWRITEMASK) == __write_mode) flag |= O_WRONLY;	/*- mm 980909 -*/
		if ((mode.io_mode & _READWRITEMASK) == __read_write_mode) flag |= O_RDWR;/*- mm 980909 -*/
		if (mode.io_mode & __append_mode) flag |= O_APPEND;

		if (mode.binary_io) flag |= O_BINARY;

		if (mode.open_mode == __create_if_necessary) flag |= O_CREAT;
		if (mode.open_mode == __create_or_truncate) flag |= (O_CREAT | O_TRUNC);

		h = __EWL_open(name,flag);  /*- cc 010518 -*/	/*- cc 010601 -*/
		if (h == -1) {
			return __io_error;
		}
		else {
			*handle = h;
			return __no_io_error;
		}

}

static unsigned int __previous_time;

void _EWL_CDECL __temp_file_name(char * name_str, void * /*fsspec*/)  /*- hh 980122 -*/
{
	WIN32_FIND_DATA fd;
	HANDLE			fh;
	TCHAR			tmpstr[16];
	int				i = 0;

	__previous_time += GetTickCount();
	do {
		__previous_time = (__previous_time + 1) & 0x7fffffff;

		_tcscpy((TCHAR*)name_str, _T(".\\EWL"));
		_tcscat((TCHAR*)name_str, __ewl_itoa(__previous_time, tmpstr, 10));
		_tcscat((TCHAR*)name_str, _T(".tmp"));

		fh = FindFirstFile((LPCTSTR)name_str,&fd);
		if (fh != INVALID_HANDLE_VALUE) FindClose(fh);
	}	while (GetLastError() != ERROR_FILE_NOT_FOUND && ++i < 65536);
}

int _EWL_CDECL __open_temp_file(__file_handle * handle)
{
	int theError;
	temp_file_info * theTempInfo;
	TCHAR temp_name[1024];
	theTempInfo = find_temp_info(0);

	if (!theTempInfo) {return __io_error;}
    __temp_file_name((char*)temp_name,	0);
	theError = __open_file((char*)temp_name, __temp_file_mode, handle);


	if (theError == __no_io_error)
	{
		/* The file has successfully opened. Fill in a temporary info struct for it.*/
		theTempInfo->mFileHandle = *handle;
		/* Is 1024 a good max file path length to assume?*/
		#if (__dest_os==__win32_os)
		theTempInfo->mFileName = __EWL_fullpath(0, temp_name, 1024); /*- cc 010605 -*/
		#else
		theTempInfo->mFileName=(TCHAR*)malloc(sizeof(temp_name)+1);
		_tcscpy(theTempInfo->mFileName,temp_name);
		#endif
	}

	return theError;
}

int __read_file(__file_handle handle, unsigned char * buffer, size_t * count)  /*- hh 980122 -*/
{
/*	fflush(stdout);	*//*- KO 960903 -*/  /*- cc  P70384.c Execution passed */
	*count =  __EWL_read(handle,(char *) buffer,*count);		/*- cc 010601 -*/
	return *count ? __no_io_error : __io_EOF;					/*- KO 961104 -*/
}

int __write_file(__file_handle handle, unsigned char * buffer, size_t * count)  /*- hh 980122 -*/
{
	size_t n;
	n = __EWL_write(handle,(char *) buffer,*count);				/*- cc 010601 -*/
	if (n >= *count) {
		*count = n;		/*!!CAM!!  Testing */
		return  __no_io_error;
	}
	else {
		*count = n;
		return __io_error;
	}
}



int __position_file(__file_handle handle, fpos_t * position, int mode)  /*- hh 980122 -*/
{
	*position = __EWL_lseek(handle, *position,mode);			/*- cc 010601 -*/
	return *position == -1 ? __io_error : __no_io_error;
}

int __close_file(__file_handle handle)
{
	temp_file_info * theTempInfo;

	if (__EWL_close(handle) == -1) 								/*- cc 010601 -*/
		return __io_error;

	/*
	 * If this is a temporary file, there will be a temp_file_info struct for this
	 * handle. In that case, delete the file and clean out the temp_file_info struct.
	 * File handle 0 is a special case (these are ANSI file handles not Windows file
	 * handles). File handle 0 is stdin which is not a temporary file. Since
	 * find_temp_info on handle 0 returns an empty temp info structure rather than
	 * a "not found" I'm special casing it.
	 */
	if (handle && (theTempInfo = find_temp_info(handle)) != NULL) {
		int theError = __delete_file((char*)theTempInfo->mFileName);

		free(theTempInfo->mFileName); /* matches malloc in _open_temp_file */
		theTempInfo->mFileHandle = 0;

		return theError;
	} else {
		return __no_io_error;
	}
}

int _EWL_CDECL __delete_file(const char* name)
{
	return DeleteFile((LPCTSTR)name)  ? __no_io_error : __io_error;
}

int _EWL_CDECL __rename_file(const char * old_name, const char * new_name)
{
	return MoveFile((LPTSTR)old_name,(LPTSTR)new_name) ? __no_io_error : __io_error;
}

#pragma mark -

#if _EWL_WFILEIO_AVAILABLE

#include <wprintf.h>

/*-	EJS 020218: rewrote */
wchar_t * _EWL_CDECL __EWL_wfullpath(wchar_t * absPath, const wchar_t * relPath, size_t maxLength) {
	wchar_t	*full;
	wchar_t	*ptr;
	int		len;

	/*	Get memory. */
	full = absPath;
	if (!full) {
		full = (wchar_t*) malloc(maxLength * sizeof(wchar_t));
		if (!full) {
			return 0L;
		}
	} else {
		full = absPath;
	}

	/* 	Make Win32 do the dirty work. */
	len = GetFullPathNameW(relPath, maxLength-1, full, &ptr);
	if (len == 0) {
		__set_errno(GetLastError());
		if (!absPath) free(full);
		return 0L;
	} else if (len >= maxLength) {
		__set_errno(ERROR_BUFFER_OVERFLOW);
		if (!absPath) free(full);
		return 0L;
	}

	return full;
}

int _EWL_CDECL __EWL_wopen(const wchar_t *path, int flags)
{
    DWORD Access;
	DWORD Share;
	DWORD Create;
	int h;

	/* Ensure that exactly one of O_RDONLY, O_RDWR, O_WRONLY is set */   /*- mm 980923 -*/
	if ((((flags & O_RDONLY)!=0) + ((flags & O_RDWR)!=0) + ((flags & O_WRONLY)!=0)) != 1)
		return (-1);
	/* Reject POSIX undefined combination */
	if ((flags & O_RDONLY) && (flags & O_TRUNC))
		return (-1);
	/* Reject as undefined exclusive without create */  /*- mm 990204  -*/
	if ((flags & O_EXCL) && !(flags & O_CREAT))
		return (-1);

	/* Make sure we have a handle available */

	h = __EWL_GetHandle(); /*- cc 010605 -*/
	if (h == -1)
	{
	    return -1;
	}
	_HandleTable[h] = (FileStruct *)malloc(sizeof(FileStruct));
	if (!_HandleTable[h])
	{
	    return -1;
	}

	/* Set up the translate mode flag */

	_HandleTable[h]->translate = (flags & O_BINARY) == 0;
	_HandleTable[h]->append = 0;

#if (_EWL_ALLOW_SHARED_WRITING == 1)									/*- mm 020405 -*/
	Share = FILE_SHARE_WRITE | FILE_SHARE_READ;		/*- mm 000524 -*/	/*- mm 020405 -*/
#else																	/*- mm 020405 -*/
	Share = FILE_SHARE_READ;						/*- mm 000524 -*/	/*- mm 020405 -*/
#endif /* _EWL_ALLOW_SHARED_WRITING */									/*- mm 020405 -*/
	/* Get the access flags */

	if (flags & O_RDONLY)
	{
	    Access = GENERIC_READ;
	}
	else if (flags & O_WRONLY)
	{
	    Access = GENERIC_WRITE;
	}
	else
	{
	    Access = GENERIC_READ | GENERIC_WRITE;
	}

	/* Get the create flags */

	if (flags & O_CREAT)
	{
	    Create = OPEN_ALWAYS;   			/*- mm 991210 -*/
		if (flags & O_TRUNC)				/*- mm 991210 -*/
			Create = CREATE_ALWAYS; 		/*- mm 991210 -*/
	    if (flags & O_EXCL)					/*- vss 990129 -*/
	    {
	    	Create = CREATE_NEW;
	    }
	}
	else if (flags & O_TRUNC)
	{
	    Create = TRUNCATE_EXISTING;
	}
	else
	{
	    Create = OPEN_EXISTING;
	}


	/* Do the open or create */

    _HandleTable[h]->handle = CreateFileW((LPCWSTR)path, (DWORD)Access, (DWORD)Share,
        (LPSECURITY_ATTRIBUTES)0, (DWORD)Create, (DWORD)0, (HANDLE)0);

	/* See if we suceeded */

	if (_HandleTable[h]->handle != INVALID_HANDLE_VALUE)
	{
		/* if appending, position to the end of the file */

	    if (flags & O_APPEND)
	    {
	        __EWL_lseek(h, 0, SEEK_END);
	        _HandleTable[h]->append = 1;
	    }

	    /* and return the handle */

	    return h;
	}

	/* Open failed, free the FileStruct assocation with the handle */

	free(_HandleTable[h]);
	_HandleTable[h] = 0;

	/* get error code */
	__set_errno(GetLastError());

	/* And return failure */

	return -1;
}


int	_EWL_CDECL __wopen_file(const wchar_t * name, __file_modes mode, __file_handle * handle)
{
	int h;
	int flag = 0;

	if ((mode.io_mode & _READWRITEMASK) == __read) flag |= O_RDONLY;	/*- mm 980909 -*/
	if ((mode.io_mode & _READWRITEMASK) == __write) flag |= O_WRONLY;	/*- mm 980909 -*/
	if ((mode.io_mode & _READWRITEMASK) == __read_write) flag |= O_RDWR;/*- mm 980909 -*/
	if (mode.io_mode & __append) flag |= O_APPEND;

	if (mode.binary_io) flag |= O_BINARY;

	if (mode.open_mode == __create_if_necessary) flag |= O_CREAT;
	if (mode.open_mode == __create_or_truncate) flag |= (O_CREAT | O_TRUNC);

	h = __EWL_wopen(name, flag);
	if (h == -1) {
		return __io_error;
	}
	else {
		*handle = h;
		return __no_io_error;
	}
}

void _EWL_CDECL __wtemp_file_name(wchar_t * name_str, void * /*fsspec*/)  /*- hh 980122 -*/
{
	WIN32_FIND_DATAW	fd;
	HANDLE				fh;
	int					i = 0;

    if (name_str)
    {
		__previous_time += GetTickCount();
		do {
			__previous_time = (__previous_time + 1) & 0x7fffffff;
     		swprintf(name_str, 20, L".\\EWL%d.tmp", __previous_time);
	 		fh = FindFirstFileW((LPCWSTR)name_str, &fd);
	 		if (fh != INVALID_HANDLE_VALUE) FindClose(fh);
    	}
		while(GetLastError() != ERROR_FILE_NOT_FOUND && ++i < 65536);
	}
}

int _EWL_CDECL __wdelete_file(const wchar_t* name)
{
	return DeleteFileW((LPCWSTR)name)  ? __no_io_error : __io_error;
}

int _EWL_CDECL __wrename_file(const wchar_t * old_name, const wchar_t * new_name)
{
	return MoveFileW((LPCWSTR)old_name,(LPCWSTR)new_name) ? __no_io_error : __io_error;
}

#endif /* _EWL_WFILEIO_AVAILABLE */

/* Change record:
 * KO  960903 I added this flush line so output will be flushed while waiting for input.
 * KO  961104 To accomodate the change mm 961031 in stdio.h.
 * KO  961120 Added code to delete temporary files when they are closed.
 * KO  961209 Fixed __close_file so it does not treat stdin as a temporary file.
 * hh  980122 Replaced <windows.h> with the following TWO includes because it is seriously
 *            broken.  The following 2 includes must be carefully ordered as shown, because
 *            they are broken too.
 * hh  980122 unused arg commented out in 4 places
 * mf  980518 wince changes-- using microsoft's common "_t" interface for supporting both
 *            wchar and char
 * mm  980708 Corrected terminating condition on construction of temp name to avoid duplication
 * vss 980729 Fix temp file name generation
 * mm  980909 Changes to make our use of open() conform to Posix specification
 * mf  990208 fixed tmpfile bug on 486(some 486 machines don't have rdtsr instruction)
 * cc  000209 changed windef.h to #define WIN32_LEAN_AND_MEAN & #include <windows.h>
 * cc  000515 Fixed #include
 * cc  010518 Changed _open to open
 * cc  010601 Made  __EWL_lseek= to lseek,__EWL_write = to write, __EWL_close = to close,
 * 			  __EWL_read = to read,__EWL_open = to open for EWL C
 * cc  010605 Made __EWL_fullpath = to _fullpath, __EWL_GetHandle = to GetHandle
 *			  __EWL_getcwd = getcwd	for EWL C
 * cc  010605 Made calls equal to itoa call __EWL_itoa
 * cc  010725 Changed call strdup to __EWL_strdup
 * cc  010115 Removed dup prototype
 * ejs 020218 Rewrote __EWL_fullpath.
 * mm  020405 Added wrappers to allow shared writing
 * JWW 020906 Use generic reference constant instead of specific idle_proc in file I/O
 * JWW 021010 Added wchar_t file I/O routines controlled by _EWL_WFILEIO_AVAILABLE
 */
