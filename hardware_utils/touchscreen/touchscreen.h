#ifndef _TOUCHSCREEN_H_
#define _TOUCHSCREEN_H_

#include "ar1100_defs.h"

/**
 * A data type to hold an x/y coordinate.
 * 
 * Fields are unsigned integers.
 */
typedef struct {
    unsigned int x;
    unsigned int y;
} Point;

/**
 * x and y are for the top left.
 * x + width  is just outside the rectangle.
 * y + height is just outside the rectangle.
 * width and height must not be zero.
 */
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} Rectangle;


/**
 * Initialize touch screen controller
 */
void Init_Touch(void);

/**
 * Test if the screen has been touched.
 */
bool ScreenTouched(void);

/**
 * Polls for a single press-down event. Timeout is in seconds.
 */
Point GetPress(const unsigned int timeout, volatile bool *const timedOut);

/**
 * Polls for a single press-up / release event. Timeout is in seconds.
 */
Point GetRelease(const unsigned int timeout, volatile bool *const timedOut);

/**
 * Rectangle bounds are positively inclusive. Ie. {0,0} is in {{0,0}, {0,0}}.
 */
bool isPointInRectangle(Point, Rectangle);


#endif
