/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*	spi.c	
 *	
 *	SPI functions for the Freescale FRDM-KL25z Freedom Board
 *	
 *	Author: Graeme Bragg
 * 			ARM-ECS / Pervasive Systems Centre
 * 			School of Electronics & Computer Science
 * 			University of Southampton
 * 
 * 
 *	1/5/2014	Rev.01		Includes functions for initialising SPI0 for CC1120,
 *							controlling CSn and sending data.
 *
 *	Page references relate to the KL25 Sub-Family Reference Manual, Document 
 *	No. KL25P80M48SF0RM, Rev. 3 September 2012. Available on 25/02/2013 from: 
 *	http://cache.freescale.com/files/32bit/doc/ref_manual/KL25P80M48SF0RM.pdf?fr=gdc
 *	
 *	Page references to "M0 Book" refer to "The Definitive Guide to the 
 *	ARM Cortex-M0" by Joseph Yiu, ISBN 978-0-12-385477-3.
 *	
 *
 * Copyright (c) 2014, University of Southampton, Electronics and Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */				
/*-----------------------------------------------------------------------------------------------------------------------------------*/

#include <MKL25Z4.h>                   /* I/O map for MKL25Z128VLK4 */
#include "cpu.h"

#include "spi.h"

void SPI0_init(void)
{
  port_enable(PORTD_EN_MASK | PORTC_EN_MASK); 				/* Make sure that clocks are enable to Port D and Port C */
  
  
  /* Configure CSn pin on Port D, Pin 0 */
  GPIOD_PDDR |= GPIO_PDDR_PDD(0x01); 						/* Set pin as Output. */                                                  
  GPIOD_PDOR |= GPIO_PDOR_PDO(0x01);    					/* Set initialisation value on 1 */                                           

  PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;							/* Clear PCR Multiplex. */
  PORTD_PCR0 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x01);		/* Clear ISF & set MUX to be basic pin. */
  
  
  /* Configure SPI0. */
  SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;      					/* Enable clock to SPI Module */
  
  PORTD_PCR3 &= ~PORT_PCR_MUX_MASK; 						/* Clear Port D, Pin 3 Mux. */
  PORTD_PCR3 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x02);     /* Clear ISF & Set Port D, Pin 3 as MISO. */                                           
  
  PORTD_PCR2 &= ~PORT_PCR_MUX_MASK;							/* Clear Port D, Pin 2 Mux. */
  PORTD_PCR2 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x02);     /* Clear ISF & Set Port D, Pin 2 as MOSI. */                                             
  
  PORTC_PCR5 &= ~PORT_PCR_MUX_MASK;							/* Clear Port C, Pin 5 Mux. */
  PORTC_PCR5 |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x02); 	/* Clear ISF & Set Port C, Pin 5 as Clock. */
  
  SPI0_C1 = (SPI_C1_MSTR_MASK ); 							/* Set SPI0_C1: Master Mode */
  SPI0_C2 = 0x00;        									/* Set SPI0_C2: Default state */
  
  SPI0_BR = SPI_BR_SPPR(0x02);         						/* Set baud rate register */
  
  SPI0_C1 |= SPI_C1_SPE_MASK;          						/* Enable SPI module */
}


/* SPI CSn control */
void SPI0_csn_low(void)
{
	FGPIOD_PCOR = 0x01;		/* Clear CSn bit. */
}

void SPI0_csn_high(void)
{
	FGPIOD_PSOR = 0x01;		/* Set CSn bit. */
}


/* Single SPI Send/Recieve. */
uint8_t SPI_single_tx_rx(uint8_t in, uint8_t module) {
	
	bool i = TRUE;
	uint8_t tmp;
	
	taskENTER_CRITICAL();
	while(i){
		tmp = SPI0_S;
		tmp &= SPI_S_SPTEF_MASK;
		
		if (tmp == SPI_S_SPTEF_MASK) {
			SPI0_D = in;
			i = FALSE;
		}
	}
	
	i = TRUE;
	while(i) {
		tmp = SPI0_S;
		tmp &= SPI_S_SPRF_MASK;
		
		if (tmp == SPI_S_SPRF_MASK) {
			tmp = SPI0_D;
			i = FALSE;
		}
	}
	
	taskEXIT_CRITICAL();
	
	return (tmp);
}

/* Send data to SPI Address. */
uint8_t SPI_tx_and_rx(uint8_t addr, uint8_t value, uint8_t module) {

	uint8_t result;
	(void)_single_tx_rx_spi_transaction(addr); //Throw away the first received byte
	result = _single_tx_rx_spi_transaction(value);
	return result;
}