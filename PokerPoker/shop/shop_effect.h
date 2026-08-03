#pragma once

#ifndef SHOP_EFFECT_H
#define SHOP_EFFECT_H

#include "../system/game_log.h"
#include "../data/table.h"

void ProcessLuckyChip(int* coin, GameLog *log);
void ProcessVIPTable(Table* table);

#endif