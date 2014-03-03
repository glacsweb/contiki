/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/02/22 09:15:38 $
 * $Revision: 1.2 $
 */

#include <critical_regions.h>

#if _EWL_THREADSAFE

#pragma data_seg_name "reserved_crt_mutex"		/* this needs to be shared amongst cores */

volatile unsigned short MUTEX_0 = 0;	//	atexit_funcs_access
volatile unsigned short MUTEX_1 = 0;	//	malloc_pool_access
volatile unsigned short MUTEX_2 = 0;	//	stdin_access
volatile unsigned short MUTEX_3 = 0;	//	stdout_access
volatile unsigned short MUTEX_4 = 0;	//	stderr_access
volatile unsigned short MUTEX_5 = 0;	//	files_access
/*volatile unsigned short MUTEX_6 = 0;	//	console_status_access - not yet used */
volatile unsigned short MUTEX_7 = 0;	//	signal_funcs_access
/*volatile unsigned short MUTEX_8 = 0;	//	thread_access - not yet used */

#define UNLOCKED 0xff
volatile unsigned short CORE_ID_MUTEX0 = UNLOCKED;
volatile unsigned short CORE_ID_MUTEX1 = UNLOCKED;
volatile unsigned short CORE_ID_MUTEX2 = UNLOCKED;
volatile unsigned short CORE_ID_MUTEX3 = UNLOCKED;
volatile unsigned short CORE_ID_MUTEX4 = UNLOCKED;
volatile unsigned short CORE_ID_MUTEX5 = UNLOCKED;
/*volatile unsigned short CORE_ID_MUTEX6 = UNLOCKED;	*/
volatile unsigned short CORE_ID_MUTEX7 = UNLOCKED;
/*volatile unsigned short CORE_ID_MUTEX8 = UNLOCKED;	*/

CRITICAL_SECTIONS __cs[num_critical_regions];


asm void lock_mutex ( unsigned short *pregion_lock, 
					  unsigned short *pcore_id ) 
{
#pragma noinline

	asm_header

		.arg
			_pregion_lock in $r0;
			_pcore_id in $r1;

		.reg $r0,$r1,$d0,$d1;

	asm_body
#if defined(_MSC8144_) || defined(_MSC8156_)
		[
		moveu.w (r1),d0			 ; get lock id
		move.l $fff06028,d1
		]
		and      #$ff0000,d1,d1
		lsrw     d1,d1			 ; get core id from MMU Platform Information Register (M_PIR)
#elif defined(_MSC8102_) || defined(_MSC8122_)
		[
		moveu.w (r1),d0			 ; get lock id
		moveu.w $F0FFF0,d1	 ; get core id from CIDR
		]
#else
		#error "Architecture-dependent code. To be added for new ones."
#endif
		cmpeq.w #$00ff,d0		 ; if unlocked
		jt l1

		cmpeq d0,d1				 ; see if this core already has the lock
		jt l2

		l1: bmtset.w #$0001,(r0) ; lock mutex
		nop   ; NOP ADDED BECAUSE OF SIMULATOR
		jt l1
		move.w d1,(r1)		; set core_id to that of this core

		l2:
	asm_end
}

void unlock_mutex ( CRITICAL_SECTIONS cs ) 
{	
	/* call this only if you have the mutex */
	
	*cs.region_lock = 0;
	*cs.core_id = UNLOCKED;
}


void __init_critical_regions ( void )
{
	__cs[0].region_lock = &MUTEX_0;
	__cs[1].region_lock = &MUTEX_1;
	__cs[2].region_lock = &MUTEX_2;
	__cs[3].region_lock = &MUTEX_3;
	__cs[4].region_lock = &MUTEX_4;
	__cs[5].region_lock = &MUTEX_5;
/*	__cs[6].region_lock = &MUTEX_6;*/
	__cs[7].region_lock = &MUTEX_7;
/*	__cs[8].region_lock = &MUTEX_8;*/

	__cs[0].core_id = &CORE_ID_MUTEX0;
	__cs[1].core_id = &CORE_ID_MUTEX1;
	__cs[2].core_id = &CORE_ID_MUTEX2;
	__cs[3].core_id = &CORE_ID_MUTEX3;
	__cs[4].core_id = &CORE_ID_MUTEX4;
	__cs[5].core_id = &CORE_ID_MUTEX5;
/*	__cs[6].core_id = &CORE_ID_MUTEX6;*/
	__cs[7].core_id = &CORE_ID_MUTEX7;
/*	__cs[8].core_id = &CORE_ID_MUTEX8;*/
}

void __kill_critical_regions ( void )
{
}

void __begin_critical_region ( int region )
{
	lock_mutex ( (unsigned short *)__cs[region].region_lock,
				 (unsigned short *)__cs[region].core_id );
}

void __end_critical_region ( int region )
{
	unlock_mutex ( __cs[region] );
}

#endif /* _EWL_THREADSAFE */
