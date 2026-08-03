#pragma once
#ifndef AI_UPDATE_H
#define AI_UPDATE_H

#include "ai.h"
#include "../system/game_log.h"

void UpdateAI(AIContext* ai, Player* aiPlayer, Table* table, Deck* deck, int playerAction, GameLog* log);

#endif