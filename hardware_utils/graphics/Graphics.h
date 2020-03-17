#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "GraphicsHardware_defs.h"
#include "../graphics/Colours.h"

#include "../touchscreen/touchscreen.h"
#include "../../main/players/players.h"

typedef enum {
    FONT_SMALL = 0,
    FONT_LARGE = 1,
} Font;

typedef enum {
    NO_ERASE = 0,
    ERASE = 1,
} Erase;

// Screen resolution values
#define X_RES 800
#define Y_RES 480

// Screen edge adjustments
#define X_MIN 33
#define X_MAX 783
#define Y_MIN 0
#define Y_MAX 479
#define X_RES_ACTUAL 750
#define Y_RES_ACTUAL 479

// Character sizes, in pixels
#define SIZE_SMALL_CHAR_X 5
#define SIZE_SMALL_CHAR_Y 7
#define SIZE_LARGE_CHAR_X 10
#define SIZE_LARGE_CHAR_Y 14
#define DEFAULT_SPACE_SIZE 3

// Area defines
#define PLAYER_SCORE_AREA_START 115
#define PLAYER_SCORE_AREA_END 369

/*
 * Initialization functions.
 */
void loadPalette(void);
void Init_Graphics(void);

/*
 * Shape-drawing functions.
 */
void drawHLine(int x1, int y1, int length, Colour colour);
void drawVLine(int x1, int y1, int length, Colour colour);
void drawLine(int x1, int y1, int x2, int y2, Colour colour);
void drawRectangle(const Rectangle, Colour colour);
void drawRectangleFilled(const Rectangle, Colour colour);
void drawThickRectangle(const Rectangle, Colour colour, int pixels);
void drawCircle(int x0, int y0, int range, Colour colour);
void drawEllipse(int rx, int ry, int xc, int yc, Colour colour);
void drawButton(const Rectangle, Font, char const*const string, Colour fg, Colour bg);
void drawString(Font, char const*const string, unsigned x, unsigned y, unsigned spaceWidth, Colour fg, Colour bg, bool erase);

/*
 * Screen-drawing functions.
 */
// Final Demo Screens, in order of user flow
void renderWelcomeScreen(void);
void renderTimeSelectionScreen(void);
void renderRangeSelectionScreen(void);
void renderConnectToServerScreen(const char* serverAddress);
void renderPlayerHideScreen(struct Player *const player);
void renderEndGameConfirmationScreen(void);
void renderEnterPinScreen(void);
void renderStatScreen(const struct Player *const player, const char* hiddenTime, const char* numberOfPlayers);
void renderGameOverScreen(void);
void renderErrorMessage(const char* errorMessage);
void renderWaitMessage(const char* waitMessage);
void renderOutOfRangeScreen(void);

// Functions for re-rendering parts of screens
void renderTimeSelectUpdateHours(const char* hours);
void renderTimeSelectUpdateMinutes(const char* minutes);
void renderRangeSelectUpdateRange(const char* radius);

// Sprint 1 Demo Screens
void renderBasicWelcomeScreen(void);
void renderBasicHomeScreen(void);
void renderBasicActiveScreen(void);

/*
 * Other functions.
 */
const char* intToCharPtr(int num);

#endif