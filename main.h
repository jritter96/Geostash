#ifndef _MAIN_H_
#define _MAIN_H_

#define GAME_TIME_MM_MIN 0
#define GAME_TIME_MM_MAX 55
#define GAME_TIME_HH_MIN 0
#define GAME_TIME_HH_MAX 23

#define GAME_RANGE_MIN 0
#define GAME_RANGE_MAX 2000
#define GAME_RANGE_INCREMENT 100

#define GAME_PIN_LENGTH 4
#define MIN_PLAYERS 2

#define LOOP_CHECK_FREQUENCY 1000

enum Screen {
    WELCOME_SCREEN,
    TIME_SELECT_SCREEN,
    RANGE_SELECT_SCREEN,
    CONNECT_TO_SERVER_SCREEN,
    HIDE_SCREEN,
    END_GAME_CONFIRM_SCREEN,
    ENTER_PIN_SCREEN,
    STAT_SCREEN,
    GAME_OVER_SCREEN,
    ERROR_SCREEN,
};

/*
 * Main function.
 */
int main(void);

/*
 * Screen-specific functions for screen-specific actions.
 */
enum Screen welcomeScreen(void);
enum Screen timeSelectionScreen(void);
enum Screen rangeSelectionScreen(void);
enum Screen connectToServerScreen(void);
enum Screen playerHideScreen(void);
enum Screen endGameConfirmationScreen(void);
enum Screen enterPinScreen(void);
enum Screen statScreen(void);
enum Screen gameOverScreen(void);
enum Screen errorScreen(void);


// Make sure that the order of entries in this is exactly
// the same as that in the above enum definition.
enum Screen (*SCREEN_INTERACTORS[])(void) = {
    welcomeScreen,
    timeSelectionScreen,
    rangeSelectionScreen,
    connectToServerScreen,
    playerHideScreen,
    endGameConfirmationScreen,
    enterPinScreen,
    statScreen,
    gameOverScreen,
    errorScreen,
};


/*
 * Other functions.
 */
void sysInit(void);
void initGameLocationAndTime(void);
void outOfRange(void);
void initMyGame(void);
void setUpDemo(void);


#endif