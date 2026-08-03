#pragma once
#ifndef CARD_H
#define CARD_H

typedef enum
{
    SPADE = 0,
    HEART,
    DIAMOND,
    CLUB
} Suit;

typedef struct Card
{
    Suit suit;
    int number;
} Card;

#endif