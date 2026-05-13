#pragma once
#ifndef BETTING_SYSTEM_H
#define BETTING_SYSTEM_H

#include "../data/player.h"
#include "../data/table.h"

void Check(Player *player, Table* table);
void Bet(Player* player, Table* table, int amount);
void Call(Player* player, Table* table);
void AllIn(Player* player, Table* table);
void Fold(Player* player);

#endif