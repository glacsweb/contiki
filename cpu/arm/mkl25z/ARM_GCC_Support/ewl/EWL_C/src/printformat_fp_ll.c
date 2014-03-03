#define __pformatter __pformatterFPLL

#define _EWL_FLOATING_POINT_IO 1
#define _EWL_FLOATING_POINT 1
#define _EWL_C99 0
#define _EWL_LONGLONG_IO 1

#ifdef __STDC_WANT_LIB_EXT1__
#undef __STDC_WANT_LIB_EXT1__
#endif

#define __STDC_WANT_LIB_EXT1__ 0

#include <printformat.c>
