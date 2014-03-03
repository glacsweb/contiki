/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */

#include <signal.h>
#include <errno.h>
#include <stdlib.h>

#if defined(_REENTRANT) && _REENTRANT
#include "critical_regions.h"
#include "context_local_data.h"
#endif

#include <ewl_misra_types.h>

#if STD_SIGNALs == 6
static void (*signal_table[STD_SIGNALs])(int_t sig)={SIG_DFL,SIG_DFL,SIG_DFL,SIG_DFL,SIG_DFL,SIG_DFL};
#else
 // If someone increases MAX_SIGNAL, he/she should fill in the initilizer table
 // and modify the about condition in #if  , otherwise signal_table is not defined
#endif

int16_t __sig_drop_count = 0;   /* the number of signals that have been
                                 handled by SIG_DFL.   */
#define FIRST_VALID_VECTOR     0x0   /* Change this to 0x200 to make SIGTRAP go to interrupt 8 */
#define INT_VECTOR_ENTRY_SIZE  64
#define JMP_OPCODE             0x3104u
#define JSR_OPCODE             0x3304u
#define RTE_OPCODE             0x9f73u
#define NOP_OPCODE             0x90c0u

#ifdef __STARCORE__

char_t *Get_VBA ( void );
#if _lint
char_t *Get_VBA ( void ) { return 0L; }
#else
asm char_t *Get_VBA ( void )
{
/*#pragma noinline */
asm_header
  return in $r0;
  .reg $d0;
asm_body
;PRAGMA stack_effect _Get_VBA,0
  move.l	vba,d0
  move.l	d0,r0
  ; move.l	0,r0
  nop
asm_end
}
#endif
#else
static __inline__ char_t *Get_VBA ( )
{
    char_t *Rval;
    __asm__ ( "move.l vba,%0" : "=d" ( Rval ));
    return Rval;
}
#endif

/************************************************************************
 *
 * These functions encode and decode the 'jmp addr' instruction
 * into/from the address pointed by ptr.
 *
 * jmp encoding (old way/buggy):   jmp AAAAAAAAAAAAAAAAaaaaaaaaaaaaaaaa
 *        0011 0001 AAAa a100   001A AAAA AAAA AAAA    10aa aaaa aaaa aaaa
 *
 * jsr encoding:   jsr AAAAAAAAAAAAAAAAaaaaaaaaaaaaaaaa
 *        0011 0011 AAAa a100   001A AAAA AAAA AAAA    10aa aaaa aaaa aaaa
 * rte encoding:   1001 1111 0111 0011
 ************************************************************************/

static void encode ( int_t address, uint16_t *ptr )
{
  uint16_t lsb = (uint16_t)((uint32_t)address & 0xffffu);
  uint16_t msb = (uint16_t)((uint32_t)address >> 16);

  *ptr++ = ((lsb & 0xe000u)>>8) | ((msb & 0xc000u)>>11) | JSR_OPCODE ;
  *ptr++ = (lsb & 0x1fffu) | 0x2000u ;
  *ptr++ = (msb & 0x3fffu) | 0x8000u ;
  *ptr   = RTE_OPCODE;
}

static int_t decode ( const uint16_t *ptr )
{
  uint16_t lsb, msb;
  uint16_t word1, word2, word3;

  word1 = *ptr++;
  word2 = *ptr++;
  word3 = *ptr++;

  msb = (uint16_t)((word1 & 0x18u)<<11) | (word3 & 0x3fffu);
  lsb = (uint16_t)((word1 & 0xe0u)<<8 ) | (word2 & 0x1fffu);

  /* return (((int)msb)<<16) | ((int)lsb); */
  MISRA_EXCEPTION_RULE_10_3()
  return (int_t)((uint_t)(((uint_t)msb)<<16) | (uint_t)(lsb&0x0FFFFu));
}

/************************************************************************
 * Function intvec_signal	ISO 7.7.1.1
 *
 * void (*intvec_signal( int sig, void (*func)( int ))) ( int x)
 *
 * Parameters:
 *  sig		Signal handler to be set
 *  func	Function to be called on occurrence
 *
 * Effect:
 *	This procedure sets the handler for the specified signal.
 *  It returns the previous handler, or SIG_ERR if no handler can be set.
 *
 ************************************************************************/

MISRA_EXCEPTION_RULE_11_3()
static void ( *intvec_signal ( int_t sig, void (*func)( int_t sig ))) ( int_t x)
{
  int_t Original_Handler;

  if ((sig < 0) || (sig >= SC100_SIGNALs)) {
	MISRA_EXCEPTION_RULE_20_5()
	errno = EINVAL;
	return __sig_err;
  } else {
    uint16_t *VecAddress = (uint16_t *)(Get_VBA() + (sig*INT_VECTOR_ENTRY_SIZE) + FIRST_VALID_VECTOR);
    uint16_t JmpOpCode;

#if defined(_REENTRANT) && _REENTRANT
    __begin_critical_region ( signal_funcs_access );
#endif
    JmpOpCode = VecAddress[0] & 0xff07u;
    if ( ((JmpOpCode != JMP_OPCODE) || ((VecAddress[3])!= NOP_OPCODE))
       &&((JmpOpCode != JSR_OPCODE) || ((VecAddress[3])!= RTE_OPCODE)) )
    {
        /* This is the case where the user has placed an inline
           interrupt handler into the interrupt vector. We can
           not install a new handler. */
		MISRA_EXCEPTION_RULE_20_5()
    	errno = EINLHAN;
#if defined(_REENTRANT) && _REENTRANT
      __end_critical_region(signal_funcs_access);
#endif
      return __sig_err;
    } else {
      Original_Handler = decode(VecAddress);
      encode((int_t)func, VecAddress);

      if (decode(VecAddress) != (int_t)func) {
          /* Installation has failed. Probably vector table
             is located in ROM   */
		MISRA_EXCEPTION_RULE_20_5()
    	  errno = EROMVEC;
#if defined(_REENTRANT) && _REENTRANT
        __end_critical_region(signal_funcs_access);
#endif
        return __sig_err;
      } else {
#if defined(_REENTRANT) && _REENTRANT
      	__end_critical_region(signal_funcs_access);
#endif
        return (void (*)(int_t sig))Original_Handler;
      }
    }
  }
}

/************************************************************************
 * Function intvec_raise	ISO 7.7.2.1
 *
 * int intvec_raise( int sig )
 *
 * Parameters:
 *  sig		Signal to be raised (in d0)
 *
 * Effect:
 *	The specified signal is raised; the current handler is called.
 * If the handler returns, this function returns 0.  If no handler can
 * be called, we return nonzero.
 ************************************************************************/

static int_t intvec_raise( int_t sig )
{
  if ((sig < 0) || (sig >= SC100_SIGNALs)) {
    return -1;
  }	else {
    int16_t *VecAddress = (int16_t *)(Get_VBA() + (sig*INT_VECTOR_ENTRY_SIZE) + FIRST_VALID_VECTOR);
    ((void (*)(int_t sig))VecAddress)(sig);
    return 0;
  }
}


/************************************************************************
 * Function std_signal	ISO 7.7.1.1
 *
 * void (*std_signal( int sig, void (*func)( int ))) ( int x)
 *
 * Parameters:
 *  sig		Signal handler to be set
 *  func	Function to be called on occurrence
 *
 * Effect:
 *	This procedure sets the handler for the specified signal.
 *  It returns the previous handler, or SIG_ERR if no handler can be set.
 *
 ************************************************************************/

static void ( *std_signal ( int_t sig, void (*func)(int_t sig))) (int_t x)
{
  void (*Original_Handler)(int_t sig);

  if ((sig < 0) || (sig >= STD_SIGNALs)) {
	  MISRA_EXCEPTION_RULE_20_5()
	  errno = EINVAL;
      return __sig_err;
  } else {
#if defined(_REENTRANT) && _REENTRANT
    __begin_critical_region ( signal_funcs_access );
#endif

      Original_Handler = signal_table[sig];
      signal_table[sig] = func;

#if defined(_REENTRANT) && _REENTRANT
      	__end_critical_region ( signal_funcs_access );
#endif
   }
   return Original_Handler;
}

/************************************************************************
 * Function std_raise	ISO 7.7.2.1
 *
 * int std_raise( int sig )
 *
 * Parameters:
 *  sig		Signal to be raised (in d0)
 *
 * Effect:
 *	The specified signal is raised; the current handler is called.
 * If the handler returns, this function returns 0.  If no handler can
 * be called, we return nonzero.
 ************************************************************************/

static int_t std_raise( int_t sig )
{
  if ((sig < 0) || (sig >= STD_SIGNALs)) {
    return -1;
  } else {
    void (*VecAddress )(int_t sig) = signal_table[sig];
    VecAddress(sig+SC100_SIGNALs);
    return 0;
  }
}

/* Wrapper functions signal& raise, to call for std_signal/raise or intvec_signal/raise */
void ( *signal ( int_t sig, void (*func)( int_t sig ))) ( int_t x){
   if (sig<SC100_SIGNALs) {
      return intvec_signal(sig,func);
   } else {
      return std_signal(sig-SC100_SIGNALs,func);
   }
}

int_t raise(int_t sig)
{
   if (sig<SC100_SIGNALs) {
      return intvec_raise(sig);
   } else {
      return std_raise(sig-SC100_SIGNALs);
   }
}

void __sig_dfl ( int_t sig )
{
	#pragma unused(sig)
	MISRA_QUIET_UNUSED_ARGS()
	__sig_drop_count++;
}

/************************************************************************
  SIG_ERR:

  we simply goto abort.
 ************************************************************************/

void __sig_err ( int_t sig )
{
	#pragma unused(sig)
	MISRA_QUIET_UNUSED_ARGS()
	MISRA_EXCEPTION_RULE_20_11()
	abort();
}

/************************************************************************
  SIG_IGN:

  just return.
 ************************************************************************/

void __sig_ign ( int_t sig )
{
	#pragma unused(sig)
	MISRA_QUIET_UNUSED_ARGS()
}

