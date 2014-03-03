/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/04/26 15:34:23 $
 * $Revision: 1.3 $
 */

#include <errno.h>
#include <io.h>
#include <time.h>
#include <time_api.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static time_t _EWL_CDECL __convert_win32time_2timet(FILETIME* wintime);

long _EWL_CDECL _findfirst(
        const char * szWild,
        struct _finddata_t * pfd
        )

{
    WIN32_FIND_DATA FileData;
    HANDLE          hFile;
    
    if ((hFile = FindFirstFile(szWild, &FileData)) == INVALID_HANDLE_VALUE) {
        __set_errno(GetLastError());
        return (-1);
    }

    pfd->attrib  = (FileData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL)
                      ? 0 : FileData.dwFileAttributes;
    pfd->time_create  = __convert_win32time_2timet(&FileData.ftCreationTime);
    pfd->time_access  = __convert_win32time_2timet(&FileData.ftLastAccessTime);
    pfd->time_write   = __convert_win32time_2timet(&FileData.ftLastWriteTime);
    pfd->size         = FileData.nFileSizeLow;
    strcpy(pfd->name, FileData.cFileName);

    return ((long)hFile);
}


int _EWL_CDECL _findnext(long hFile, struct _finddata_t * pfd)

{
    WIN32_FIND_DATA FileData;
    if (!FindNextFile((HANDLE)hFile, &FileData)) {
		  __set_errno(GetLastError());     
        return (-1);
    }

    pfd->attrib       = (FileData.dwFileAttributes == FILE_ATTRIBUTE_NORMAL)
                      ? 0 : FileData.dwFileAttributes;
    pfd->time_create  = __convert_win32time_2timet(&FileData.ftCreationTime);
    pfd->time_access  = __convert_win32time_2timet(&FileData.ftLastAccessTime);
    pfd->time_write   = __convert_win32time_2timet(&FileData.ftLastWriteTime);

    pfd->size         = FileData.nFileSizeLow;
    strcpy(pfd->name, FileData.cFileName);

    return (0);
}


int _EWL_CDECL _findclose(long hFile)
{
    if (!FindClose((HANDLE)hFile)) {
        errno = EINVAL;
        return (-1);
    }
    return (0);
}

static time_t _EWL_CDECL __convert_win32time_2timet(FILETIME* wintime)
{
    FILETIME loc_time;
    SYSTEMTIME sys_time;
    struct tm tmp;
    if (!wintime->dwLowDateTime && !wintime->dwHighDateTime) return -1;
    

    /*
     * Convert to a broken down local time value
     */
    if ( !FileTimeToLocalFileTime(wintime, &loc_time) ||
         !FileTimeToSystemTime(&loc_time, &sys_time) )
    {
        return -1;
    }

	tmp.tm_sec  = sys_time.wSecond;
	tmp.tm_min  = sys_time.wMinute;       
	tmp.tm_hour = sys_time.wHour;      /* hours since midnight */
	tmp.tm_mday = sys_time.wDay;       /* day of month */
	tmp.tm_mon  = sys_time.wMonth-1;    /* since January(0 for Jan.) */
	tmp.tm_year = sys_time.wYear-1900; /* since 1900 */
	tmp.tm_wday = sys_time.wDayOfWeek; /* since Sunday(0-6)*/

	if(!__msl_leap_year(tmp.tm_year))  
		tmp.tm_yday = __msl_month_to_days[1][tmp.tm_mon]; 
	else
		tmp.tm_yday = __msl_month_to_days[2][tmp.tm_mon]; 
	                             /* days since jan. 1 */
	tmp.tm_isdst = -1;
/*	tmp.tm_isdst = __isdst();				/*- mm 010421 -*/

    return  mktime(&tmp);
}

/* Change record:
 * cc  000209 Changed windef.h to #define WIN32_LEAN_AND_MEAN & #include <windows.h>
 * cc  000518 Fixed #include
 * mm  010421 Corrected setting of tm_isdst 
 * cc  010918 Updated the include files
 */