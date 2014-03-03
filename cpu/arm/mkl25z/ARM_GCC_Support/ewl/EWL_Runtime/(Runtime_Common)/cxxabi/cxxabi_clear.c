/* Metrowerks Runtime Library
 * Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
 *
 * Purpose:  Implementation of the portable Itanium C++ ABI
 *           see http://www.codesourcery.com/cxx-abi/
 *
 * $Date: 2008/10/17 15:38:56 $
 * $Revision: 1.1 $
 */

#include <stddef.h>

extern void *__clear(void *ptr,size_t size);
extern void *__clear(void *ptr,size_t size)
{
	char *cptr;

	if((cptr=(char *)ptr)!=NULL)
	{
		for(; size!=0; --size, ++cptr)
		{
			*cptr=0;
		}
	}
	return ptr;
}
