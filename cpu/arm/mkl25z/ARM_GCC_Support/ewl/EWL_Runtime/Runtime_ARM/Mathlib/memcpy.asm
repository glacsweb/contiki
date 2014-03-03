#define _AEABI 1
	
#ifdef _AEABI

	EXPORT __aeabi_memcpy
	EXPORT __aeabi_memcpy4
	EXPORT __aeabi_memcpy8

	AREA |.text|,CODE,READONLY

__aeabi_memcpy
__aeabi_memcpy4
__aeabi_memcpy8
	cmp		r2,#0
	bxeq		lr
	cmp		r2,#16
	bls		top_bytes
	eor		r3,r1,r0
	ands		r3,r3,#3
	bne		top_bytes
	push		{r4-r6}
top16
	ldmcsia		r1!,{r3-r6}
	subs		r2,#16
	stmcsia		r0!,{r3-r6}
count16
	cmp		r2,#16
	bcs		top16
	b		count4
top4
	ldr		r3,[r1],#4
	subs		r2,r2,#4
	str		r3,[r0],#4
count4
	cmp		r2,#4
	bcs		top4
	pop		{r4-r6}
	b		count_bytes
top_bytes
	ldrb		r3,[r1],#1
	subs		r2,r2,#1
	strb		r3,[r0],#1
count_bytes
	cmp		r2,#0
	bne		top_bytes
#ifdef THUMB
	bx		lr
#else
	mov		pc,lr
#endif

#endif         
	END

/* Chipwerks change record:
* April 07, 2006 Added with reference to 4.3.4 Memory copying, clearing, and setting,
* in RUN-TIME ABI FOR THE ARM ARCHITECTURE document
* Second change - Feb 17, 2012 - New routines for memcpy/4/8.
*/
