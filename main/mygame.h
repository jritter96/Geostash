#ifndef _MYGAME_H_
#define _MYGAME_H_

#include "../hardware_utils/gps/gps.h"
#include "players/players.h"

/*
 * myGame struct, representing important characteristics
 * of the current game session.
 */
struct {
    char de1WifiIpAddress[16];
    struct time gameDuration;
    struct time gameStartTime;
    struct time roundStartTime;
    struct time roundEndTime;
    int selectedGameRange;
    bool gameOver;
    struct location gameStartLocation;
    struct location roundStartLocation;
    struct Player players[MAX_PLAYERS_PER_GAME];
    struct Player* currentHider;
} myGame;

#endif