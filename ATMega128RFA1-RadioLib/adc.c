/*
 * adc.c
 *
 *  Created on: Jan 28, 2012
 *      Author: ratmandu
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t adcValues[9];
uint8_t  currentADC = 0;

void adcInit() {
	ADCSRA = (1<<ADPS1) | (1<<ADPS2);
	int i;
	for (i=0; i<9; i++) {
		adcValues[i] = 0;
	}
}

void adcSelect(uint8_t adcPin) {
	if (adcPin < 8) {
		ADMUX |= (adcPin & 0x1F);
		ADCSRB &= ~(1<<MUX5);
	} else if (adcPin == 8) {
		// set to temperature sensor
		ADMUX |= 9;
		ADCSRB |= (1<<MUX5);
	}
	currentADC = adcPin;
}

void adcStart() {

}

void adcStop() {

}

ISR(ADC_vect) {
	adcValues[currentADC] = (ADCH << 8);
	adcValues[currentADC] |= (ADCL);
	adcSelect((currentADC+1)%9);
	adcStart();
}
