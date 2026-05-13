#pragma once
#ifndef GAME_FLOW_H
#define GAME_FLOW_H

#include "../data/player.h"
#include "../data/table.h"
#include "../ai/ai.h"

void ShowCommunityCards(Table* table);

void ShowPlayerCards(Player* player);

void ShowAICards(Player* ai);

void PlayRound(Player* player, Player* aiplayer, AIContext* ai);

void RunPokerGame();

#endif