#pragma once
#ifndef GAME_FLOW_H
#define GAME_FLOW_H

#include "../data/player.h"
#include "../data/table.h"
#include "../ai/ai.h"

#include "../system/game_log.h"

void ShowCommunityCards(Table* table, GameLog* log);

void ShowPlayerCards(Player* player, GameLog* log);

void ShowAICards(Player* ai, GameLog* log);

void PlayRound(Player* player, Player* aiplayer, AIContext* ai, GameLog *log);

void RunPokerGame();

#endif