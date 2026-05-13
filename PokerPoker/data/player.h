#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "card.h"

typedef struct Player
{
    // init Data
    int chip;
    int totalBet;
    bool isFold;

    // Custom Data
    Card hand[2];
    int aiLevel;

} Player;

void InitializePlayer(Player* player);
bool IsAI(Player* player);

#endif