/*
 * main.c
 *
 *  Created on: Jan 22, 2012
 *      Author: ratmandu
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include "radio.h"
#include "adc.h"
#include "serial.h"
#include "gps.h"

int main(void) {
	clock_prescale_set(clock_div_1);

	DDRG = (1<<PG0);

	cli();
	trx_radioInit();
	initSerial0(38400);
	initSerial0RxBuf();
	adcInit();
	adcStart();
	sei();

	int i = 0;
	char temp[50];
	uint16_t adc_val = 0;

	while (1) {
			adc_val = adcGetLastValue(0);
			sprintf(temp, "ADC %i: %04u\r", i, adc_val);
			serial0PutString((unsigned char*)temp);

		_delay_ms(3);
	}

	return 1;
}

ISR(BADISR_vect) {
;
}
