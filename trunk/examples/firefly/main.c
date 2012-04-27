/*
 * main.c
 *
 *  Created on: Apr 26, 2012
 *      Author: brimstone
 */

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/power.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <radio.h>
#include <serial.h>
#include <stdlib.h>

// has to be a pointer so it's "global"
volatile uint8_t flag;
volatile uint8_t * pflag = &flag;

int main(void) {
	/*flag = malloc(sizeof(*flag));*/
	// need this for setup
	clock_prescale_set(clock_div_1);

	// Setup our serial port
	initSerial0(115200);
	initSerial0RxBuf();

	// setup our onboard led
	DDRG = 0;
	DDRG |= (1<<PG0);

	// setup our radio
	trx_radioInit();

	// turn on interrupts
	sei();

	// boot blink
	PORTG |= (1<<PG0);
	_delay_ms(500);
	PORTG &= ~(1<<PG0);

	while (1) {
	// print a little motd
		serial0PutString((unsigned char *)"Waiting to see if we're not master\r\n");
		flag = 1;
		_delay_ms(2000);
		if (flag == 1)
			break;
		serial0PutString((unsigned char *)"We're apparently in slave mode, so sleep for a minute or two\r\n");
		_delay_ms(20000);
	}
	serial0PutString((unsigned char *)"We're master!\r\n");
	while(1) {
		// we blink
		PORTG |= (1<<PG0);
		_delay_ms(100);
		PORTG &= ~(1<<PG0);
		// delay
		_delay_ms(100);
		// everyone blinks
		trx_send((void *)"Blink!", 6);
		PORTG |= (1<<PG0);
		_delay_ms(200);
		PORTG &= ~(1<<PG0);
		// sleep
		_delay_ms(600);
	}
	// if we ever get here, something bad happened
	return 1;
}

void receive_radio(uint8_t* frameData, uint8_t frameLength) {
	// put your own frame decoding/data handling function here
	serial0PutString((unsigned char *)frameData);
	serial0PutString((unsigned char *)"\r\n");
	*pflag = 2;
	PORTG |= (1<<PG0);
	_delay_ms(200);
	PORTG &= ~(1<<PG0);
}
