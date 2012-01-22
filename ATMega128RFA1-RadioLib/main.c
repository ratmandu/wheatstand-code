/*
 * main.c
 *
 *  Created on: Jan 22, 2012
 *      Author: ratmandu
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "radio.c"

int main(void) {
//	uint8_t resetReason = MCUSR;
	MCUSR = 0;
	clock_prescale_set(clock_div_1);

	DDRG = (1<<PG0);
//	if (resetReason & PORF) {
//		PORTG = (1<<PG0);
//		while (1)
//			;
//	}
	cli();
	trx_radioInit();
	sei();
	while (1) {
		;
//		sei();
//		unsigned char data[2];
//		data[0] = 'T';
//		data[1] = 'E';
//		trx_send(&data, sizeof(data));
//		PORTG |= (1<<PG0);
//		_delay_ms(500);
//		PORTG &= ~(1<<PG0);
//		_delay_ms(500);
	}

	return 1;
}

ISR(BADISR_vect) {
;
}
