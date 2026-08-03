#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "../data/player.h"
#include "../data/table.h"

#include "../system/game_log.h"

void DrawGameLOGO(const char* gmaelog);
void RenderGame(Player *player, Player* aiPlayer, Table* table, GameLog *log);

void RenderGameReveal(Player* player, Player* aiPlayer, Table* table, GameLog *log);
void RenderLife(int life);

#endif