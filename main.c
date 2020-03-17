#include "main.h"

// We want this to be avaliable everywhere:
#include <stdbool.h>

// Low-level hardware utilities:
#include "hardware_utils/graphics/Graphics.c"
#include "hardware_utils/touchscreen/touchscreen.c"
#include "hardware_utils/gps/gps.c"
#include "hardware_utils/wifi/wifi.c"

// Screen rendering:
#include "main/graphics_buttons.h"

// WiFi comunications:
#include "hardware_utils/wifi/parsing.c"
#include "main/communication.c"

// Game-specific business-logic / data-structures:
#include "main/mygame.h"
#include "main/players/players.c"

// Standard C-library
#include <stdio.h>
#include <string.h>


/*
 * Main function.
 */
int main(void)
{
    // Scramble the random number generator:
    srand(time(NULL));

    // System initialization
    sysInit();

    // Start with the game's first screen:
    enum Screen nextScreen = WELCOME_SCREEN;
    while (1) {
        nextScreen = SCREEN_INTERACTORS[nextScreen]();
        if (nextScreen > (sizeof SCREEN_INTERACTORS / sizeof *SCREEN_INTERACTORS)) {
            // If the next screen is not a valid enum:
            nextScreen = WELCOME_SCREEN;
        }
    }
}


/*
 * Screen-specific functions for screen-specific actions.
 */

enum Screen welcomeScreen(void)
{
    (void)WELCOME_SCREEN;
    renderWelcomeScreen();

    // Initialize myGame struct
    initMyGame();
    // Continuously poll for touch-screen activity
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        if (isPointInRectangle(pt, WELCOME_START_GAME_BUTTON)) {
            // Direct to next screen in game flow if start button is pressed
            return TIME_SELECT_SCREEN;
        }
    }
    return ERROR_SCREEN;
}


enum Screen timeSelectionScreen(void)
{
    (void)TIME_SELECT_SCREEN;
    renderTimeSelectionScreen();

    if (myGame.gameDuration.hh != GAME_TIME_HH_MIN || myGame.gameDuration.mm != GAME_TIME_MM_MIN) {
        // If the values in myGame have their default values, we can
        // assuming that this is our first time rendering the screen
        // this game. If we have been through the screen before this
        // game, redraw the values that were set previously.
        renderTimeSelectUpdateHours(intToCharPtr(myGame.gameDuration.hh));
        renderTimeSelectUpdateMinutes(intToCharPtr(myGame.gameDuration.mm));
    } else {
        myGame.gameDuration.mm = GAME_TIME_MM_MIN;
        myGame.gameDuration.hh = GAME_TIME_HH_MIN;
    }

    // Continuously poll for touch-screen activity
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        /*
         * Button handlers:
         */
        // Next button handler
        if (isPointInRectangle(pt, NEXT_BUTTON)) {
            if ((myGame.gameDuration.mm > GAME_TIME_MM_MIN || myGame.gameDuration.hh > GAME_TIME_HH_MIN)) {
                // Direct to next screen in game flow if next button is pressed, and a valid time has been entered
                timeSetTotalSeconds(&myGame.gameDuration);
                return RANGE_SELECT_SCREEN;
            } else {
                renderErrorMessage("ERROR: INVALID DURATION");
                wait(2);
                renderTimeSelectionScreen();
                renderTimeSelectUpdateHours(intToCharPtr(myGame.gameDuration.hh));
                renderTimeSelectUpdateMinutes(intToCharPtr(myGame.gameDuration.mm));
            }
        }
        // Back button handler
        else if (isPointInRectangle(pt, BACK_BUTTON)) {
            // Direct to previous screen
            timeSetTotalSeconds(&myGame.gameDuration);
            return WELCOME_SCREEN;
        }
        // Increment hours button handler
        else if (isPointInRectangle(pt, TIME_SELECT_INCREMENT_HOURS_BUTTON)) {
            if (myGame.gameDuration.hh < GAME_TIME_HH_MAX) {
                renderTimeSelectUpdateHours(intToCharPtr(++(myGame.gameDuration.hh)));
            }
        }
        // Decrement hours button handler
        else if (isPointInRectangle(pt, TIME_SELECT_DECREMENT_HOURS_BUTTON)) {
            if (myGame.gameDuration.hh > GAME_TIME_HH_MIN) {
                renderTimeSelectUpdateHours(intToCharPtr(--(myGame.gameDuration.hh)));
            }
        }
        // Increment minutes button handler
        else if (isPointInRectangle(pt, TIME_SELECT_INCREMENT_MINUTES_BUTTON)) {
            if (myGame.gameDuration.mm < GAME_TIME_MM_MAX) {
                renderTimeSelectUpdateMinutes(intToCharPtr(myGame.gameDuration.mm += 5));
            }
        }
        // Decrement minutes button handler
        else if (isPointInRectangle(pt, TIME_SELECT_DECREMENT_MINUTES_BUTTON)) {
            if (myGame.gameDuration.mm > GAME_TIME_MM_MIN) {
                renderTimeSelectUpdateMinutes(intToCharPtr(myGame.gameDuration.mm -= 5));
            }
        }
    }
    return ERROR_SCREEN;
}


enum Screen rangeSelectionScreen(void)
{
    (void)RANGE_SELECT_SCREEN;
    renderRangeSelectionScreen();

    if (myGame.selectedGameRange != GAME_RANGE_MIN) {
        // If the range values in myGame have their default values,
        // we can assume that this is our first time rendering this
        // screen this game. If was have been on this screen before
        // this game, redraw the values that were set previously.
        renderRangeSelectUpdateRange(intToCharPtr(myGame.selectedGameRange));
    } else {
        myGame.selectedGameRange = GAME_RANGE_MIN;
    }

    // Continuously poll for touch-screen activity
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        /*
         * Button handlers:
         */
        // Next button handler
        if (isPointInRectangle(pt, NEXT_BUTTON)) {
            if (myGame.selectedGameRange > GAME_RANGE_MIN) {
                // Direct to next screen in game flow if start button
                // is pressed, and a valid range has been entered.
                return CONNECT_TO_SERVER_SCREEN;
            } else {
                renderErrorMessage("ERROR: INVALID RANGE");
                wait(2);
                renderRangeSelectionScreen();
                renderRangeSelectUpdateRange(intToCharPtr(myGame.selectedGameRange));
            }

        // Back button handler
        } else if (isPointInRectangle(pt, BACK_BUTTON)) {
            return TIME_SELECT_SCREEN;

        // Increment button handler
        } else if (isPointInRectangle(pt, RANGE_SELECT_INCREMENT_RANGE_BUTTON)) {
            if (myGame.selectedGameRange < GAME_RANGE_MAX) {
                // Update range value if incrementing it will not exceed the max range value
                myGame.selectedGameRange = myGame.selectedGameRange + GAME_RANGE_INCREMENT;
                // redraw field in screen
                renderRangeSelectUpdateRange(intToCharPtr(myGame.selectedGameRange));
            }

        // Decrement button handler
        } else if (isPointInRectangle(pt, RANGE_SELECT_DECREMENT_RANGE_BUTTON)) {
            if (myGame.selectedGameRange > GAME_RANGE_MIN) {
                // Update range value if decrementing it will result in an invalid value
                myGame.selectedGameRange = myGame.selectedGameRange - GAME_RANGE_INCREMENT;
                // redraw field in screen
                renderRangeSelectUpdateRange(intToCharPtr(myGame.selectedGameRange));
            }
        }
    }
    return ERROR_SCREEN;
}


enum Screen connectToServerScreen(void)
{
    (void)CONNECT_TO_SERVER_SCREEN;
    renderConnectToServerScreen(myGame.de1WifiIpAddress);

    // Continuously poll for touch-screen activity
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);

        processWiFi();

        if (timedOut) continue;

        /*
         * Button handlers:
         */
         // Next button handler, this starts the game
        if (isPointInRectangle(pt, NEXT_BUTTON)) {
            if (countActivePlayers(myGame.players) >= MIN_PLAYERS) {
                renderWaitMessage("Initializing Game..."); // the location initialization can sometimes be a slow process, so we update the user so they at least know that something is happening
                initGameLocationAndTime();
                myGame.currentHider = getRandomPlayer(); // initialize game with random starting player
                return HIDE_SCREEN;
            } else {
                renderErrorMessage("NOT ENOUGH PLAYERS IN GAME");
                wait(2);
                renderConnectToServerScreen(myGame.de1WifiIpAddress);
            }
        }
        // Back button handler
        else if (isPointInRectangle(pt, BACK_BUTTON)) {
        return RANGE_SELECT_SCREEN;
        }
    }
    return ERROR_SCREEN;
}


enum Screen playerHideScreen()
{
    (void)HIDE_SCREEN;
    renderPlayerHideScreen(myGame.currentHider);

    // Continuously poll for touch-screen activity
    int checkCount = 0;
    while (1) {
        if (checkCount == LOOP_CHECK_FREQUENCY) {
            // to save on computation time, we only check the location
            // every LOOP_CHECK_FREQUENCY times around the loop. Due to
            // the nature of this application, it was deemed worth losing
            // the precision.
            checkCount = 0;
            // check range
            struct location currentLocation = getLocation();
            if (getDistance(myGame.gameStartLocation, currentLocation) > (double)myGame.selectedGameRange) {
                outOfRange(); // wait in the out of range function until the game board is brought back in range
                renderPlayerHideScreen(myGame.currentHider);
            }
        } else {
            checkCount++;
        }

        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        /*
         * Button handlers:
         */
         // Hide button handler
        if (isPointInRectangle(pt, HIDE_SCREEN_HIDE_BUTTON)) {
            myGame.roundStartLocation = getLocation(); // a new round of the game is starting here
            myGame.roundStartTime = getTime();
            return ENTER_PIN_SCREEN;

        // End game button handler
        } else if (isPointInRectangle(pt, HIDE_SCREEN_END_GAME_BUTTON)) {
            return END_GAME_CONFIRM_SCREEN;
        }
    }
    return ERROR_SCREEN;
}

enum Screen endGameConfirmationScreen()
{
    (void)END_GAME_CONFIRM_SCREEN;
    renderEndGameConfirmationScreen();
    // Continuously poll for touch-screen activity
    while (1)
    {
        // TODO: do we need to add this back?
        // Do this before the touchscreen handling so it doesn't get skipped by the continue statement.
        //processWiFi();

        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        /*
         * Button handlers:
         */
         // Yes button handler
        if (isPointInRectangle(pt, END_GAME_CONFIRMATION_SCREEN_YES_BUTTON)) {
            myGame.roundEndTime = getTime(); // moving to game over now, calculate final score for current hider
            myGame.currentHider->score += calcScoreDeltaFromTime(
                getRecord(myGame.roundStartTime, myGame.roundEndTime)
            );
            return GAME_OVER_SCREEN;
        }
        // No button handler
        else if (isPointInRectangle(pt, END_GAME_CONFIRMATION_SCREEN_NO_BUTTON)) {
            return HIDE_SCREEN;
        }
    }
    return ERROR_SCREEN;
}


enum Screen enterPinScreen(void)
{
    (void)ENTER_PIN_SCREEN;
    renderEnterPinScreen();

    const Rectangle NUMPAD[10] = {
        PIN_NUMPAD_0, PIN_NUMPAD_1, PIN_NUMPAD_2, PIN_NUMPAD_3, PIN_NUMPAD_4,
        PIN_NUMPAD_5, PIN_NUMPAD_6, PIN_NUMPAD_7, PIN_NUMPAD_8, PIN_NUMPAD_9
    };
    unsigned int pin = 0;
    unsigned int pinIndex = 0;

    // Continuously poll for touch-screen activity
    int checkCount = 0;
    while (1) {
        if (checkCount == LOOP_CHECK_FREQUENCY) {
            // only perform the expensive check location and check time
            // operations once every LOOP FREQUENCY times around the loop
            // to save on computation
            checkCount = 0;
            // Check board location is valid
            struct location currentLocation = getLocation();
            if (getDistance(myGame.gameStartLocation, currentLocation) > (double)myGame.selectedGameRange) {
                outOfRange(); // wait in out of range function until the game board is brought back in range
                return ENTER_PIN_SCREEN;
            }

            // Check if the game time is reached:
            struct time currentTime = getTime();
            if (currentTime.totalSeconds > (myGame.gameDuration.totalSeconds + myGame.gameStartTime.totalSeconds)) {
                myGame.roundEndTime = getTime();
                myGame.currentHider->score += calcScoreDeltaFromTime(
                    getRecord(myGame.roundStartTime, myGame.roundEndTime)
                );
                return GAME_OVER_SCREEN;
            }
        } else {
            checkCount++;
        }

        // Do this before the touchscreen handling so it doesn't get skipped by the continue statement.
        processWiFi();

        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;

        // Pinpad buttons handler
        if (pinIndex < GAME_PIN_LENGTH && isPointInRectangle(pt, PIN_NUMPAD)) {
            for (int i = 0; i < 10; i++) {
                if (isPointInRectangle(pt, NUMPAD[i])) {
                    /*
                     * Handle pinpad activity if < GAME_PIN_LENGTH numbers have been entered.
                     */
                    char pinEntered[2] = "";
                    pinEntered[0] = i + '0';

                    // Draw the entered pin on the screen
                    drawString(FONT_LARGE, pinEntered, 400 + 70 * pinIndex, 270, DEFAULT_SPACE_SIZE, BLACK, WHITE, 0);

                    pin = pin * 10 + i;
                    pinIndex++;
                    break;
                }
            }
        }
        // Delete button handler
        else if (isPointInRectangle(pt, PIN_DELETE_BUTTON)) {
            // Remove the last-entered number from the pin number
            pinIndex--;
            char pinToDelete[2] = "";
            pinToDelete[0] = (pin % 10) + '0';
            // Draw the new, shorter pin on the screen
            drawString(FONT_LARGE, pinToDelete, 400 + 70 * pinIndex, 270, DEFAULT_SPACE_SIZE, WHITE, WHITE, 1);

            pin /= 10;
        }
        // Enter button handler
        else if (isPointInRectangle(pt, PIN_ENTER_BUTTON)) {
            if (getPlayerByPinNumber(pin)->pinNumber != USER_PIN_UNASSIGNED) {
                // finished one round of the game, calculate current
                // hider's score and show new hiders stats before starting
                // new round.
                myGame.roundEndTime = getTime();
                myGame.currentHider->score += calcScoreDeltaFromTime(
                    getRecord(myGame.roundStartTime, myGame.roundEndTime)
                );
                myGame.currentHider = getPlayerByPinNumber(pin);
                return STAT_SCREEN;

            } else {
                // Draw a "INCORRECT PIN" error message
                drawString(FONT_LARGE, "INCORRECT PIN", 452, 230, DEFAULT_SPACE_SIZE, RED, WHITE, 0);

                // Clear the incorrect pin number entered from the screen
                for (int i = 3; i > -1; i--) {
                    char pinToDelete[2];
                    pinToDelete[0] = pin % 10 + '0';

                    // Draw the wrong pin over with the background color to "erase" it from view
                    drawString(FONT_LARGE, pinToDelete, 400 + 70 * i, 270, DEFAULT_SPACE_SIZE, THEME_BACKGROUND, THEME_BACKGROUND, 1);

                    pin /= 10;
                }

                pin = 0;
                pinIndex = 0;
            }
        }
    }
    return ERROR_SCREEN;
}


enum Screen statScreen(void)
{
    sortPlayersByScore(myGame.players);
    struct time hiddenTimeStruct = getRecord(myGame.roundStartTime, myGame.roundEndTime);
    int hiddenTime = (int)calcScoreDeltaFromTime(hiddenTimeStruct);

    (void)STAT_SCREEN;
    renderStatScreen(
        myGame.currentHider,
        intToCharPtr(hiddenTime),
        intToCharPtr(countActivePlayers(myGame.players))
    );

    for (int i = 0; i < countActivePlayers(myGame.players); i++) {
        struct Player* p = myGame.players + i;
    }
    // Continuously poll for touch-screen activity
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;
        if (isPointInRectangle(pt, STAT_SCREEN_CONTINUE_BUTTON)) {
            return HIDE_SCREEN;
        }
    }
    return ERROR_SCREEN;
}


enum Screen gameOverScreen(void)
{
    (void)GAME_OVER_SCREEN;
    renderGameOverScreen();

    myGame.gameOver = 1;
    sortPlayersByScore(myGame.players);

    // Wait for user to request returning to the main menu to start a new game:
    while (1) {
        volatile bool timedOut;
        const Point pt = GetRelease(1, &timedOut);
        if (timedOut) continue;
        if (isPointInRectangle(pt, GAME_OVER_MAIN_MENU_BUTTON)) {
            return WELCOME_SCREEN;
        }
    }
    return ERROR_SCREEN;
}


enum Screen errorScreen(void)
{
    // If the code got here, that means it managed to get
    // past a while(1) loop, which should never happen.
    renderErrorMessage("SCREEN ESCAPED POLLING LOOP");
    delay(5.0);
    return WELCOME_SCREEN;
}


/*
 * Other functions.
 */
void sysInit(void)
{
    Init_GPS();
    Init_Graphics();
    Init_Touch();
    Init_WiFi(myGame.de1WifiIpAddress);
}


void initGameLocationAndTime(void)
{
    myGame.gameStartLocation = getLocation();
    myGame.gameStartTime = getTime();
}


void outOfRange(void)
{
    renderOutOfRangeScreen();
    // wait in out of range screen until the board is moved into the game range
    while (1) {
        struct location currentLocation = getLocation();
        if ((int) getDistance(myGame.gameStartLocation, currentLocation) <= myGame.selectedGameRange) {
            return;
        }
    }
}


// reset fields of myGame before game start
void initMyGame(void)
{
    // Initialize initial values for all the types found in myGame
    const struct time       initialTime     = { .hh = 0, .mm = 0, .ss = 0, .totalSeconds = 0 };
    const struct location   initialLocation = { .lat = 0, .lon = 0 };
    const struct Player     initialPlayer   = { .username = { 0 }, .pinNumber = USER_PIN_UNASSIGNED, .score = 0 };

    // Initialize all fields in myGame, except for de1WifiIpAddress which should remain the same on subsequent games
    myGame.gameDuration     = initialTime;
    myGame.gameStartTime    = initialTime;
    myGame.roundStartTime   = initialTime;
    myGame.roundEndTime     = initialTime;

    myGame.selectedGameRange    = 0;
    myGame.gameOver             = 0;
    myGame.gameStartLocation    = initialLocation;
    myGame.roundStartLocation   = initialLocation;

    // Arbitrarily set the current hider to be the first entry
    // (which is not even an existing player yet). This should
    // be an easy way to avoid checks for when the board is first
    // being hidden and there is no previous hider.
    myGame.currentHider = myGame.players;
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
        myGame.players[i] = initialPlayer;
    }
}
