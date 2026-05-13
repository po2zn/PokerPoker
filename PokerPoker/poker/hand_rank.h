#pragma once

#ifndef HAND_RANK_H
#define HAND_RANK_H

// 0부터 약함 순, HIGH_CARD < ONE_PAIR 느낌
typedef enum
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH

} HandRank;

#endif