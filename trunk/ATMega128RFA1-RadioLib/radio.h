/*
 * radio.h
 *
 *  Created on: Jan 15, 2012
 *      Author: ratmandu
 */

#ifndef RADIO_H_
#define RADIO_H_

#define TRX_MAX_FRAME_SIZE 127
#define TRX_FIFO_SIZE 128
#define TRX_WAIT_BEFORE
//#define TRX_WAIT_AFTER

#define SHORT_ADDR_L 0xFE
#define SHORT_ADDR_H 0xED

#define RADIO_CHANNEL 20

#define PAN_ADDR 0xACDC

#define MAC_ADDR_0 0x01
#define MAC_ADDR_1 0x23
#define MAC_ADDR_2 0x45
#define MAC_ADDR_3 0x67
#define MAC_ADDR_4 0x89
#define MAC_ADDR_5 0xab
#define MAC_ADDR_6 0xcd
#define MAC_ADDR_7 0xef

#define modifyReg(var, mask, value)   (var = ((var & ~mask) | (value & mask)))

// Transceiver state status
#define TRX_STATUS_MASK                                                                                                 0x1f
enum
{
        STATUS_BUSY_RX = 0x01,
        STATUS_BUSY_TX = 0x02,
        STATUS_RX_ON = 0x06,
        STATUS_TRX_OFF = 0x08,
        STATUS_PLL_ON = 0x09,
        STATUS_SLEEP = 0x0f,
        STATUS_BUSY_RX_AACK = 0x11,
        STATUS_BUSY_TX_ARET = 0x12,
        STATUS_RX_AACK_ON = 0x16,
        STATUS_TX_ARET_ON = 0x19,
        STATUS_STATE_TRANSITION_IN_PROGRESS = 0x1f
};

// Transceiver state change commands
enum
{
        STATE_NOP = 0x00,
        STATE_TX_START = 0x02,
        STATE_FORCE_TRX_OFF = 0x03,
        STATE_FORCE_PLL_ON = 0x04,
        STATE_RX_ON = 0x06,
        STATE_TRX_OFF = 0x08,
        STATE_PLL_ON = 0x09,
        STATE_RX_AACK_ON = 0x16,
        STATE_TX_ARET_ON = 0x19
};

#define PHY_RSSI_MASK 			0x1f
#define PHY_RSSI_RANDOM_MASK	0x60
#define TRX_BUF(i) 				_SFR_MEM8(0x180 + i)
#define NULL (0)
#define ReadBit(var, bit) (((var) & (1 << (bit))) != 0)

static volatile uint8_t rssi;
static uint8_t* frameData[125];
static uint8_t frameLength;

enum
{
        VERSION_A = 2, // A and B are both 2 according to the data sheet
        VERSION_B = 2,
        VERSION_C = 3,
        VERSION_D = 4
};

uint8_t currentlyTransmitting;

void trx_radioInit();
void trx_setChannel(uint8_t channel);
void trx_send(void* data, uint8_t len);
void trx_calibrate();
void trx_setTXPower(uint8_t power);
void trx_setRXSensitivity(uint8_t level);
void trx_setTRXState(uint8_t state);
void trx_enterTRXSleep();
void trx_leaveTRXSleep();
void trx_setPANID(uint16_t panid);
void trx_setShortAddress(uint16_t shortaddr);
void trx_setExtAddress(uint8_t* extaddr);
void trx_waitWhile(uint8_t status);
void trx_getExtAddress(uint8_t* addr);

uint16_t trx_getShortAddress();
uint16_t trx_getPANID();
uint8_t trx_isSleeping();
uint8_t trx_getTRXState();
uint8_t trx_getRandomNumber();
uint8_t trx_getRSSI();




#endif /* RADIO_H_ */
