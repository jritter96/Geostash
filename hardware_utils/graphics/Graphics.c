#include "Graphics.h"

#include "GraphicsHardware_defs.h"
#include "ColourPallette.c"
#include "Fonts.c"

#include "../../main/graphics_buttons.h"
#include "../../main/mygame.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


extern const unsigned short int Font10x14[][14];
extern const unsigned char Font5x7[][7];

/*******************************************************************************************
* This function writes a single pixel to the x,y coords specified using the specified colour
* Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
********************************************************************************************/
void WriteAPixel(int x, int y, Colour colour)
{
    WAIT_FOR_GRAPHICS;

    GraphicsX1Reg = x;
    GraphicsY1Reg = y;
    GraphicsColourReg = colour;
    GraphicsCommandReg = PutAPixel;
}

/*********************************************************************************************
* This function reads a single pixel from the x,y coords specified and returns its colour
* Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
*********************************************************************************************/
int ReadAPixel(int x, int y)
{
    WAIT_FOR_GRAPHICS;

    GraphicsX1Reg = x;
    GraphicsY1Reg = y;
    GraphicsCommandReg = GetAPixel;

    WAIT_FOR_GRAPHICS;
    // return the palette number (colour)
    return (int)(GraphicsColourReg);
}

/**********************************************************************************
** subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
************************************************************************************/
static void ProgramPalette(const Colour PaletteNumber, const unsigned int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16;                 // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB;                       // program green and blue into ls 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour; // issue command
}

// 5x7 Font
static void drawSmallChar(const int x, const int y, Colour fgColour, Colour bgColour, const char c, bool erase)
{
    if ((x >= X_RES) || (y >= Y_RES)) {
        return;
    }
    if ((c >= ' ') && (c <= '~')) {
        unsigned row;
        for (row = 0; row < SIZE_SMALL_CHAR_Y; row++) {
            const unsigned char pixels = Font5x7[c - 0x20][row];
            unsigned int bitMask = 1 << (SIZE_SMALL_CHAR_X - 1);
            unsigned col;
            for (col = 0; col < SIZE_SMALL_CHAR_X; col++) {
                if ((pixels & bitMask)) {
                    WriteAPixel(x + col, y + row, fgColour);
                } else if (erase) {
                    WriteAPixel(x + col, y + row, bgColour);
                }
                bitMask >>= 1;
            }
        }
    }
}

// 10x14 Font
static void drawLargeChar(const int x, const int y, Colour fgColour, Colour bgColour, const char c, bool erase)
{
    if ((x >= X_RES) || (y >= Y_RES)) {
        return;
    }
    if ((c >= ' ') && (c <= '~')) {
        int row;
        for (row = 0; row < SIZE_LARGE_CHAR_Y; row++) {
            const unsigned short pixels = Font10x14[c - 0x20][row];
            unsigned int bitMask = 1 << (SIZE_LARGE_CHAR_X - 1);
            unsigned col;
            for (col = 0; col < SIZE_LARGE_CHAR_X; col++) {
                if (pixels & bitMask) {
                    WriteAPixel(x + col, y + row, fgColour);
                } else if (erase) {
                    WriteAPixel(x + col, y + row, bgColour);
                }
                bitMask >>= 1;
            }
        }
    }
}

void wait(float seconds)
{
    clock_t clockStart = clock();
    while (clock() - clockStart < seconds * CLOCKS_PER_SEC);
}

/*
 * Given a percentage from the top of the screen, returns the y-coordinate
 * corresponding to that percentage.
 */
int verticalPercentToYCoord(int vertPercent)
{
    return vertPercent / 100.00 * Y_RES_ACTUAL;
}

/*
 * Given parameters about a string to print, returns the x-coordinate of the
 * left side of the string to make it centered.
 */
int centeredStringInScreenXCoord(const char* string, Font font, int width)
{
    int stringLength;

    switch (font)
    {
        case FONT_SMALL:
            stringLength = (SIZE_SMALL_CHAR_X * strlen(string)) + (width * (strlen(string) - 1));
            break;
        default:
            stringLength = (SIZE_LARGE_CHAR_X * strlen(string)) + (width * (strlen(string) - 1));
    }

    return ((X_RES_ACTUAL - stringLength) / 2) + 33;
}

// returns the starting x coordinate of a string so that it is centered between x1 and x2
int centeredStringXCoord(const char* string, Font font, int width, int x1, int x2)
{
    int stringLength;
    switch (font)
    {
        case FONT_SMALL:
            stringLength = (SIZE_SMALL_CHAR_X * strlen(string)) + (width * (strlen(string) - 1));
            break;
        default:
            stringLength = (SIZE_LARGE_CHAR_X * strlen(string)) + (width * (strlen(string) - 1));
    }
    if (x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    return x1 + (((x2 - x1) - stringLength) / 2);
}

// returns the starting y coordinate of a string that is centered between y1 and y2
int centeredStringYCoord(Font font, int y1, int y2)
{
    if (y1 > y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    switch (font)
    {
        case FONT_SMALL:
            return y1 + (((y2 - y1) - SIZE_SMALL_CHAR_Y) / 2);
        default:
            return y1 + (((y2 - y1) - SIZE_LARGE_CHAR_Y) / 2);
    }
}

/*
 * Initialization functions.
 */
void Init_Graphics(void)
{
    unsigned i;
    for (i = 0; i < Colour_NUM_ENTRIES; i++) {
        ProgramPalette(i, ColourPalletteData[i]);
    }
}

// colours in the entire screen the inputted colour
void fillScreen(int backgroundColour)
{
    unsigned i;
    for (i = 0; i < Y_MAX; i++) {
        drawHLine(0, i, 800, backgroundColour);
    }
}

// helper function to convert int the char*
const char* intToCharPtr(int num)
{
    char* buffer;
    sprintf(buffer, "%d", num);
    const char * charPtr = buffer;
    return charPtr;
}

/*
 * Shape-drawing functions.
 */
void drawHLine(int x1, int y1, int length, Colour colour)
{
    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x1 + length - 1;
    GraphicsY2Reg = y1;
    GraphicsColourReg = colour;
    GraphicsCommandReg = DrawHLine;
}

void drawVLine(int x1, int y1, int length, Colour colour)
{
    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x1;
    GraphicsY2Reg = y1 + length - 1;
    GraphicsColourReg = colour;
    GraphicsCommandReg = DrawVLine;
}

void drawLine(int x1, int y1, int x2, int y2, Colour colour)
{
    WAIT_FOR_GRAPHICS;
    GraphicsX1Reg = x1;
    GraphicsY1Reg = y1;
    GraphicsX2Reg = x2;
    GraphicsY2Reg = y2;
    GraphicsColourReg = colour;
    GraphicsCommandReg = DrawLine;
}

void drawRectangle(const Rectangle r, Colour colour)
{
    const unsigned rx2 = (r.x + r.width  - 1);
    const unsigned ry2 = (r.y + r.height - 1);
    drawHLine(r.x, r.y, r.width,  colour);
    drawHLine(r.x, ry2, r.width,  colour);
    drawVLine(r.x, r.y, r.height, colour);
    drawVLine(rx2, r.y, r.height, colour);
}

void drawRectangleFilled(Rectangle r, Colour colour)
{
    unsigned col;
    for (col = r.x; col < r.x + r.width; col++) {
        drawVLine(col, r.y, r.height, colour);
    }
}

// draws a rectangle pixels pixels thick, the inputted rectangle is the outer limit of the rectangle
void drawThickRectangle(Rectangle r, Colour colour, int pixels)
{
    unsigned i;
    for (i = 0; i < pixels; i++)
    {
        drawRectangle(r, colour);
        r.x++;
        r.y++;
        r.width -= 2;
        r.height -= 2;
    }
}

/*
 * Prints a string at the specified location.
 * 
 * (x, y) corresponds to the coordinate of the top-left of the string.
 * width corresponds to the width/space between characters.
 */
void drawString(
    Font font,
    const char* string,
    const unsigned x, const unsigned y,
    const unsigned spaceWidth,
    Colour fgColour, Colour bgColour,
    bool erase
) {
    switch (font)
    {
        int i;
        case FONT_SMALL:
            for (i = 0; string[i]; i++) {
                drawSmallChar(x + ((spaceWidth + SIZE_SMALL_CHAR_X) * i), y, fgColour, bgColour, string[i], erase);
            }
            break;
        case FONT_LARGE:
            for (i = 0; string[i]; i++) {
                drawLargeChar(x + ((spaceWidth + SIZE_LARGE_CHAR_X) * i), y, fgColour, bgColour, string[i], erase);
            }
            break;
        default: {
            const char* errorMessage = "ERROR_INVALID_FONT";
            for (i = 0; errorMessage[i]; i++) {
                drawLargeChar(x + ((spaceWidth + SIZE_LARGE_CHAR_X) * i), y, YELLOW, RED, errorMessage[i], erase);
            }
            break;
        }
    }
}

/*
 * Draws a button with text centered within it.
 * 
 * (x1, y1) and (x2, y2) correspond to the button's top-left and bottom-right
 * corners, respectively.
 */
void drawButton(
    const Rectangle r, 
    Font font, const char* string, 
    Colour fgColour, Colour bgColour
    ) {
    // Draw button
    drawRectangleFilled(r, bgColour);

    const size_t length = strlen(string);
    const int x = r.x + (r.width  / 2) - (((font == FONT_SMALL ? 8 : 12) * length) / 2);
    const int y = r.y + (r.height / 2)   - (font == FONT_SMALL ? 3 : 5);
    const int width = DEFAULT_SPACE_SIZE - (font == FONT_SMALL ? 0 : 1);

    // Draw button text
    drawString(font, string, x, y, width, fgColour, bgColour, NO_ERASE);
}

/*
 * Screen-drawing functions.
 */
void renderWelcomeScreen(void)
{
    const char* DISPLAY_MESSAGE = "G E O S T A S H";
    const char* BUTTON_MESSAGE = "Start Game";

    fillScreen(THEME_BACKGROUND);
    drawString(FONT_LARGE, DISPLAY_MESSAGE, centeredStringInScreenXCoord(DISPLAY_MESSAGE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(35), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawButton(WELCOME_START_GAME_BUTTON, FONT_LARGE, BUTTON_MESSAGE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
}

/*
 * Draws a Back and a Next button in the bottom corners of the screen
 */
void renderNavButtons(void)
{
    const char* NEXT_BUTTON_TITLE = "Next";
    const char* BACK_BUTTON_TITLE = "Back";

    drawButton(NEXT_BUTTON, FONT_LARGE, NEXT_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // next button
    drawButton(BACK_BUTTON, FONT_LARGE, BACK_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // back button
}

// draws a screen with two input fields manipulated by scroll boxes
void renderTimeSelectionScreen(void)
{
    const char* SCREEN_TITLE = "SET GAME DURATION";

    fillScreen(THEME_BACKGROUND);
    renderNavButtons();
    drawThickRectangle((Rectangle){208, 180, 100, 120}, THEME_ACCENT, 3); // hour box
    drawThickRectangle((Rectangle){448, 180, 100, 120}, THEME_ACCENT, 3); // minute box
    drawButton(TIME_SELECT_DECREMENT_HOURS_BUTTON, FONT_LARGE, "-", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // - hours
    drawButton(TIME_SELECT_INCREMENT_HOURS_BUTTON, FONT_LARGE, "+", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // + hours
    drawButton(TIME_SELECT_DECREMENT_MINUTES_BUTTON, FONT_LARGE, "-", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // - minutes
    drawButton(TIME_SELECT_INCREMENT_MINUTES_BUTTON, FONT_LARGE, "+", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // + minutes
    drawString(FONT_LARGE, SCREEN_TITLE, centeredStringInScreenXCoord(SCREEN_TITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE); // Title
    drawString(FONT_LARGE, intToCharPtr(GAME_TIME_HH_MIN), centeredStringXCoord(intToCharPtr(GAME_TIME_HH_MIN), FONT_LARGE, DEFAULT_SPACE_SIZE, 208, 308), centeredStringYCoord(FONT_LARGE, 180, 300), DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE); // display starting minimum hours
    drawString(FONT_LARGE, intToCharPtr(GAME_TIME_MM_MIN), centeredStringXCoord(intToCharPtr(GAME_TIME_MM_MIN), FONT_LARGE, DEFAULT_SPACE_SIZE, 448, 548), centeredStringYCoord(FONT_LARGE, 180, 300), DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE); // display starting minimum minutes
    drawString(FONT_LARGE, "H", centeredStringXCoord("H", FONT_LARGE, DEFAULT_SPACE_SIZE, 208, 308), 155, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // H
    drawString(FONT_LARGE, "M", centeredStringXCoord("M", FONT_LARGE, DEFAULT_SPACE_SIZE, 448, 548), 155, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // M
    drawString(FONT_LARGE, ":", centeredStringXCoord(":", FONT_LARGE, DEFAULT_SPACE_SIZE, 358, 448), verticalPercentToYCoord(50) - 7, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // :
}

// overwrites and redraws the string in the hours input field
void renderTimeSelectUpdateHours(const char* hours)
{
    drawRectangleFilled((Rectangle){211, 183, 94, 114}, THEME_BACKGROUND);
    drawString(FONT_LARGE, hours, centeredStringXCoord(hours, FONT_LARGE, DEFAULT_SPACE_SIZE, 208, 308), centeredStringYCoord(FONT_LARGE, 180, 300), DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE);
}

// overwrites and redraws the string in the minutes input field
void renderTimeSelectUpdateMinutes(const char* minutes)
{
    drawRectangleFilled((Rectangle){451, 183, 94, 114}, THEME_BACKGROUND);
    drawString(FONT_LARGE, minutes, centeredStringXCoord(minutes, FONT_LARGE, DEFAULT_SPACE_SIZE, 448, 548), centeredStringYCoord(FONT_LARGE, 180, 300), DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE);
}

// draws a screen with a single input field manipulated by a scroll box
void renderRangeSelectionScreen(void)
{
    const char* SCREEN_TITLE = "SET GAME RANGE";
    const char* BOX_LABEL = "METERS";

    fillScreen(THEME_BACKGROUND);
    renderNavButtons();
    drawThickRectangle((Rectangle){333, 180, 150, 120}, THEME_ACCENT, 3); // range box
    drawButton(RANGE_SELECT_DECREMENT_RANGE_BUTTON, FONT_LARGE, "-", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // - range
    drawButton(RANGE_SELECT_INCREMENT_RANGE_BUTTON, FONT_LARGE, "+", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // + range
    drawString(FONT_LARGE, SCREEN_TITLE, centeredStringInScreenXCoord(SCREEN_TITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE); // title
    drawString(FONT_LARGE, intToCharPtr(GAME_RANGE_MIN), centeredStringInScreenXCoord(intToCharPtr(GAME_RANGE_MIN), FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(50) - 7, DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE); // display starting minimum range
    drawString(FONT_LARGE, BOX_LABEL, centeredStringInScreenXCoord(BOX_LABEL, FONT_LARGE, DEFAULT_SPACE_SIZE), 155, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // range box label
}

// overwrites and redraws the string in the range input field
void renderRangeSelectUpdateRange(const char* range)
{
    drawRectangleFilled((Rectangle){336, 183, 144, 114}, THEME_BACKGROUND);
    drawString(FONT_LARGE, range, centeredStringInScreenXCoord(range, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(50) - 7, DEFAULT_SPACE_SIZE, BLACK, THEME_BACKGROUND, NO_ERASE);
}

// draws a screen that display the serverAddress that the users need ot connect to
void renderConnectToServerScreen(const char* serverAddress)
{
    const char* TITLE = "ALMOST TIME TO PLAY";
    const char* DESC1 = "Connect to";
    const char* DESC2 = "on your mobile device's browser to join the game.";
    const char* DESC3 = "Press START once all players are connected.";
    const char* NEXT_BUTTON_TITLE = "Start";
    const char* BACK_BUTTON_TITLE = "Back";

    fillScreen(THEME_BACKGROUND);
    drawString(FONT_LARGE, TITLE, centeredStringInScreenXCoord(TITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(15), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, DESC1, centeredStringInScreenXCoord(DESC1, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(32), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawThickRectangle((Rectangle){258, 188, 300, 30}, THEME_ACCENT, 2);
    drawString(FONT_LARGE, serverAddress, centeredStringInScreenXCoord(serverAddress, FONT_LARGE, DEFAULT_SPACE_SIZE), 195, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, DESC2, centeredStringInScreenXCoord(DESC2, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(50), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, DESC3, centeredStringInScreenXCoord(DESC3, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(57), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawButton(NEXT_BUTTON, FONT_LARGE, NEXT_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // start game button
    drawButton(BACK_BUTTON, FONT_LARGE, BACK_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // back button
}

// draws a screen that allows a player to begin a game round by setting the board tp HIDE mode, or allows them to end the game early
void renderPlayerHideScreen(struct Player *const player)
{
    const char* SCREEN_TITLE1 = "Press HIDE after you have found";
    const char* SCREEN_TITLE2 = " a hiding spot for the board...";
    const char* HIDE_BUTTON_TITLE = "HIDE";
    const char* END_BUTTON_TITLE = "END GAME";
    const char stringFragment[16] = "Current Hider: ";

    char displayString[50];
    strcpy(displayString, stringFragment);
    strcat(displayString, player->username);
    fillScreen(THEME_BACKGROUND);
    drawString(FONT_LARGE, SCREEN_TITLE1, centeredStringInScreenXCoord(SCREEN_TITLE1, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, SCREEN_TITLE2, centeredStringInScreenXCoord(SCREEN_TITLE2, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(15), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, displayString, centeredStringInScreenXCoord(displayString, FONT_LARGE, DEFAULT_SPACE_SIZE), 115, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE);
    drawButton(HIDE_SCREEN_HIDE_BUTTON, FONT_LARGE, HIDE_BUTTON_TITLE, THEME_BACKGROUND, THEME_ACCENT); // hide button
    drawButton(HIDE_SCREEN_END_GAME_BUTTON, FONT_LARGE, END_BUTTON_TITLE, THEME_BACKGROUND, RED); // end game button
}

// draws a confirmation prompt to avoid the user unwittingly ending a game early
void renderEndGameConfirmationScreen(void)
{
    const char* MESSAGE = "End the game?";

    drawRectangleFilled((Rectangle){316, 173, 185, 134}, RED);
    drawString(FONT_LARGE, MESSAGE, centeredStringInScreenXCoord(MESSAGE, FONT_LARGE, DEFAULT_SPACE_SIZE), 203, DEFAULT_SPACE_SIZE, WHITE, RED, NO_ERASE);
    drawButton(END_GAME_CONFIRMATION_SCREEN_YES_BUTTON, FONT_LARGE, "YES", RED, WHITE);
    drawButton(END_GAME_CONFIRMATION_SCREEN_NO_BUTTON, FONT_LARGE, "NO", RED, WHITE);
}

// draws a screen with a number pad to allow the user to enter a pin and "unlock" the board when the find it, the game will be in this screen while it is hidden
void renderEnterPinScreen(void)
{
    const char* SCREEN_TITLE = "G A M E  I N  P R O G R E S S";
    const char* SCREEN_SUBTITLE = "Enter the pin to unlock the board...";

    fillScreen(THEME_BACKGROUND);
    drawString(FONT_LARGE, SCREEN_TITLE, centeredStringInScreenXCoord(SCREEN_TITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, 0);
    drawString(FONT_LARGE, SCREEN_SUBTITLE, centeredStringInScreenXCoord(SCREEN_SUBTITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(25), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, 0);
    drawButton(PIN_NUMPAD_1, FONT_LARGE, "1", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_2, FONT_LARGE, "2", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_3, FONT_LARGE, "3", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_4, FONT_LARGE, "4", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_5, FONT_LARGE, "5", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_6, FONT_LARGE, "6", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_7, FONT_LARGE, "7", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_8, FONT_LARGE, "8", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_9, FONT_LARGE, "9", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_NUMPAD_0, FONT_LARGE, "0", THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawButton(PIN_ENTER_BUTTON, FONT_LARGE, "ENTER", WHITE, RED);
    drawButton(PIN_DELETE_BUTTON, FONT_LARGE, "DELETE", WHITE, RED);
}

// screen to display the current stats of a player who finds the board
void renderStatScreen(const struct Player *const player, const char* hiddenTime, const char* numberOfPlayers)
{
    const char* CONTINUE_BUTTON_TITLE = "Continue";
    const char titleFragment[17] = "Congratulations ";
    const char stringFragment1[23] = "You found GEOSTASH in ";
    const char stringFragment2[31] = "You are currently in position ";
    const char stringFragment3[9] = " out of ";
    const char stringFragment4[18] = " with a score of ";
    const char minutes[9] = " minutes";
    const char* CONTINUE_MESSAGE = "Press Continue to begin the next round";
    char title[50];
    char line1[50];
    char line2[50];
    char line3[50];
    strcpy(title, titleFragment);
    strcat(title, player->username);
    strcpy(line1, stringFragment1);
    strcat(line1, hiddenTime);
    strcat(line1, minutes);
    strcpy(line2, stringFragment2);
    strcat(line2, intToCharPtr(getPlayerPosition(player)));
    strcat(line2, stringFragment3);
    strcat(line2, numberOfPlayers);
    strcpy(line3, stringFragment4);
    strcat(line3, intToCharPtr((int)player->score));
    strcat(line3, minutes);

    fillScreen(THEME_BACKGROUND);
    drawButton(STAT_SCREEN_CONTINUE_BUTTON, FONT_LARGE, CONTINUE_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR); // continue button
    drawString(FONT_LARGE, title, centeredStringInScreenXCoord(title, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, line1, centeredStringInScreenXCoord(line1, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(30), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, line2, centeredStringInScreenXCoord(line2, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(55), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, line3, centeredStringInScreenXCoord(line3, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(60), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE);
    drawString(FONT_LARGE, CONTINUE_MESSAGE, centeredStringInScreenXCoord(CONTINUE_MESSAGE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(75), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE);
}

// display the leaderboard for the ended game
void renderGameOverScreen(void)
{
    const int numberOfPlayers = countActivePlayers(myGame.players);
    const char* SCREEN_TITLE = "G A M E   O V E R";
    const char* SCREEN_SUBTITLE = "Player Scores:";
    const char* MAIN_MENU_BUTTON_TITLE = "Main Menu";
    int i, y_increment = (PLAYER_SCORE_AREA_END - PLAYER_SCORE_AREA_START) / numberOfPlayers, y_val = PLAYER_SCORE_AREA_START + 10;// + y_increment;

    fillScreen(THEME_BACKGROUND);
    drawButton(GAME_OVER_MAIN_MENU_BUTTON, FONT_LARGE, MAIN_MENU_BUTTON_TITLE, THEME_BUTTON_TEXT_COLOUR, THEME_BUTTON_COLOUR);
    drawString(FONT_LARGE, SCREEN_TITLE, centeredStringInScreenXCoord(SCREEN_TITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(10), DEFAULT_SPACE_SIZE, THEME_SCREEN_TITLE_COLOUR, THEME_BACKGROUND, NO_ERASE); // Title
    drawString(FONT_LARGE, SCREEN_SUBTITLE, centeredStringInScreenXCoord(SCREEN_SUBTITLE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(20), DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // Subtitle
    for (i = 0; i < numberOfPlayers; i++)
    {
        char displayNameString[35];
        char displayScoreString[13];
        snprintf(displayNameString, sizeof(displayNameString), "%d", i + 1);
        snprintf(displayScoreString, sizeof(displayScoreString), "%d", (int)myGame.players[i].score);
        //printf("score: %d\n", players[i].score);
        char formatting1[4] = ".  ";
        char formatting1_DD[3] = ". "; // reduced space for double digit numbers
        char formatting2[5] = " min";
        if (i < 9)
        {
            strncat(displayNameString, formatting1, sizeof(formatting1));
        }
        else
        {
            strncat(displayNameString, formatting1_DD, sizeof(formatting1_DD));
        }
        strcat(displayNameString, myGame.players[i].username);
        strncat(displayScoreString, formatting2, sizeof(formatting2));
        drawString(FONT_LARGE, displayNameString, 97, y_val, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // Player Name
        drawString(FONT_LARGE, displayScoreString, 552, y_val, DEFAULT_SPACE_SIZE, THEME_ACCENT, THEME_BACKGROUND, NO_ERASE); // Player Score
        y_val += y_increment;
    }
}

// draws a generic error message box in the middle of the screen
void renderErrorMessage(const char* errorMessage)
{
    drawButton((Rectangle){158, 190, 500, 100}, FONT_LARGE, errorMessage, WHITE, RED);
}

// draws a generic message box in the middle of the screen
void renderWaitMessage(const char* waitMessage)
{
    drawThickRectangle((Rectangle){204, 161, 408, 158}, THEME_ACCENT, 4);
    drawButton((Rectangle){208, 165, 400, 150}, FONT_LARGE, waitMessage, THEME_ACCENT, WHITE);
}

// draws a screen that alerts the user if they are taking the board out of range
void renderOutOfRangeScreen(void)
{
    const char* OUT_OF_RANGE_MESSAGE = "OUT OF RANGE, PLEASE RETURN TO PLAY AREA TO CONTINUE GAME";
    fillScreen(RED);
    drawString(FONT_LARGE, OUT_OF_RANGE_MESSAGE, centeredStringInScreenXCoord(OUT_OF_RANGE_MESSAGE, FONT_LARGE, DEFAULT_SPACE_SIZE), verticalPercentToYCoord(50) - 7, DEFAULT_SPACE_SIZE, YELLOW, RED, NO_ERASE);
}