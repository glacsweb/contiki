/* Metrowerks ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * 
 * $Date: 2010/04/19 12:18:21 $ 
 * $Revision: 1.2 $ 
 */ 

/*

FILE
	__arm_eabi_init.c

DESCRIPTION

	Use this file for C.
	
	Interface for board-level initialization and termination.
	
	If hardware initialization and pre-main user initialization are required,
	copy this file to your project directory and customize it (instead of
	customizing __start.c).
	
	TBD: targetting environments: bare board, Symbian, ARMulator.

*/

#include "file_io.h"
#include "semihosting.h"
#include "MWException.h"


/*
 *	_ExitProcess
 *
 */

void _ExitProcess(void)
{
	sys_exit();
}


#ifndef _MAX_ATEXIT_FUNCS
	#define _MAX_ATEXIT_FUNCS 64
#endif

static DestructorChain atexit_funcs[_MAX_ATEXIT_FUNCS];
static long	atexit_curr_func = 0;

extern void *__register_global_object(void *object,void *destructor,void *regmem)
{
	((DestructorChain *)regmem)->next=__global_destructor_chain;
	((DestructorChain *)regmem)->destructor=destructor;
	((DestructorChain *)regmem)->object=object;
	__global_destructor_chain=(DestructorChain *)regmem;

	return object;
}

extern int __register_atexit(void (*func)(void))
{
	if (atexit_curr_func == _MAX_ATEXIT_FUNCS)
		return -1;
	__register_global_object(0, func, &atexit_funcs[atexit_curr_func++]);
	return 0;
}

extern int __cxa_atexit(void (*f)(void *), void *p, void *d); 
extern int __cxa_atexit(void (*f)(void *), void *p, void *d)
{
	__register_global_object(p, f, d);
}

extern int __aeabi_atexit(void* p, void (*f)(void*), void* d);
extern int __aeabi_atexit(void* p, void (*f)(void*), void* d)
{
	__register_global_object(p, f, d);
}


// __init_registers, __init_hardware, __init_user suggested by Kobler 
__declspec(weak) asm void __init_registers(void)
{
	mov pc,lr
	
}

#ifdef EWL_VFP11
__declspec(weak) asm void __init_vfp11(void)
{
#define	VFPEnable 0x40000000
#define RF_Enable 0x03c00000

	MRC p15, 0, r1, c1, c0, 2 //  r1 = Access Control Register
	ORR r1, r1, #(0xf << 20)  //  enable full access for p10,11
	MCR p15, 0, r1, c1, c0, 2 // Access Control Register = r1
	MOV r1, #0
	MCR p15, 0, r1, c7, c5, 4 // flush prefetch buffer because of FMXR below
	// and CP 10 & 11 were only just enabled

	// Enable VFP itself
	MOV r0,#VFPEnable
	FMXR FPEXC, r0 // FPEXC = r0

	// Enable "Run Fast" Mode and round toward zero
	MOV r0,#RF_Enable
	FMXR FPSCR, r0
	BX LR
}
#endif

__declspec(weak) asm void __init_hardware(void)
{
#ifdef EWL_VFP11
	b	__init_vfp11
#endif
	mov pc,lr
	
}

__declspec(weak) asm void __init_user(void)
{
	mov pc,lr
	
}

