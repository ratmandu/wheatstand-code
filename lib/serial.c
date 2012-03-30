/*
 * serial.c
 *
 *  Created on: Jan 26, 2012
 *      Author: ratmandu
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "serial.h"

void initSerial0(uint32_t baud) {
	uint32_t baudreg;

	baudreg = (F_CPU/baud/16-1);
	UBRR0H = (uint8_t)((baudreg >> 8) & 0xFF);
	UBRR0L = (uint8_t)(baudreg & 0xFF);
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) | (0<<TXCIE0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void initSerial1(uint32_t baud) {
	uint32_t baudreg;

	baudreg = (F_CPU/baud/16-1);
	UBRR1H = (uint8_t)((baudreg >> 8) & 0xFF);
	UBRR1L = (uint8_t)(baudreg & 0xFF);
	UCSR1B = (1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1) | (1<<TXCIE1);
	UCSR1C = (1<<UCSZ00) | (1<<UCSZ01);
}

void serial0PutChar(unsigned char data) {
	while (!(UCSR0A&(1<<UDRE0)));
	UDR0 = data;
}

void serial1PutChar(unsigned char data) {
	while (!(UCSR1A&(1<<UDRE0)));
	UDR1 = data;
}

void serial0PutString(unsigned char *str) {
	int i=0;
	while (str[i] != '\0') {
		serial0PutChar(str[i++]);
	}
}

void serial1PutString(unsigned char *str) {
	int i=0;
	while (str[i] != '\0') {
		serial1PutChar(str[i++]);
	}
}

void initSerial0RxBuf() {
	memset(&serial0RxBuf, 0, SERIAL_0_RX_BUF_LEN);
	bytesInSerial0RxBuf = 0;
	buffer0ReadLoc = 0;
	buffer0WriteLoc = 0;
}

void initSerial1RxBuf() {
	memset(&serial1RxBuf, 0, SERIAL_1_RX_BUF_LEN);
	bytesInSerial1RxBuf = 0;
	buffer1ReadLoc = 0;
	buffer1WriteLoc = 0;
}

void writeToBuffer0(unsigned char data) {
	uint8_t temp = (buffer0WriteLoc + 1) % SERIAL_0_RX_BUF_LEN;
	if (temp != buffer0ReadLoc) {
		serial0RxBuf[buffer0WriteLoc] = data;
		buffer0WriteLoc = temp;
		bytesInSerial0RxBuf++;
	}
}

void writeToBuffer1(unsigned char data) {
	uint8_t temp = (buffer1WriteLoc + 1) % SERIAL_1_RX_BUF_LEN;
	if (temp != buffer1ReadLoc) {
		serial1RxBuf[buffer1WriteLoc] = data;
		buffer0WriteLoc = temp;
		bytesInSerial1RxBuf++;
	}
}

unsigned char readFromBuffer0(uint8_t *bufsize) {
	if (buffer0ReadLoc == buffer0WriteLoc) {
		// buffer is empty
		*bufsize = 0;
		return '\0';
	} else {
		*bufsize = (SERIAL_0_RX_BUF_LEN + buffer0WriteLoc - buffer0ReadLoc) % SERIAL_0_RX_BUF_LEN;
		unsigned char data = serial0RxBuf[buffer0ReadLoc];
		buffer0ReadLoc = (buffer0ReadLoc+1) % SERIAL_0_RX_BUF_LEN;
		return data;
	}
}

unsigned char readFromBuffer1(uint8_t *bufsize) {
	if (buffer1ReadLoc == buffer1WriteLoc) {
		// buffer is empty
		*bufsize = 0;
		return '\0';
	} else {
		*bufsize = (SERIAL_1_RX_BUF_LEN + buffer1WriteLoc - buffer1ReadLoc) % SERIAL_1_RX_BUF_LEN;
		unsigned char data = serial1RxBuf[buffer1ReadLoc];
		buffer1ReadLoc = (buffer1ReadLoc+1) % SERIAL_1_RX_BUF_LEN;
		return data;
	}
}

void buf0SkipTo(unsigned char data) {
	unsigned char b,d = 0;
	do {
		d = readFromBuffer0(&b);
	} while (b == 0 || d != data);
}

void buf1SkipTo(unsigned char data) {
	unsigned char b,d = 0;
	do {
		d = readFromBuffer1(&b);
	} while (b == 0 || d != data);
}

uint8_t buf0Size() {
	return (SERIAL_0_RX_BUF_LEN + buffer0WriteLoc - buffer0ReadLoc) % SERIAL_0_RX_BUF_LEN;
}

uint8_t buf1Size() {
	return (SERIAL_1_RX_BUF_LEN + buffer1WriteLoc - buffer1ReadLoc) % SERIAL_1_RX_BUF_LEN;
}

ISR(USART0_RX_vect) {
	writeToBuffer0(UDR0);
}

ISR(USART1_RX_vect) {
	writeToBuffer1(UDR1);
}
