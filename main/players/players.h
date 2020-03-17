#ifndef _PLAYERS_H_
#define _PLAYERS_H_

#include "../../hardware_utils/gps/gps.h"


// NOTE: determined by the available space on post game results screen
#define MAX_PLAYERS_PER_GAME 10

// Does not count a trailing null character.
#define USERNAME_MAX_LENGTH 30

// A four-digit number.
typedef unsigned int user_pin_t;
// If a Player struct has this as its pinNumber, then the struct
// is currently free (not representing a player).
#define USER_PIN_UNASSIGNED 0

/**
 * Score is in units of seconds.
 */
struct Player {
    char username[USERNAME_MAX_LENGTH + 1];
    user_pin_t pinNumber;
    double score;
};

// Returns the newly registered player's pin number.
user_pin_t registerPlayer(char const*const username);

// Returns the number of active players in the game
int countActivePlayers(struct Player players[MAX_PLAYERS_PER_GAME]);

// Returns NULL if a player by the given pin number does not exist.
struct Player* getPlayerByPinNumber(user_pin_t);

struct Player* sortPlayersByScore(struct Player players[MAX_PLAYERS_PER_GAME]);

int getPlayerPosition(struct Player const*const player);

struct Player* getRandomPlayer();

double calcScoreDeltaFromTime(struct time deltaTime);

#endif