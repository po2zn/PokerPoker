#include <stdlib.h>

#include "shop_effect.h"
#include "shop_inventory.h"

#include "../dealer/dealer.h"

#include "../system/game_log.h"

void ProcessLuckyChip(int* coin, GameLog *log)
{
    if (coin == NULL) return;
	if (log == NULL) return;

    if (!gInventory.isLuckyChip)
    {
        return;
    }

    int chance = rand() % 100;

    if (chance < 45)
    {
        coin += 20;
        AddLog(log, LOG_SYSTEM, " ★[Lucky Chip] 바닥에서 코인 10개를 주웠습니다!!! ★");
    }
}

void ProcessVIPTable(Table *table)
{
    if (!gInventory.isVIPTable)
    {
        return;
    }

    RevealFlop(table);
	RevealTurn(table);
    RevealRiver(table);
    return;
}