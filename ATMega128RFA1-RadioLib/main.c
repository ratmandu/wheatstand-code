/*
 * main.c
 *
 *  Created on: Jan 22, 2012
 *      Author: ratmandu
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/io.h>
#include <string.h>
#include "radio.c"


int main(void) {
	clock_prescale_set(clock_div_1);

	DDRG = (1<<PG0);

	cli();
	trx_radioInit();
	sei();

	while (1) {
		;
		sei();
		unsigned char data[2];
		data[0] = 'T';
		data[1] = 'E';
		trx_send(&data, sizeof(data));
		PORTG |= (1<<PG0);
		_delay_ms(500);
		PORTG &= ~(1<<PG0);
		_delay_ms(500);
	}

	return 1;
}

ISR(BADISR_vect) {
;
}
