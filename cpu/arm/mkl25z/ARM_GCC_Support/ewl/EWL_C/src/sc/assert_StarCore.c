/* *** Copyright Freescale Semiconductor, Inc, 2005 *** */ 
#include <stdio.h>
#include <stdlib.h>

#include <ewl_misra_types.h>

void __nonopt_cw_assert ( int_t line_number, int_t file_number );
void __nonopt_cw_assert ( int_t line_number, int_t file_number )
{
	(void)printf ( "cw_assert() fault line %d in the file of number %d\n", line_number, file_number );
	(void)printf ( "Program aborted ... \n" );
	MISRA_EXCEPTION_RULE_20_11()
	exit ( -1 );
}
