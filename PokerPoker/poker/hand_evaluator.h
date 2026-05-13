#pragma once

#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include "../data/card.h"

#include "hand_rank.h"

typedef struct HandResult
{
    HandRank rank;
    // 1~14
    int primaryValue;

    // 투페어 보조값
    int secondaryValue;

    int kicker[5];

} HandResult;

HandResult EvaluateHand(Card cards[], int cardCount);

int CompareHands(HandResult player, HandResult ai);

const char* GetHandRankName(HandRank rank);

HandResult FindBestHand(Card cards[], int cardCount);

#endif