#pragma once

#ifndef DECK_H
#define DECK_H

#include "card.h"

#define DECK_SIZE 52

typedef struct Deck
{
    Card cards[DECK_SIZE];
    int topIndex;
} Deck;

void InitializeDeck(Deck* deck);

void ShuffleDeck(Deck* deck);

Card DrawCard(Deck* deck);

#endif