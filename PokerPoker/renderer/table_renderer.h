#pragma once
#ifndef TABLE_RENDERER_H
#define TABLE_RENDERER_H

#include "screen_buffer.h"

#include "../data/table.h"
#include "../data/player.h"


void DrawHiddenCard(ScreenBuffer* buffer, int x, int y);
void DrawPokerTableReveal(ScreenBuffer* buffer, Player* player, Player* aiPlayer, Table* table);
void DrawPokerTable(ScreenBuffer* buffer, Player* player, Player* aiPlayer, Table* table);

#endif