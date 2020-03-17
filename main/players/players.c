#include "players.h"

#include "../mygame.h"

#include <stdlib.h>
#include <time.h>

user_pin_t registerPlayer(char const*const username) {
    int unusedPlayerIndex = 0;
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
        if (myGame.players[i].pinNumber == USER_PIN_UNASSIGNED) {
            unusedPlayerIndex = i;
            break;
        }
    }

    user_pin_t pinNumber;
    bool pinAlreadyUsed;
    do {
        pinAlreadyUsed = false;
        pinNumber = rand() % 9999;
        if (pinNumber == USER_PIN_UNASSIGNED) continue;
        for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
            if (myGame.players[i].pinNumber == USER_PIN_UNASSIGNED) {
                continue;
            }
            if (pinNumber == myGame.players[i].pinNumber) {
                pinAlreadyUsed = true;
                break;
            }
        }
    } while (pinAlreadyUsed);

    myGame.players[unusedPlayerIndex].pinNumber = pinNumber;
    strcpy(myGame.players[unusedPlayerIndex].username, username);
    return pinNumber;
}

int countActivePlayers(struct Player players[MAX_PLAYERS_PER_GAME]) {
    int count = 0;
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
        if (players[i].pinNumber != USER_PIN_UNASSIGNED) {
            count++;
        }
    }
    return count;
}

struct Player* getPlayerByPinNumber(user_pin_t pinNumber) {
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++)
    {
        if (myGame.players[i].pinNumber == pinNumber) {
            return myGame.players + i;
        }
    }
    return NULL;
}

struct Player* sortPlayersByScore(struct Player players[MAX_PLAYERS_PER_GAME])
{
    user_pin_t hiderPin = myGame.currentHider->pinNumber;
    struct Player temp;
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
        for (int j = i + 1; j < MAX_PLAYERS_PER_GAME - i; j++) {
            if (players[i].score < players[j].score) {
                temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }
    // Update the pointer to point to the entry that has now moved.
    myGame.currentHider = getPlayerByPinNumber(hiderPin);
    return players;
}

int getPlayerPosition(struct Player const*const player) {
    for (int i = 0; i < MAX_PLAYERS_PER_GAME; i++) {
        if (myGame.players + i == player) {
            return i + 1;
        }
    }
    return -1;
}

struct Player* getRandomPlayer(void)
{
    int numberOfPlayers = countActivePlayers(myGame.players);
    int index = (rand() % (numberOfPlayers));
    return myGame.players + index;
}

double calcScoreDeltaFromTime(struct time deltaTime) {
    return (double)deltaTime.totalSeconds / 60.0;
}
