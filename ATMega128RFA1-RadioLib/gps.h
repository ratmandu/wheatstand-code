/*
 * gps.h
 *
 *  Created on: Jan 29, 2012
 *      Author: ratmandu
 */

#ifndef GPS_H_
#define GPS_H_

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
extern char nmeaSentences[][6];
static struct nmea nmeaData;
char nmeaBuf[nmeaBufSize];

void initGPS(void);
void gpsParse(void);

#endif /* GPS_H_ */
