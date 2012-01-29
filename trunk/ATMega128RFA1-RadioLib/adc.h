/*
 * adc.h
 *
 *  Created on: Jan 29, 2012
 *      Author: ratmandu
 */

#ifndef ADC_H_
#define ADC_H_

uint16_t adcValues[9];
uint8_t  currentADC = 0;

void adcInit(void);
void adcSelect(uint8_t adcPin);
void adcStart(void);
void adcStop(void);
uint16_t adcGetLastValue(uint8_t pin);
void ADC_vect(void);

#endif /* ADC_H_ */
