#include <stdbool.h>

#include "coin.h"

Coin gCoin;

void InitializeCurrency()
{
    gCoin.coin = 0;
}

void AddCoin(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    gCoin.coin += amount;
}

void ReduceCoin(int amount)
{
    if (amount <= 0)
    {
        return;
    }
    if (gCoin.coin < amount)
    {
        gCoin.coin = 0;
    }
    else
    {
        gCoin.coin -= amount;
    }
}

bool UseCoin(int amount)
{
    if (amount <= 0)
    {
        return false;
    }

    if (gCoin.coin < amount)
    {
        return false;
    }

    return true;
}