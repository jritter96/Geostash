#include "gps.h"

#include "../system_defs.h"
#include "gps_defs.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define NUM_DECIMAL_PLACES 2


void Init_GPS(void)
{
    // Set the UART chip's baud rate:
    const unsigned int divisorLatch = SYSTEM_CLOCK_FREQ / 9600 / 16;
    GPS_UART_ADDR_LCR = 0x80;
    GPS_UART_ADDR_DLL = divisorLatch & 0xFF;
    GPS_UART_ADDR_DLM = divisorLatch >> 8;
    GPS_UART_ADDR_LCR = 0x00;

    //Set bit 7 of Line control register back to 0 and
    //program other bits in that reg for 8 bit data, 1 stop bit, and no parity
    GPS_UART_ADDR_LCR = 0x03;

    // Reset the FIFO’s in the FiFo Control Reg by setting bits 1 & 2
	GPS_UART_ADDR_FCR = 0x06;

    // Now Clear all bits in the FiFo control registers
	GPS_UART_ADDR_FCR = 0x00;
}


char sendCommand(const char c)
{
    // wait for Transmitter Holding Register bit (5) of line status register to be '1'
    // indicating we can write to the device
    // write character to Transmitter fifo register
    // return the character we printed
	while (~GPS_UART_ADDR_LSR & 0x20);

	GPS_UART_ADDR_THR = c;
	return c;
}


char getLog(void)
{
    // wait for Data Ready bit (0) of line status register to be '1'
    // read new character from ReceiverFiFo register
	while (~GPS_UART_ADDR_LSR & 0x01);

    // return new character
	return GPS_UART_ADDR_RBR;
}


struct location getLocation(void) {
    char const*const FIELD_DELIM = ",";

    char msg[500 + 1] = {0,};
    for (int i = 0; i < 500; i++) {
        msg[i] = getLog();
    }
    char * token = strstr(msg, "$GPGGA");
    token = strstr(token + 1, "$GPGGA"); 

    if (!token) {
        // not found :(
        return (struct location){0,0};
    }
    token = strtok(token, FIELD_DELIM); // tokenize GPGGA.
    token = strtok(NULL,  FIELD_DELIM); // tokenize time.
    token = strtok(NULL,  FIELD_DELIM); // tokenize latitude (might be the character 'A').
    if (*token == 'A') {
        token = strtok(NULL,  FIELD_DELIM); // tokenize latitude.
    }
    #define toknum(strIndex) (token[strIndex] - '0')
    const unsigned lat_dd
        = toknum(0) * 10
        + toknum(1);
    const unsigned lat_mm
        = toknum(2) * 100000
        + toknum(3) * 10000
        // dot character here.
        + toknum(5) * 1000
        + toknum(6) * 100
        + toknum(7) * 10
        + toknum(8);
    double lat = lat_dd + ((double)lat_mm / 60.0 / 10000.0);
    token = strtok(NULL, FIELD_DELIM);
    if (*token == 'S') {
        lat = -lat;
    }
    token = strtok(NULL, FIELD_DELIM);
    unsigned lon_dd
        = toknum(0) * 100
        + toknum(1) * 10
        + toknum(2);
    unsigned lon_mm
        = toknum(3) * 100000
        + toknum(4) * 10000
        // dot character here.
        + toknum(6) * 1000
        + toknum(7) * 100
        + toknum(8) * 10
        + toknum(9);
    double lon = lon_dd + ((double)lon_mm / 60.0 / 10000.0);
    token = strtok(NULL,FIELD_DELIM);
    if (*token == 'W') {
        lon = -lon;
    }
    return (struct location){lat:lat, lon:lon};
}


struct time getTime(void){
    const char FIELD_DELIM[2] = ",";

    struct time t;
    char msg[500];
    for (int i = 0; i < 500; i++) {
        msg[i] = getLog();
    }
    // check if the format option is GPGGA
    // get first line
    const char newLine[2] = "$";
    char * token = strtok(msg, newLine);

    while (token != NULL) {
        if (!strcmp(token, "GPGGA")) {
            token = strtok(NULL, FIELD_DELIM);
            t.ss = ((int)(token[4]-'0') * 10) +  (int)(token[5]-'0');
            t.mm = ((int)(token[2]-'0') * 10) +  (int)(token[3]-'0');
            t.hh = ((int)(token[0]-'0') * 10) + ((int)(token[1]-'0') - 8);
            if (t.hh < 0) { t.hh += 24; }
            t.totalSeconds = t.ss + (t.mm * 60) + (t.hh * 3600);
            break;
        }
        token = strtok(NULL, newLine);
        token = strtok(NULL, FIELD_DELIM);
    }
    return t;
}


double getDistance(const struct location loc1, const struct location loc2) {
    double lat1 = loc1.lat;
    double lat2 = loc2.lat;
    double lon1 = loc1.lon;
    double lon2 = loc2.lon;

    const double dLat = (lat2 - lat1) * M_PI / 180.0;
    const double dLon = (lon2 - lon1) * M_PI / 180.0;

    // convert to radians
    lat1 *= M_PI / 180.0;
    lat2 *= M_PI / 180.0;

    // apply formulae:
    // https://stackoverflow.com/a/27943/11107541
    const double a
        = pow(sin(dLat / 2), 2)
        + pow(sin(dLon / 2), 2)
        * cos(lat1) * cos(lat2);
    const double EARTH_RANGE_KM = 6371;
    const double c = 2 * asin(sqrt(a));
    return EARTH_RANGE_KM * c * 1000;
}


void timeSetTotalSeconds(struct time *const t) {
    unsigned long total = t->hh;
    total = total * 60 + t->mm;
    total = total * 60 + t->ss;
    t->totalSeconds = total;
}


struct time getRecord(const struct time startTime, const struct time endTime) {
    struct time record;
    unsigned long diff = endTime.totalSeconds - startTime.totalSeconds;

    record.ss = diff % 60; diff /= 60;
    record.mm = diff % 60; diff /= 60;
    record.hh = diff;
    record.totalSeconds = diff;

    return record;
}


void gpsDistanceToString(double dist, char *const buf) {
    // note on below: anything greater than 9 requires printing as unsigned long long
    double decimalPart = (dist - (int)dist);
    for (int i = 0; i < NUM_DECIMAL_PLACES; i++) {
        decimalPart *= 10;
    }
    int charsWritten = sprintf(buf, "%d.%lu", (int)dist, (unsigned long)decimalPart);
    buf[charsWritten] = '\0';
}
