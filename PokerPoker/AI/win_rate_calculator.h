#pragma once

#include "../data/player.h"
#include "../data/table.h"
#include "../data/deck.h"

#include "../poker/hand_evaluator.h"

int CompareHandResult(HandResult a, HandResult b);
float CalculateWinRate(Player* aiPlayer, Table* table, Deck* deck);
