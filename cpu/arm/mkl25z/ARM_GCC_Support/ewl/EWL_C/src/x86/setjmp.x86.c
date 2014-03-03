/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/12/16 01:40:09 $
 * $Revision: 1.1 $
 */

#include <setjmp.h>


extern void *__local_destructor_chain;

__declspec(naked) _EWL_IMP_EXP_C int _EWL_CDECL _Setjmp(jmp_buf /*env*/)   /*- hh 980122 -*/
{
    asm
	{

		POP		ECX					// return address to ECX
		POP		EAX					// env pointer to EAX
		
		MOV		[EAX], EBX			// save EBX in first loc
		MOV		4[EAX], ESI			// save ESI in second loc
		MOV		8[EAX], EDI			// save EDI in third loc
		MOV		12[EAX], ESP		// save ESP in fourth loc
		MOV		16[EAX], EBP		// save EBP in fifth loc
		MOV		20[EAX], ECX		// save return address in sixth loc
		
		XOR		EAX, EAX			// value is 0, indicating return from setjmp instead of longjmp
		PUSH	EAX					// leave space for the passed param
		PUSH	ECX					// put back return addres
		RETN						// and return
	}
}


__declspec(naked) _EWL_IMP_EXP_C void _EWL_CDECL longjmp(jmp_buf /*env*/, int /*value*/)   /*- hh 980122 -*/
{
    asm
	{
	    POP		ECX					// return address to ECX
		POP		EDX					// env pointer to EDX
		POP		EAX					// value to return in EAX

		MOV		EBX, [EDX]			// restore saved EBX
		MOV		ESI, 4[EDX]			// restore saved ESI
		MOV		EDI, 8[EDX]			// restore saved EDI
		MOV		ESP, 12[EDX]		// restore saved ESP
		MOV		EBP, 16[EDX]		// restore saved EBP
		
		OR		EAX, EAX
		JNE		1
		INC		EAX

		PUSH	EAX					// leave space for the pushed param
		PUSH	20[EDX]				// restore return address onto the stack
		RETN						// and go back to the setjmp caller
	}
}

/* Change record:
 * hh  980122 commented out unused arg 3 places
 * cc  011203 Added _EWL_CDECL for new name mangling 
 */
