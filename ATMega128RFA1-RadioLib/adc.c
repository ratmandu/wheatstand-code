/*
 * adc.c
 *
 *  Created on: Jan 28, 2012
 *      Author: ratmandu
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

void adcInit() {
	ADCSRA = (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE) | (1<<ADEN);
	ADMUX = (1<<REFS0);// | (1<<ADLAR);
	int i;
	for (i=0; i<9; i++) {
		adcValues[i] = 0;
	}
	adcSelect(0);
}

void adcSelect(uint8_t adcPin) {
	if (adcPin >= 9) {
		adcPin = 0;
	}

	ADMUX &= ~(1<<MUX0) | ~(1<<MUX1) | ~(1<<MUX2) | ~(1<<MUX3) | ~(1<<MUX4);

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
	ADCSRA |= (1<<ADSC);
}

void adcStop() {
	ADCSRA &= ~(1<<ADSC);
}

uint16_t adcGetLastValue(uint8_t pin) {
	return adcValues[pin];
}

ISR(ADC_vect) {
//	adcValues[currentADC] = (uint16_t)(ADCL);
//	adcValues[currentADC] |= (uint16_t)(ADCH << 8) & 0x03;
	adcValues[currentADC] = ADC;
	adcSelect(0);
	adcStart();
}
