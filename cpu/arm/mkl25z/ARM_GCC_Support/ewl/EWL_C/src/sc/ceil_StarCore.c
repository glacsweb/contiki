/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

#include <math.h>

#ifdef ceil
#undef ceil
#endif

f64_t
ceil ( f64_t x )
{
    f64_t y;
    return ((modf ( x, & y ) > 0.0) ? (y + 1.0) : y );
}

