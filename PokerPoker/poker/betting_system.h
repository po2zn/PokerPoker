#pragma once
#ifndef BETTING_SYSTEM_H
#define BETTING_SYSTEM_H

#include "../data/player.h"
#include "../data/table.h"

#include "../system/game_log.h"

bool Check(Player* player, Table* table, GameLog* log);
void Bet(Player* player, Table* table, int amount, GameLog* log);
void Call(Player* player, Table* table, GameLog* log);
void AllIn(Player* player, Table* table, GameLog* log);
void Fold(Player* player, GameLog* log);

#endif