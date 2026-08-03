#ifndef PLAYER_H
#define PLAYER_H

#define MAX_HAND_CARD 3
#include <stdbool.h>

#include "card.h"

typedef struct Player
{
    // init Data
    int chip;
    int totalBet;
    bool isFold;

    // Custom Data
    Card hand[MAX_HAND_CARD];
    int handCardCount;

    int aiLevel;

    // shop_item
    bool hasUsedCardSwap;

    int LIFE;

} Player;

void InitializePlayer(Player* player);
bool IsAI(Player* player);

#endif