/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

#include <math.h>
#include <limits.h>

#ifdef modf
#undef modf
#endif

f64_t
modf( f64_t value, f64_t *iptr )
{
#ifndef _CW_X_SLLD_
	/* Old version */
   	int_t temp;	
    MISRA_EXCEPTION_RULE_12_7()
    if ((((f64_t) INT_MAX ) < value ) || (((f64_t) INT_MIN ) > value )) {
        *iptr = value;
        return 0.0;
    }
    
    temp = (int_t) value;
	value -= (f64_t)temp;

	*iptr = (f64_t) temp;
    return value;
    
#else    
    /* version for double and long long support */
    
    int64_t temp;
    if ((((f64_t)LONGLONG_MAX)<value)||(((f64_t)LONGLONG_MIN )>value)) {
		*iptr = value;
		return 0.0;
    }
    temp = (int64_t)value;
	value -= temp;
	*iptr = (f64_t) temp;
    return value;
    
#endif
}
