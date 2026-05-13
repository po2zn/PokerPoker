#ifndef TABLE_H
#define TABLE_H

#include "card.h"

typedef struct Table
{
    int pot;
    int sidePot;
    int currentBet;

    Card communityCards[5];
    int revealedCardCount;

} Table;

void InitializeTable(Table* table);

#endif