#include "dealer.h"

void DealCards(Deck* deck, Player* player)
{
    for (int i = 0; i < 2; i++)
    {
        player->hand[i] = DrawCard(deck);
    }
}

void SetCommunityCards(Deck* deck, Table* table)
{
    for (int i = 0; i < 5; i++)
    {
        table->communityCards[i] = DrawCard(deck);
    }

    table->revealedCardCount = 0;
}

void RevealFlop(Table* table)
{
    table->revealedCardCount = 3;
}

void RevealTurn(Table* table)
{
    table->revealedCardCount = 4;
}

void RevealRiver(Table* table)
{
    table->revealedCardCount = 5;
}

void SetBlind(Player* player, Player* ai, Table* table, int blindAmount)
{
    int playerBlind, aiBlind;

    if (player->chip >= blindAmount) playerBlind = blindAmount;
    else playerBlind = player->chip;

    if (ai->chip >= blindAmount) aiBlind = blindAmount;
    else aiBlind = ai->chip;

    player->chip -= playerBlind;
    player->totalBet += playerBlind;
    
    ai->chip -= aiBlind;
    ai->totalBet += aiBlind;

    table->pot += playerBlind + aiBlind;

    table->currentBet = blindAmount;
}