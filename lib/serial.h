/*
 * serial.h
 *
 *  Created on: Jan 29, 2012
 *      Author: ratmandu
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#define SERIAL_0_RX_BUF_LEN	256
#define SERIAL_1_RX_BUF_LEN	256

char serial0RxBuf[SERIAL_0_RX_BUF_LEN];
char serial1RxBuf[SERIAL_1_RX_BUF_LEN];
uint8_t bytesInSerial0RxBuf;
uint8_t bytesInSerial1RxBuf;
uint8_t buffer0ReadLoc, buffer0WriteLoc;
uint8_t buffer1ReadLoc, buffer1WriteLoc;

void initSerial0(uint32_t baud);
void initSerial1(uint32_t baud);
void serial0PutChar(unsigned char data);
void serial1PutChar(unsigned char data);
void serial0PutString(unsigned char *str);
void serial1PutString(unsigned char *str);
void initSerial0RxBuf(void);
void initSerial1RxBuf(void);
void writeToBuffer0(unsigned char data);
void writeToBuffer1(unsigned char data);
unsigned char readFromBuffer0(uint8_t *bufsize);
unsigned char readFromBuffer1(uint8_t *bufsize);
void buf0SkipTo(unsigned char data);
void buf1SkipTo(unsigned char data);
uint8_t buf0Size(void);
uint8_t buf1Size(void);
void USART0_RX_vect(void);
void USART1_RX_vect(void);


#endif /* SERIAL_H_ */
