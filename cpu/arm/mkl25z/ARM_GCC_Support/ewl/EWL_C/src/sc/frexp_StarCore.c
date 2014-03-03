/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 

#include <math.h>

/*************************************************************************
 Function Name:  frexp
 Description:    Splits a fractional type value into a fraction f
                 and an exponent n, such that either f=0.0 or
                 0.5<=|f|<1.0, and f*(2**n)=x
 Parameters:     x - fractional value
                 exp - an interger pointer to store the exponent.
**************************************************************************/

f64_t frexp(f64_t x, int_t *xp)
{
  *xp = 0;
  if ( x == 0.0 ) { return 0.0; }

  while ( (x >= 1.0) || (x <= -1.0) ) {
    (*xp)++;
    x = x/2.0;
  }

  while ( (x < 0.5) && (x > -0.5) ) {
    (*xp)--;
    x = x*2.0;
  }

  return x;
}

