/*
 * radio.c
 *
 *  Created on: Jan 15, 2012
 *      Author: ratmandu
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "radio.h"

void trx_radioInit() {
//	uint8_t addr[8];
//	addr[0] = MAC_ADDR_0;
//	addr[1] = MAC_ADDR_1;
//	addr[2] = MAC_ADDR_2;
//	addr[3] = MAC_ADDR_3;
//	addr[4] = MAC_ADDR_4;
//	addr[5] = MAC_ADDR_5;
//	addr[6] = MAC_ADDR_6;
//	addr[7] = MAC_ADDR_7;
//
	IRQ_STATUS |= (1 << TX_END) | (1 << RX_END) | (1 << RX_START);
	IRQ_MASK |= (1 << TX_END_EN) | (1 << RX_END_EN) | (1 << RX_START_EN);

	trx_setTRXState(STATE_FORCE_TRX_OFF);
	while (trx_getTRXState() != STATUS_TRX_OFF);
//	trx_setExtAddr(addr);
	trx_setChannel(RADIO_CHANNEL);
	trx_setPANID(PAN_ADDR);
	trx_setTRXState(STATE_RX_ON);
	while (trx_getTRXState() != STATUS_RX_ON);

//	trx_calibrate();

	PORTG |= (1<<PG0);
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);
	PORTG &= ~(1<<PG0);
}

void trx_setChannel(uint8_t channel) {
	PHY_CC_CCA = ((PHY_CC_CCA & ~0x1F) | (channel & 0x1F));
}

void trx_calibrate() {
	FTN_CTRL |= (1 << FTN_START);
    PLL_CF |= (1 << PLL_CF_START);
    PLL_DCU |= (1 << PLL_DCU_START);
}

void trx_setTXPower(uint8_t power) {
	PHY_TX_PWR = ((PHY_TX_PWR & ~0x0F) | (power & 0x0F));
}

void trx_setRXSensitivity(uint8_t level) {
	RX_SYN = ((RX_SYN & ~0x0F) | (level & 0x0F));
}

void trx_send(void* data, uint8_t len) {
	if (len > 125) {
		return;
	}

	// make sure we arent receiving anything
	waitWhile(STATUS_BUSY_RX);

	// turn on PLL and prepare for transmission
	trx_setTRXState(STATE_PLL_ON);

	// we are using automatic CRC, so we need to send 2 extra bytes
	TRXFBST = len;

	// insert into transmission buffer
	memcpy((void*)&TRXFBST+1, data, len);

//	TRX_BUF(i+1) = 't';

	while (trx_getTRXState() != STATUS_PLL_ON)
		;

	// start transmission
	trx_setTRXState(STATE_TX_START);

}

void trx_setTRXState(uint8_t state) {
	while ((TRX_STATUS & 0x1F) == 0x1F)
		;
	TRX_STATE = state;
}

void trx_enterTRXSleep() {
	trx_setTRXState(STATE_TRX_OFF);
}

void trx_leaveTRXSleep() {
	trx_setTRXState(STATE_RX_ON);
}

void trx_setPANID(uint16_t panid) {
	PAN_ID_0 = (panid & 0xFF);
	PAN_ID_1 = ((panid >> 8) &0xFF);
}

void trx_setShortAddress(uint16_t shortaddr) {
	SHORT_ADDR_0 = (shortaddr & 0xFF);
	SHORT_ADDR_1 = (shortaddr >>8) &0xFF;
}

void trx_setExtAddr(uint8_t* extaddr) {
	IEEE_ADDR_0 = extaddr[0];
	IEEE_ADDR_1 = extaddr[1];
	IEEE_ADDR_2 = extaddr[2];
	IEEE_ADDR_3 = extaddr[3];
	IEEE_ADDR_4 = extaddr[4];
	IEEE_ADDR_5 = extaddr[5];
	IEEE_ADDR_6 = extaddr[6];
	IEEE_ADDR_7 = extaddr[7];
}

void waitWhile(uint8_t status) {
	while (trx_getTRXState() == status) {
		;
	}
}

uint16_t trx_getShortAddress() {
	uint16_t shortAddr;
	shortAddr = (SHORT_ADDR_H << 8);
	shortAddr |= SHORT_ADDR_L;
	return shortAddr;
}

uint16_t trx_getPANID() {
	uint16_t panID;
	panID = (PAN_ID_1 << 8);
	panID |= PAN_ID_0;
	return panID;
}

void trx_getExtAddr(uint8_t* addr) {
	addr[0] = IEEE_ADDR_0;
	addr[1] = IEEE_ADDR_1;
	addr[2] = IEEE_ADDR_2;
	addr[3] = IEEE_ADDR_3;
	addr[4] = IEEE_ADDR_4;
	addr[5] = IEEE_ADDR_5;
	addr[6] = IEEE_ADDR_6;
	addr[7] = IEEE_ADDR_7;
}

uint8_t trx_isSleeping() {
	if (trx_getTRXState() == STATUS_SLEEP)
		return 1;
	else
		return 0;
}

uint8_t trx_getTRXState() {
	return (TRX_STATUS & TRX_STATUS_MASK);
}

uint8_t trx_getRandomNumber() {
	uint8_t rand;
	rand = ((PHY_RSSI & PHY_RSSI_RANDOM_MASK) >> 5);
	_delay_us(2);
	rand |= ((PHY_RSSI & PHY_RSSI_RANDOM_MASK) >> 3);
	_delay_us(2);
	rand |= ((PHY_RSSI & PHY_RSSI_RANDOM_MASK) >> 1);
	_delay_us(2);
	rand |= ((PHY_RSSI & PHY_RSSI_RANDOM_MASK) << 1);
	return rand;
}

uint8_t trx_getRSSI() {
	return PHY_RSSI & PHY_RSSI_MASK;
}

ISR(TRX24_TX_END_vect) {
	trx_setTRXState(STATE_RX_ON);
}

ISR(TRX24_RX_END_vect) {
	if (ReadBit(PHY_RSSI, RX_CRC_VALID) == 0) {
		return;
	}

	uint8_t length = TST_RX_LENGTH;
	length -= 2;

	frameLength = length;

	uint8_t i;
	for (i=0; i<length; i++) {
		memcpy(frameData, (void*)&TRXFBST+1, (uint8_t)TRXFBST);
	}

	// blink LED
	PORTG |= (1<<PG0);
	// put your own frame decoding/data handling function here

	_delay_ms(50);
	PORTG &= ~(1<<PG0);
	_delay_ms(150);
}

ISR(TRX24_RX_START_vect) {
	rssi = PHY_RSSI;
	PORTG |= (1<<PG0);
	_delay_ms(200);
	PORTG &= ~(1<<PG0);
}
