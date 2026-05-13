#pragma once

#ifndef DEALER_H
#define DEALER_H

#include "../data/deck.h"
#include "../data/player.h"
#include "../data/table.h"

void DealCards(Deck* deck, Player* player);

void SetCommunityCards(Deck* deck, Table* table);

void RevealFlop(Table* table);
void RevealTurn(Table* table);
void RevealRiver(Table* table);

void SetBlind(Player* player, Player* ai, Table* table, int blindAmount);

#endif