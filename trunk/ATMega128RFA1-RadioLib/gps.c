/*
 * gps.c
 *
 *  Created on: Jan 29, 2012
 *      Author: ratmandu
 */

#include <avr/io.h>
#include <string.h>
#include "gps.h"
#include "serial.h"

struct nmea {
	float lat;
	char latHemi; // N or S
	float lon;
	char lonHemi; // E or W
	float speed;  // knots
	float alt;	  // Meters above sea level
	float head;	  // true heading in degrees
	float utc;	  // UTC time HHMMSS
	int sats;	  // number of satellites total
	int satsInView;// number of satellites in view
	int fix3D;    // do we have a 3D fix?
	int fixQual;  // Fix Quality
	int day;	  // Day
	int month;	  // Month
	int16_t year;   // Year
	int tzOffset; // offset of current timezone
};

#define nmeaBufSize 150
char nmeaSentences[][6] = {"GPGGA", "GPGLL", "GPGSA", "GPGSV", "GPRMC", "GPVTG", "GPZDA"};
static struct nmea nmeaData;
char nmeaBuf[nmeaBufSize];

void initGPS() {
	initSerial1(38400);

	nmeaData.lat = 0.0;
	nmeaData.latHemi = 'N';
	nmeaData.lon = 0.0;
	nmeaData.lonHemi = 'W';
	nmeaData.speed = 0.0;
	nmeaData.alt = 0.0;
	nmeaData.head = 0.0;
	nmeaData.utc = 0.0;
	nmeaData.sats = 0;
	nmeaData.satsInView = 0;
	nmeaData.fix3D = 0;
	nmeaData.fixQual = 0;
	nmeaData.day = 0;
	nmeaData.month = 0;
	nmeaData.year = 0;
	nmeaData.tzOffset = 0;
}

void gpsParse() {
	memset(&nmeaBuf, 0, nmeaBufSize);
	uint8_t bufSize;
	// loop until we find a $
	buf1SkipTo('$');
	if (buf1Size() < 10) { // magic number for now. make sure there is data in the buffer
		return;
	}

	uint8_t count = 0;
	// read until we hit an asterisk or the buffer is full
	do {
		nmeaBuf[count++] = readFromBuffer1(&bufSize);
	} while (count < 150 && nmeaBuf[count-1] != '*');

	if (count == 150) {
		// we have bunk data. check baud rate
		return;
	}

	// check what NMEA sentence we have
	if (strncmp(nmeaBuf, nmeaSentences[0], 5)) { // we have a GPS Fix Data (GPGGA)

	} else if (strncmp(nmeaBuf, nmeaSentences[1], 5)) { // we have

	} else if (strncmp(nmeaBuf, nmeaSentences[2], 5)) { //

	} else if (strncmp(nmeaBuf, nmeaSentences[3], 5)) { //

	} else if (strncmp(nmeaBuf, nmeaSentences[4], 5)) { //

	} else if (strncmp(nmeaBuf, nmeaSentences[5], 5)) { //

	} else if (strncmp(nmeaBuf, nmeaSentences[6], 5)) { //

	}

}
