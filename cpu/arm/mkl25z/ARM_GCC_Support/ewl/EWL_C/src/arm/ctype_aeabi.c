/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2010/05/31 07:50:37 $
 * $Revision: 1.5 $
 */

#include <ansi_parms.h>
#include <ewl_misra_types.h>

_MISRA_EXCEPTION_RULE_19_6()
#undef	_EWL_USE_INLINE
#define	_EWL_USE_INLINE 0
#undef	_EWL_INLINE
MISRA_QUIET_UNUSED_MACRO()
#define	_EWL_INLINE 

#include <ctype.h>

#ifndef _AEABI_PORTABLE
# error "AEABI not supported by header.h"
#endif

uchar_t const __aeabi_ctype_table_C[__aeabi_cmap_size] =
	{/* Control characters 0-31*/
	/*0,1,2,3,4*/				__C,__C,__C,__C,__C,

 	/*5,6,7,8,9*/				__C,__C,__C,__C,0x41,

 	/*10,11,12,13,14*/			0x41,0x41,0x41,0x41,__C,

 	/*15,16,17,18,19*/			__C,__C,__C,__C,__C,

 	/*20,21,22,23,24*/			__C,__C,__C,__C,__C,

 	/*25,26,27,28,29*/			__C,__C,__C,__C,__C,

 	/*30,31,32,33,34*/			__C,__C,0x5,__P,__P,

 	/*35,36,37,38,39*/			__P,__P,__P,__P,__P,

 	/*40,41,42,43,44*/			__P,__P,__P,__P,__P,

 	/*45,46,47,48,49*/			__P,__P,__P,DIGIX,DIGIX,

 	/*50,51,52,53,54*/			DIGIX,DIGIX,DIGIX,DIGIX,DIGIX,

 	/*55,56,57,58,59*/			DIGIX,DIGIX,DIGIX,__P,__P,

 	/*60,61,62,63,64*/			__P,__P,__P,__P,__P,

 	/*65,66,67,68,69*/			UPPCX,UPPCX,UPPCX,UPPCX,UPPCX,

 	/*70,71,72,73,74*/			UPPCX,__U,__U,__U,__U,

 	/*75,76,77,78,79*/			__U,__U,__U,__U,__U,

 	/*80,81,82,83,84*/			__U,__U,__U,__U,__U,

 	/*85,86,87,88,89*/			__U,__U,__U,__U,__U,

 	/*90,91,92,93,94*/			__U,__P,__P,__P,__P,

 	/*95,96,97,98,99*/			__P,__P,LOWCX,LOWCX,LOWCX,

 	/*100,101,102,103,104*/		LOWCX,LOWCX,LOWCX,__L,__L,

 	/*105,106,107,108,109*/		__L,__L,__L,__L,__L,

 	/*110,111,112,113,114*/		__L,__L,__L,__L,__L,

 	/*115,116,117,118,119*/		__L,__L,__L,__L,__L,

 	/*120,121,122,123,124*/		__L,__L,__L,__P,__P,

	/*125,126,127,128,129*/		__P,__P,__C,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0
	};

_EWL_CDECL int_t isalnum(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & (__U+__L+__N));
}

_EWL_CDECL int_t isblank(int_t c)
{
	   		return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((c == 0x9) || (__aeabi_ctype_table_C[c] & __B));
}

_EWL_CDECL int_t  isalpha(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & (__U+__L));
}

_EWL_CDECL int_t  iscntrl(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __C)	;
}

_EWL_CDECL int_t  isdigit(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __N);
}

_EWL_CDECL int_t  isgraph(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c])&(__L+__U+__N+__X+__P));
}

_EWL_CDECL int_t islower(int_t c)
{
	 return  ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __L);
}

_EWL_CDECL int_t isprint(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & (__L+__U+__N+__P+__B));
}

_EWL_CDECL int_t ispunct(int_t c)
{
	 return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __P);
}

_EWL_CDECL int_t isspace(int_t c)
{
	 return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __S);
}

_EWL_CDECL int_t isupper(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __U);
}

_EWL_CDECL int_t isxdigit(int_t c)
{
	return ((c < 0) || (c >= __aeabi_cmap_size)) ? 0:((int_t)(__aeabi_ctype_table_C[c]) & __X);
}

_EWL_CDECL int_t toupper(int_t c)
{
	return (((c < 0x61) || (c >= 0x7B)) ? c : (int_t) (c - 0x20));
}

_EWL_CDECL int_t tolower(int_t c)
{
	return (((c < 0x41) || (c >= 0x5B)) ? c : (int_t) (c + 0x20));
}



/* 	__aeabi_ctype_table_C[] array and non inline functions are added for
 * _AEABI_PORTABILITY_LEVEL macro. Refer Section 5.3 from "C Library ABI for the ARM architecture.pdf"
 */
