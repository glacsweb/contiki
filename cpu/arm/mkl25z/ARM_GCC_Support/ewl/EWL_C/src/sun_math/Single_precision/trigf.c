/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/05/14 16:57:17 $
 * $Revision: 1.1 $
 */
 
// fast sin and cos

#include <math.h>

#if _EWL_C99 || defined(_EWL_USES_SP_MATH_LIB)

#define pi 3.141592653589793f

float __sinpoly(float x);
float __cospoly(float x);

float __sinpoly(float x)
{
	float xsq;
	
	xsq = x*x;
	
	return (((-0.35950439e-4f * xsq + 0.2490001007e-2f) * xsq - 0.8074543253e-1f) * xsq + 0.7853981633f)*x;
		   
}


float __cospoly(float x)
{
	float xsq;
	xsq = x*x;
	
	return ((-0.31872783e-3f * xsq + 0.1584968416e-1f) * xsq - 0.30842416558f) * xsq + 0.9999999673f;
}

#if !__BUILD_FDLIBM

float sinf(float x)
{
	float y, z;
	int n;
	
	// Handle negative arguments
	
	if (*(_INT32 *)&x < 0)
	{
	    return -sinf(-x);
	}
	
    // get the remainder when dividing by pi / 4
    
    y = x * (float)(4.0 / pi);
    // get truncated integer

	n = y;
	
    y = y - (float)n;
    
#if 0

    // handle base on which octant
    
    switch(n & 7)
    {
    case 0:
    	return __sinpoly(y);
    	
    case 1:
    	return __cospoly(1.0f - y);
    	
    case 2:
    	return __cospoly(y);
    	
    case 3:
    	return __sinpoly(1.0f - y);
    	
    case 4:
    	return -__sinpoly(y);
    	
    case 5:
        return -__cospoly(1.0f - y);
        
    case 6:
        return -__cospoly(y);
        
    case 7:
        return -__sinpoly(1.0f - y);
    }

#else
	if (n & 1)
	{
	    y = 1.0f - y;
	}
	if ((n+1)&2)
	{
	    z = __cospoly(y);
	}
	else
	{
		z = __sinpoly(y);
	}
	if ((n & 7) > 3)
	{
	    z = -z;
	}
	return z;
	
#endif
}


float cosf(float x)
{
	float y, z;
	int n;
	
	// Handle negative arguments
	#if __MIPS__
		x = fabs(x);
	#else	
	if (*(_INT32 *)&x < 0)
	{
	    return cosf(-x);
	}
	#endif
	
	
    // get the remainder when dividing by pi / 4
    
    y = x * (float)(4.0 / pi);
    
    // get truncated integer

	n = y;
	
    y = y - (float)n;
            
    // handle base on which octant
    
#if 0

    switch(n & 7)
    {
    case 0:
    	return __cospoly(y);
    	
    case 1:
    	return __sinpoly(1.0f - y);
    	
    case 2:
    	return -__sinpoly(y);
    	
    case 3:
        return -__cospoly(1.0f - y);
        
    case 4:
        return -__cospoly(y);
        
    case 5:
        return -__sinpoly(1.0f - y);
        
    case 6:
    	return __sinpoly(y);
    	
    case 7:
    	return __cospoly(1.0f - y);
    	
    }

#else
	if (n & 1)
	{
	    y = 1.0f - y;
	}
	if ((n+1)&2)
	{
	    z = __sinpoly(y);
	}
	else
	{
		z = __cospoly(y);
	}
	if (((n+2) & 7) > 3)
	{
	    z = -z;
	}

	return z;
#endif
}

#endif /*!__BUILD_FDLIBM*/
#endif