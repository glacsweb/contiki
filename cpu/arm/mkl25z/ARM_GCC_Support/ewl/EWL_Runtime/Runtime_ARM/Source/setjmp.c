
/* CodeWarrior ARM Runtime Support Library 
 * Copyright © 1995-2003 Metrowerks Corporation. All rights reserved. 
 * Copyright © 2010 Freescale Semiconductors. All rights reserved. 
 *
 * $Date: 2010/04/27 14:23:27 $
 * $Revision: 1.2 $
 */

/*
 *	setjmp.c	-	setjmp() and longjmp() routines for ARM family
 *
 *	THEORY OF OPERATION
 *
 *	The runtime support routines __setjmp() and longjmp() support the C <setjmp.h>
 *	facilities. __setjmp() captures the state of the program in a jmp_buf data structure
 *	which has the following C definition:
 *
 *		typedef long jmp_buf[16];		 // saved registers (see below for order)
 *
 *	setjmp() and longjmp() are defined as follows:
 *
 *		int __setjmp(jmp_buf env);
 *		#define setjmp(env) __setjmp(env)
 *		void longjmp(jmp_buf env, int val);
 *
 */

#include <setjmp.h>

// BC Allow this file to exist in otherwise Thumb library for now.
#if !defined (__thumb2)
#pragma thumb off
#endif
/*
 *	__setjmp	-	C setjmp() routine
 *
 *	On entry a1 points to a jmp_buf struct. On exit, a1 is 0.
 *
 */

asm int __setjmp(register jmp_buf env)
{
#if defined(__thumb2)
		stmia a1!,{v1-v8,lr}	// sp cannot be in the list
		str   sp,[a1,#0]	// sp
		mov	a1,#0
		bx 	lr
#else
		stmia	a1,{v1-v8,sp,lr}
		mov		a1,#0
		bx		lr
#endif
}


/*
 *	longjmp		-	C longjmp() routine
 *
 *	On entry a1 points to a jmp_buf struct and a2 contains the return value.
 *	On exit, a1 contains 1 if a2 was 0, otherwise it contains the value from a2.
 *
 */

asm void longjmp(register jmp_buf env, register int val)
{
#if defined(__thumb2)
		ldmia	a1!,{v1-v8,lr}
		ldr	sp,[a1,#0]
		movs    a1,a2
		it eq
		mov 	a1,#1
		bx 	lr
#else
		ldmia	a1,{v1-v8,sp,lr}
		movs	a1,a2
		moveq	a1,#1
		bx		lr
#endif
}
#if !defined (__thumb2)
#pragma thumb reset
#endif
