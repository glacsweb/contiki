/* Metrowerks ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * 
 * $Date: 2010/04/27 15:08:51 $ 
 * $Revision: 1.2 $ 
 */ 

/*
 *	semihosted_console_io.c	-	redirect console I/O through ARM semihosting interface
 *
 */

#include "console_io.h"
#include "semihosting.h"

/*
 *	sys_writec		-	write a character to the console via semihosting interface
 *
 */

__declspec(weak) asm void sys_writec(register void *cp)
{
#ifndef __thumb // ARM version

		// save return address on stack
		str		lr,[sp,#-4]!

		mov		r1,cp
		mov		r0,#SYS_WRITEC
		swi		SEMIHOSTING_SWI

		// restore return address from stack and return
		ldr		pc,[sp],#4

#else // Thumb version

		// save return address on stack
		push	{lr}

		mov		r1,cp
		mov		r0,#SYS_WRITEC
#if defined(__thumb2)
		bkpt		SEMIHOSTING_SWI
#else
		swi		SEMIHOSTING_SWI
#endif

		// restore return address from stack and return
		pop		{pc}

#endif 
}


/*
 *	sys_write0		-	write a nul-terminated string to the console via semihosting interface
 *
 */

__declspec(weak) asm void sys_write0(register void *str)
{
#ifndef __thumb // ARM version

		// save return address on stack
		str		lr,[sp,#-4]!

		mov		r1,str
		mov		r0,#SYS_WRITE0
		swi		SEMIHOSTING_SWI

		// restore return address from stack and return
		ldr		pc,[sp],#4

#else // Thumb version

		// save return address on stack
		push	{lr}

		mov		r1,str
		mov		r0,#SYS_WRITE0
#if defined(__thumb2)
		bkpt		SEMIHOSTING_SWI
#else
		swi		SEMIHOSTING_SWI
#endif

		// restore return address from stack and return
		pop		{pc}

#endif 
}


/*
 *	sys_readc		-	read a character from the console via semihosting interface
 *
 */

__declspec(weak) asm int sys_readc(void)
{
#ifndef __thumb // ARM version

		// save return address on stack
		str		r14,[r13,#-4]!
		
		mov		r1,#0
		mov		r0,#SYS_READC
		swi		SEMIHOSTING_SWI	

		// restore return address from stack and return
		ldr		pc,[sp],#4

#else // Thumb version

		// save return address on stack
		push	{lr}

		mov		r1,#0
		mov		r0,#SYS_READC
#if defined(__thumb2)
		bkpt		SEMIHOSTING_SWI
#else
		swi		SEMIHOSTING_SWI
#endif

		// restore return address from stack and return
		pop		{pc}

#endif 
}


/*
 *	sys_exit		-	terminate an application via semihosting interface
 *
 */

__declspec(weak) asm void sys_exit()
{
		LDCONST		r1,#ADP_Stopped_ApplicationExit
		mov		r0,0x18    // same as gcc libs, angel_SWIreason_ReportException
#if defined(__thumb2)
		bkpt		SEMIHOSTING_SWI
#else
		swi		SEMIHOSTING_SWI
#endif
		mov		pc,lr
}


/*
 *	__read_console	-	read from the console into 'buffer' until end-of-line or 'count' characters have been read
 *
 */

int __read_console(__std(__file_handle) handle, unsigned char *buffer, __std(size_t) *count)
{
	#pragma unused(handle)
	__std(size_t) i, n;
	int c;

	for (i = 0, n = *count; i < n; i++) {
		buffer[i] = sys_readc();
		/*	Metrowerks debugger returns '\r', while ARM debugger returns '\n',
			for 'return' key */
		if (buffer[i] == '\r' || buffer[i] == '\n')
		{
			*count = i + 1;
			break;
		}
	}
	
	return(__no_io_error);
}


/*
 *	__write_console	-	write 'count' bytes from 'buffer' to the console
 *
 */


#if 0
int	__write_console(__std(__file_handle) handle, unsigned char *buffer, __std(size_t) *count)
{
	#pragma unused(handle)
	__std(size_t) i, n;
	unsigned char tmp;


	if ( *count > 1 ) 
	{
			 
		tmp = buffer[*count-1];
		buffer[*count-1] = 0;
		sys_write0(buffer);
		
		// final character
		sys_writec(&tmp);
	}
	else // *count <= 1
	{
		sys_writec(buffer);
	}	

	return(__no_io_error);
}
#else
int	__write_console(__std(__file_handle) handle, unsigned char *buffer, __std(size_t) *count)
{
#if !defined(__SEMIHOSTING)
	#pragma unused(handle)
#endif
	__std(size_t) i, n, cnt;

#define _WC_BUFLEN 80
	static unsigned char tmpstring[_WC_BUFLEN+1];

	cnt = *count;

	if ( cnt <= 1)
	{
		sys_writec(buffer);
		return(__no_io_error);
	}

	i=0;

/* Use this for output to UART Terminal */

#if !defined(__SEMIHOSTING) 
	while ( cnt > _WC_BUFLEN )
	{

		strncpy(tmpstring,&buffer[i],_WC_BUFLEN);
		tmpstring[_WC_BUFLEN]=0;
		sys_write0(tmpstring);
		cnt -= _WC_BUFLEN;
		i += _WC_BUFLEN;
	}

	if ( cnt ) {
		strncpy(tmpstring,&buffer[i],cnt);
		tmpstring[cnt]=0;
		sys_write0(tmpstring);
	}

/* Use this for simulator output using semihosting interface (sys_write0 did not work for 
	gnu qemu simulator */
#else /* __SEMIHOSTING */
	__write_file(handle,buffer,count);
#endif

	return(__no_io_error);
}

#endif


/*
 *	__close_console	-	close the console
 *
 */

int __close_console(__std(__file_handle) handle)
{
	#pragma unused(handle)

	return(__no_io_error);
}
