#include <stdio.h>

#include "betting_system.h"

void Check(Player *player, Table* table)
{
    if (player->totalBet < table->currentBet)
    {
        printf("\n체크할 수 없습니다. 먼저 콜(%d칩)이 필요합니다.\n", table->currentBet - player->totalBet);

        return;
    }

    printf("\n체크 완료.\n");
}

// == Raise
void Bet(Player* player, Table* table, int amount)
{
    if (amount <= table->currentBet) 
    {
        printf("\n레이즈 금액은 현재 베팅(%d)보다 커야 합니다.\n", table->currentBet);
        return;
    }

    int needChip = amount - player->totalBet;

    if (needChip <= 0) 
    {
        printf("\n잘못된 베팅 금액입니다.\n");
        return;
    }

    if (player->chip < amount)
    {
        printf("\n칩 부족 -> 올인 레이즈 (%d칩)\n", player->chip);
        amount = player->totalBet + player->chip;
        needChip = player->chip;
    }

    
    player->chip -= needChip;

    player->totalBet += needChip;

    table->pot += needChip;

    table->currentBet = player->totalBet;

    printf("\n레이즈 완료 : %d칩 추가 납부 → 누적 베팅 %d (새 currentBet=%d)\n", 
        needChip, player->totalBet, table->currentBet);
}

void Call(Player* player, Table* table)
{
    int needChip = table->currentBet - player->totalBet;

    if (needChip <= 0)
    {
        printf("\n이미 콜 상태입니다.\n");
        return;
    }

    if (player->chip < needChip)
    {
        printf("\n칩 부족 -> ALL IN 처리\n");

        table->pot += player->chip;
        player->totalBet += player->chip;
        player->chip = 0;

        return;
    }

    player->chip -= needChip;
    player->totalBet += needChip;
    table->pot += needChip;

    printf("\n콜 완료 : %d\n", player->totalBet);
}

void AllIn(Player *player, Table *table) 
{
    int amount = player->chip;
    if (amount <= 0)
    {
        printf("\n이미 올인 상태입니다.\n");
        return;
    }

    player->chip = 0;
    player->totalBet += amount;
    table->pot += amount;

    if (player->totalBet > table->currentBet)
    {
        table->currentBet = player->totalBet;
    }

    printf("\nALL IN !!! : %d칩 (누적 베팅 %d)\n", amount, player->totalBet);
}

void Fold(Player* player)
{
    player->isFold = true;

    printf("\nFold 했습니다.\n");
}