/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/06/14 12:39:16 $
 * $Revision: 1.3 $
 */

/*
 *	Routines
 *	--------
 *		__get_clock
 *		__get_time
 *
 *		__to_gm_time
 *
 */

#include <stddef.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>  
#include <time.h> 
#include <time_api.h>
#include <x86/time.win32.h> 

time_t __systemtime_to_time_t(SYSTEMTIME *time) {

	long years, leap_years, leap_days, days, i, secs;

	static int MonthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
							/*jan feb mar apr may jun jul aug sep oct nov dec */

	/* Calculate the number of full years */
	years = time->wYear - 1970;		

	/* Calculate the number of leap years in that time */
	leap_years = years;
	if (time->wMonth > 2)
	{
	    leap_years++;
	}
	leap_days = (leap_years + 1) / 4;


	/* Calculate total days */

	days = (years * 365) + leap_days;

	/* Calculate days in this year */

	for (i=1; i<time->wMonth; i++)
	{
	    days += MonthDays[i-1];
	}
	days += time->wDay - 1;
	
	/* and calculate the seconds */

	secs =  (((((days * 24) + time->wHour) * 60) + time->wMinute) * 60) + time->wSecond;
	
	return secs;	
}

time_t __get_time(void){

  	SYSTEMTIME time;
	GetLocalTime(&time);
	return __systemtime_to_time_t(&time);
}

clock_t __get_clock(void){

	return(GetTickCount());				/*- mm 000111 -*/
}


/* Contributed by Cyrus Daboo  mm990910 */
int __to_gm_time(time_t * time)
{
	TIME_ZONE_INFORMATION tzi;
	long diff = 0;
	DWORD result = GetTimeZoneInformation(&tzi);
	switch(result)
	{
		case TIME_ZONE_ID_UNKNOWN:
			diff = tzi.Bias;
			break;
		case TIME_ZONE_ID_STANDARD:
			diff = tzi.Bias + tzi.StandardBias;
			break;
		case TIME_ZONE_ID_DAYLIGHT:
			diff = tzi.Bias + tzi.DaylightBias;
			break;
		default:
			return(0);
	}

	*time += diff * 60;
	return(1);
}

#if _EWL_WIDE_CHAR	

char __std[64],__dst[64];
int _daylight;
long _timezone;
char *_tzname[2]={&__std[0], &__dst[0]};


void _EWL_CDECL tzset(void)
{	
	
	TIME_ZONE_INFORMATION __systzinfo; 
	char *_TZ;
	long __convert;
	int __epm = 0;	
	_TZ = getenv("TZ"); 
	if ( _TZ != NULL ) 
	{
		/* The format that will be used:  */
		/* timezone name[+|-]difference between UTC and local time in hours */
		/* [:min[:seconds]][daylight savings time zone]*/
		 strncpy(_tzname[0], _TZ, 3);
		/* negative offset indicates timezone is east of the prime meridian. This is an optional flag */ 
		if (*(_TZ+=3) == '-'){
			__epm++;
			_TZ++;
		}
		if (*_TZ == '+') 
			_TZ++;
		if (*_TZ == '0') 
			_TZ++;
		if ((__convert = atol(_TZ++)) > 9) _TZ++;
		_timezone =  __convert *3600;
	
		if (*_TZ == ':')
		{
			_TZ++;
			if (*_TZ == '0') 
				_TZ++;
			__convert = atol(_TZ++);
			_timezone +=(__convert*60);
			if (__convert > 9) 
				_TZ++;
			if (*_TZ == ':')
			{
				_TZ++;
				if (*_TZ == '0') _TZ++;
				__convert = atol(_TZ++);
				_timezone += __convert;
				if (__convert > 9) _TZ++;
			}		
		}
	
		if (__epm > 0) 
			_timezone=-_timezone;
		if (*_TZ == '\0')
		{	
			_daylight =0;
			*_tzname[1] = '\0';
		}
		else
		{
			_daylight = 1;
			strncpy(_tzname[1],_TZ,3);
		}
	}

	if ( _TZ == NULL ){
		if(GetTimeZoneInformation( &__systzinfo) == 0xFFFFFFFF)
		{
			/* TZ is not set and we are not able to get the information		 */
			/* from the system.  Therefor we will set the default values.    */
		
			_daylight = 1;
		
			_timezone = 28800;
		
			_tzname[0] = "PST";
		
			_tzname[1] = "PDT";
		}
		else{	
					
			/* the bias has the difference in minutes of UTC and local */
			/* _timezone expects the difference in seconds therefor the * 60 */
			
			_timezone = __systzinfo.Bias * 60;
			
			/* if StandardDate contains a date for */
			/* trasition between daylight and standard time then */
			/* add StandardBias to the Bias used during standard time */
			
			if (__systzinfo.StandardDate.wMonth != 0) 
				_timezone += (__systzinfo.StandardBias * 60);
			
			/* if DaylightDate contains a date for the trasition between */
			/* standard to daylight then add DaylightBias to standard time */
			/****Note that you must back out the StandardBias to go back to standard time */
				
			if ((__systzinfo.DaylightDate.wMonth != 0) && (__systzinfo.DaylightBias != 0 ))
			
				_daylight = 1;
			else 
				_daylight = 0;
	
			wcstombs(_tzname[0],__systzinfo.StandardName,64);
			wcstombs(_tzname[1],__systzinfo.DaylightName,64);
		
		}
	}
}

#endif

/* begin addition */                            /*- mm 010421 -*/
int __isdst(void)
{
	TIME_ZONE_INFORMATION tzi;
	DWORD result = GetTimeZoneInformation(&tzi);
	switch(result)
	{
		case TIME_ZONE_ID_UNKNOWN:
			return (-1);
			break;
		case TIME_ZONE_ID_STANDARD:
			return (0);
			break;
		case TIME_ZONE_ID_DAYLIGHT:
			return (1);
			break;
		default:
			return(-1);
	}
}
/* end addition */                              /*- mm 010421 -*/

/* Change record:
 * KO  961010 Changed so that time is measured from 1970 rather than 1900. 
 * hh  980122 Replaced <windows.h> with the following TWO includes because it is seriously
 *            broken.  The following 2 includes must be carefully ordered as shown, because
 *            they are broken too.
 * mf  980624 don't include x86_prefix.h
 * mm  990910 Replacement code for __to_gm_time contributed by Cyrus Daboo, works for NT as
 *            well as for Win32 in regions where there is no daylight time.
 * cc  991222 added tzset
 * mm  000111 Reimplemented __get_clock---IR9912-2063
 * cc  000515 changed tzset to _tzset
 * cc  000518 fixed #includes
 * mm  010421 Added implementation of __isdst()
 * cc  010517 undid change 000515 
 * cc  010918 Updated the include files
 * cc  011203 Added _EWL_CDECL for new name mangling 
 * JWW 030224 Changed __NO_WIDE_CHAR flag into the new more configurable _EWL_WIDE_CHAR
 */