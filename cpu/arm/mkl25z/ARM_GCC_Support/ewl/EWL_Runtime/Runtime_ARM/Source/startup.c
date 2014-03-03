/* Metrowerks ARM Runtime Support Library
 * Copyright © 1995-2002 Metrowerks Corporation. All rights reserved.
 *
 * $Date: 2011/09/21 06:41:34 $
 * $Revision: 1.4 $
 */


/*
 *	startup.c	-	entry-point for ARM programs
 *
 */
#include <string.h>
#include <stdlib.h>
#include "Runtime_configuration.h"

#if SUPPORT_SEMIHOST_ARGC_ARGV
#define __MAX_CMDLINE_ARGS 10
static char *argv[__MAX_CMDLINE_ARGS] = { 0 };
#else
static char *argv[] = { 0 };
#endif

extern int __argc_argv(char **, int);
extern char __SP_INIT[];
extern void __call_static_initializers(void);
extern int main(int, char **);

extern void __FSL_fp_init(void);

extern asm void __init_registers();
extern asm void __init_hardware();
extern asm void __init_user();

#if defined(__APCS_ROPI)
extern void __init_pic();
#endif

#if defined(__APCS_RWPI)
extern void __init_pid();
#endif

#if defined(__APCS_ROPI) || defined(__APCS_RWPI)
extern void __load_static_base();
#endif

#if defined(__SEMIHOSTING)
extern void __init_semihost(void);
#endif

#if SUPPORT_ROM_TO_RAM
extern void __copy_rom_sections_to_ram(void);
extern char __S_romp[];
#endif

static void zero_fill_bss(void)
{
	extern char __START_BSS[];
	extern char __END_BSS[];

	memset(__START_BSS, 0, (__END_BSS - __START_BSS));
}

#ifdef __thumb // Thumb version
// To keep iar debugger happy
void __iar_program_start(void);
void __thumb_startup(void);
void __iar_program_start()
{
	__thumb_startup();
}

/*#ifdef __VFPV4__
asm void __init_vfpv4(void)
{
#define	VFPEnable 0x00F00000
#define RF_Enable 0x000000
#define Flush0_Enable	0x1000000


	MOVW r1,#60808
	MOVT r1,#57344
	LDR r0,[r1]
	ORR.W r0,r0,#VFPEnable
	STR r0, [r1]
	MOV.W r0,#RF_Enable
//#ifdef IEEE_STD
	//ORR.W r0,r0,#Flush0_Enable
//#else
	ORR.W r0,r0,#0x02000000
//#endif
	VMSR FPSCR, r0
	BX LR
}
#endif
*/
asm void __thumb_startup(void)
{
		// Setup registers
		bl 	__init_registers

		// setup hardware
		bl 	__init_hardware

		//setup floating-point unit
//#ifdef __VFPV4__
		//bl	__init_vfpv4
//#endif

		//	setup the stack before we attempt anything else
		lda	r0,__SP_INIT
		// skip stack setup if __SP_INIT is 0
		// assume sp is already setup.
		cmp	r0,#0
		beq	skip_sp
		mov	sp,r0
		sub	sp,#4
		mov	r0,#0
		mvn	r0,r0
		str	r0,[sp,#0]
		add	sp,#4
skip_sp:



#if defined(__APCS_ROPI) || defined(__APCS_RWPI)
		//	static base register initialization
		bl	__load_static_base
#endif
#if defined(__APCS_RWPI)
		//	-pid
		//	setup static base for SB relative position independent data
		//	perform runtime relocation
		bl	__init_pid
#endif
#if defined(__APCS_ROPI)
		//	-pic
		//	perform runtime relocation for position independent code
		bl	__init_pic
#endif
		//	zero-fill the .bss section
		bl	zero_fill_bss

#if SUPPORT_ROM_TO_RAM
		lda 	r0, __S_romp
		cmp	r0,#0
		beq	over
		bl	__copy_rom_sections_to_ram
over:
#endif


		//      initialize the floating-point library
#ifdef __VFPV4__
		bl      __FSL_fp_init
#endif


		//	call C++ static initializers
		bl	__call_static_initializers

		// initializations before main, user specific
		bl 	__init_user

#if defined(__SEMIHOSTING)
		// semihost initializations
		bl	__init_semihost
#endif


		//	call main(argc, &argv)
#if SUPPORT_SEMIHOST_ARGC_ARGV
		lda	r1,argv
		mov	r0,#__MAX_CMDLINE_ARGS	// size of argv array
		bl	__argc_argv
		lda	r1,argv	// reload r1 w/ addr of argv
		// r0 is correct w/ rtn from argc_argv
#else
		mov	a1,#0
		lda	a2,argv
#endif
		bl	main

		//	exit the program
		mov	a1,#0
		bl	exit

		//	should never get here
@1:		b	@1

}
#endif

#pragma thumb off
void __startup(void);
asm void __startup(void)
{
#ifndef __thumb // ARM version

		// Setup registers
		bl 	__init_registers


		// setup hardware
		bl	 __init_hardware

		// ----- access to memory is save now

		//	setup the stack before we attempt anything else
		lda	sp,__SP_INIT
		// mtwx14319 - skip stack setup if __SP_INIT is 0
		// assume sp is already setup.
		cmp	sp,#0
		beq	skip_sp
		mvn	r0,#0
		str	r0,[sp,#-4]!
skip_sp:


		// ----- stack is set up, you can call C functions now

		//	zero-fill the .bss section
		bl	zero_fill_bss

#if SUPPORT_ROM_TO_RAM
		lda 	r0, __S_romp
		cmp	r0,#0
		blne	__copy_rom_sections_to_ram
#endif


		//      initialize the floating-point library
		bl      _fp_init

		//	call C++ static initializers
		bl	__call_static_initializers

		// initializations before main, user specific
		bl 	__init_user

		//	call main(argc, &argv)

#if SUPPORT_SEMIHOST_ARGC_ARGV
		lda		r1,argv
		mov		r0,#__MAX_CMDLINE_ARGS	// size of argv array
		bl		__argc_argv
		lda		r1,argv	// reload r1 w/ addr of argv
						// r0 is correct w/ rtn from argc_argv
#else
		mov		a1,#0
		lda		a2,argv
#endif
		bl		main

		//	exit the program
		mov		a1,#0
		bl		exit

		//	should never get here
@1:		b		@1

#else // Thumb version

		//	we cannot start in Thumb mode, so call __thumb_startup (but not
		//	for Cortex-M3 (ARMv7), call __thumb_startup directly)
		//	via interworking branch
		lda		r0,__thumb_startup
		bx 		r0

#endif
}
#pragma thumb reset

