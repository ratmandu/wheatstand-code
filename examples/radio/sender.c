/*
 * main.c
 *
 *  Created on: Mar 30, 2012
 *      Author: brimstone
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <radio.h>

int main(void) {
	// need this for setup
	clock_prescale_set(clock_div_1);

	// setup our onboard led
	DDRG = 0;
	DDRG |= (1<<PG0);

	// setup our radio
	trx_radioInit();

	while (1) {
		// led on
		PORTG |= (1<<PG0);
		_delay_ms(500);
		trx_send((void*)"hi", 2);
		// led off
		PORTG &= ~(1<<PG0);
		_delay_ms(500);
	}
	// if we ever get here, something bad happened
	return 1;
}
