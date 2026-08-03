#include "player.h"

void InitializePlayer(Player* player)
{
    player->chip = 50;
    player->totalBet = 0;
    player->isFold = false;
    player->hasUsedCardSwap = false;

    player->handCardCount = 2;
}

bool IsAI(Player* player)
{
    return player->aiLevel > 0;
}