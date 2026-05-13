#pragma once

#ifndef AI_H
#define AI_H

#include "../data/player.h"
#include "../data/table.h"
#include "../data/deck.h"

#include "../enum/action_type.h"
#include "../enum/ai_state.h"


typedef struct AIContext
{
    AIState state;
    float winRate;

    ActionType selectedAction;
    int raiseAmount;

    bool isBluffing;

} AIContext;

void InitializeAI(AIContext* ai);

float CalculateWinRate(Player* aiPlayer, Table* table, Deck* deck);

ActionType DecideAIAction(AIContext* ai, Player* aiPlayer, Table* table, Deck *deck, int playerAction);

void ExecuteAIAction(AIContext* ai, Player* aiPlayer, Table* table);

#endif