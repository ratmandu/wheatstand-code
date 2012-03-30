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
#include <stdlib.h>
#include <string.h>
#include <serial.h>

// size of our command variable
#define tmpcmdsize 150

// this read in a line, saving it back to the variable passed in and return the
// number of characters 
int readline(char * tmpcmd) {
	// keep our count
	uint8_t count = 0;
	// keep up with a buffer
	uint8_t bufSize;
	do {
		// wait for input
		PORTG |= (1<<PG0);
		while (buf0Size() < 1) { }
		PORTG &= ~(1<<PG0);
		// read in a letter
		tmpcmd[count] = readFromBuffer0(&bufSize);
		// print the letter back out
		serial0PutChar(tmpcmd[count++]);
		// TODO handle backspace, arrow keys and delete
		// TODO change 150 to either tmpcmdsize or sizeof()
	} while (count < 150 && tmpcmd[count-1] != '\r');
	// print a new line
	serial0PutString((unsigned char *)"\n");
	// remove our last char, which should be a line feed
	count--;
	tmpcmd[count] = 0;
	// return our length to our calling function
	return count;
}

int main(void) {
	// need this for setup
	clock_prescale_set(clock_div_1);

	// temp command
	char tmpcmd[tmpcmdsize];

	// Setup our serial port
	initSerial0(115200);
	initSerial0RxBuf();

	// turn on interrupts so we can read data back from serial
	sei();
	// setup our onboard led
	DDRG = 0;
	DDRG |= (1<<PG0);

	// print a little motd
	serial0PutString((unsigned char *)"Hi!\r\n");
	while (1) {
		// clear out our command variable
		memset(&tmpcmd, 0, tmpcmdsize);
		// print out a prompt for input	
		serial0PutString((unsigned char *)">");
		// read a line
		readline(tmpcmd);
		// if it's something we know
		if (strcmp(tmpcmd, "hi") == 0) {
			// act on it
			serial0PutString((unsigned char *)"Hello!\r\n");
		}
		else {
			// error
			serial0PutString((unsigned char *)"I don't know what you said.\r\n");
		}
	}
	// if we ever get here, something bad happened
	return 1;
}
