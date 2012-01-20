/*
 * radio.c
 *
 *  Created on: Jan 15, 2012
 *      Author: ratmandu
 */

#include "radio.h"

void trx_radioInit() {

}

void trx_setChannel(uint8_t channel) {
	PHY_CC_CCA = ((PHY_CC_CCA & ~0x1F) | (channel & 0x1F));
}

void trx_calibrate() {

}

void trx_setTXPower(uint8_t power) {
	PHY_TX_PWR = ((PHY_TX_PWR & ~0x0F) | (power & 0x0F));
}

void trx_setRXSensitivity(uint8_t level) {
	RX_SYN = ((RX_SYN & ~0x0F) | (level & 0x0F));
}

void trx_send(void* data, uint8_t len) {

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

}

void trx_setShortAddress(uint16_t shortaddr) {

}

void trx_setExtAddr(uint8_t* extaddr) {

}

uint16_t trx_getShortAddress() {
	uint16_t shortAddr;
	shortAddr = (SHORT_ADDR_H << 8);
	shortAddr |= SHORT_ADDR_L;
	return shortAddr;
}

uint16_t trx_getPANID() {
	uint16_t panID;
	panID = (PAN_ADDR_H << 8);
	panID |= PAN_ADDR_L;
	return panID;
}

uint8_t* trx_getExtAddr() {

}

uint8_t trx_isSleeping() {

}

uint8_t trx_getTRXState() {
	if ((TRX_STATUS & TRX_STATUS_MASK) == STATUS_SLEEP)
		return 1;
	else
		return 0;
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
	uint8_t rssi;
	rssi = PHY_RSSI & PHY_RSSI_MASK;
}
