#pragma once

#ifndef COIN_H
#define COIN_H

typedef struct Coin
{
    int coin;

} Coin;

extern Coin gCoin;

void InitializeCurrency();

void AddCoin(int amount);
void ReduceCoin(int amount);
bool UseCoin(int amount);

#endif