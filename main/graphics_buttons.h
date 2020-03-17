#ifndef _GRAPHICS_BUTTONS_H_
#define _GRAPHICS_BUTTONS_H_

#include "../hardware_utils/touchscreen/touchscreen.h"

/*
 * Rectangles used by both touchscreen press handling and button drawing.
 */

#define BUTTON_SIZE 55
#define BUTTON_SPACE 70
#define START_X 80
#define START_Y 200

/*
 * All GUI buttons, by screen.
 */

// common buttons
const Rectangle NEXT_BUTTON =                             {684, 419, 100, 60};
const Rectangle BACK_BUTTON =                             {32, 419, 100, 60};

// Welcome Screen button
const Rectangle WELCOME_START_GAME_BUTTON =               {200, 300, 400, 100};

// Time-Select Screen buttons
const Rectangle TIME_SELECT_INCREMENT_HOURS_BUTTON =      {318, 180, 50, 55};
const Rectangle TIME_SELECT_DECREMENT_HOURS_BUTTON =      {318, 245, 50, 55};
const Rectangle TIME_SELECT_INCREMENT_MINUTES_BUTTON =    {558, 180, 50, 55};
const Rectangle TIME_SELECT_DECREMENT_MINUTES_BUTTON =    {558, 245, 50, 55};

// Range-Select Screen buttons
const Rectangle RANGE_SELECT_INCREMENT_RANGE_BUTTON =     {493, 180, 50, 55};
const Rectangle RANGE_SELECT_DECREMENT_RANGE_BUTTON =     {493, 245, 50, 55};

// Hide Screen Buttons
const Rectangle HIDE_SCREEN_HIDE_BUTTON =                 {208, 185, 400, 100};
const Rectangle HIDE_SCREEN_END_GAME_BUTTON =             {208, 335, 400, 100};

// Stat Screen buttons
const Rectangle STAT_SCREEN_CONTINUE_BUTTON =             {634, 419, 150, 60};

// End Game Confirmation Screen buttons
const Rectangle END_GAME_CONFIRMATION_SCREEN_YES_BUTTON = {326, 247, 70, 50};
const Rectangle END_GAME_CONFIRMATION_SCREEN_NO_BUTTON =  {421, 247, 70, 50};

// Game Over Screen buttons
const Rectangle GAME_OVER_MAIN_MENU_BUTTON =              {308, 399, 200, 70};

// Connect to Server Screen button

// Pin Screen buttons
const Rectangle PIN_NUMPAD = {START_X, START_Y, BUTTON_SIZE * 4, BUTTON_SIZE * 5};
const Rectangle PIN_NUMPAD_0 = { START_X + BUTTON_SPACE, START_Y + 3 * BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_1 = { START_X, START_Y, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_2 = { START_X + BUTTON_SPACE, START_Y, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_3 = { START_X + 2 * BUTTON_SPACE, START_Y, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_4 = { START_X , START_Y + BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_5 = { START_X + BUTTON_SPACE, START_Y + BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_6 = { START_X + 2 * BUTTON_SPACE, START_Y + BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_7 = { START_X, START_Y + 2 * BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_8 = { START_X + BUTTON_SPACE, START_Y + 2 * BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_NUMPAD_9 = { START_X + 2 * BUTTON_SPACE, START_Y + 2 * BUTTON_SPACE, BUTTON_SIZE, BUTTON_SIZE };
const Rectangle PIN_ENTER_BUTTON =                         {600, 340, 120, 55};
const Rectangle PIN_DELETE_BUTTON =                        {350, 340, 120, 55};

#endif /* _GRAPHICS_BUTTONS_H_ */