#ifndef _GPS_H_
#define _GPS_H_

#include "gps_defs.h"

#define GPD_DISTANCE_STRING_BUFFER_SIZE 50

struct time {
    unsigned int   hh;
    unsigned short mm;
    unsigned short ss;
    unsigned long  totalSeconds;
};

struct location {
    double lat;
    double lon;
};


void Init_GPS(void);

char sendCommand(const char c);
char getLog(void);

struct location getLocation(void);

// return the distance in meters
double getDistance(const struct location l1, const struct location l2);

struct time getTime(void);

void timeSetTotalSeconds(struct time *const);

struct time getRecord(const struct time startTime, const struct time endTime);

void gpsDistanceToString(double dist, char *const buf);

#endif
