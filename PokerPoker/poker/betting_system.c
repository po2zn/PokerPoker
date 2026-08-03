#include <stdio.h>
#include <stdbool.h>
#include "betting_system.h"

#include "../audio/audio.h"

#include "../shop/shop_effect.h"
#include "../shop/coin.h"

// trash code. 함수의 의의를 져버린 쓰레기 코드 쯧쯧.
bool Check(Player *player, Table* table, GameLog *log)
{    
    if (player->totalBet < table->currentBet)
    {
        AddLog(log, LOG_SYSTEM,"체크할 수 없습니다. 먼저 콜(%d칩)이 필요합니다.", table->currentBet - player->totalBet);

        return false;
    }
    ProcessLuckyChip(gCoin.coin, log);

    PlaySFX(SOUND_CHECK);
    AddLog(log, LOG_SYSTEM,"체크 완료.");
    return true;
}

// == Raise
void Bet(Player* player, Table* table, int amount, GameLog* log)
{
    if (amount <= table->currentBet) 
    {
        AddLog(log, LOG_SYSTEM, "레이즈 금액은 현재 베팅(%d)보다 커야 합니다.", table->currentBet);
        return;
    }

    int needChip = amount - player->totalBet;

    if (needChip <= 0) 
    {
        AddLog(log, LOG_SYSTEM, "잘못된 베팅 금액입니다.");
        return;
    }

    if (player->chip < amount)
    {
        PlaySFX(SOUND_ALL_IN);
        AddLog(log, LOG_SYSTEM,"칩 부족 -> 올인 레이즈 (%d칩)", player->chip);
        amount = player->totalBet + player->chip;
        needChip = player->chip;
    }

    player->chip -= needChip;
    player->totalBet += needChip;
    table->pot += needChip;
    table->currentBet = player->totalBet;
    
    ProcessLuckyChip(gCoin.coin, log);
    PlaySFX(SOUND_ALL_IN);
    AddLog(log, LOG_SYSTEM,"레이즈 완료 : %d칩 추가 납부 → 누적 베팅 %d (새 currentBet=%d)", 
        needChip, player->totalBet, table->currentBet);
}

void Call(Player* player, Table* table, GameLog* log)
{
    int needChip = table->currentBet - player->totalBet;

    if (needChip <= 0)
    {
        AddLog(log, LOG_SYSTEM,"이미 콜 상태입니다.");
        return;
    }

    if (player->chip < needChip)
    {
        PlaySFX(SOUND_ALL_IN);
        AddLog(log, LOG_SYSTEM,"칩 부족 -> ALL IN 처리");

        table->pot += player->chip;
        player->totalBet += player->chip;
        player->chip = 0;

        return;
    }

    player->chip -= needChip;
    player->totalBet += needChip;
    table->pot += needChip;
    ProcessLuckyChip(gCoin.coin, log);
    PlaySFX(SOUND_CHECK);
    AddLog(log, LOG_SYSTEM,"콜 완료 : %d", player->totalBet);
}

void AllIn(Player *player, Table *table, GameLog* log)
{
    int amount = player->chip;
    if (amount <= 0)
    {
        AddLog(log, LOG_SYSTEM,"이미 올인 상태입니다.");
        return;
    }

    player->chip = 0;
    player->totalBet += amount;
    table->pot += amount;

    if (player->totalBet > table->currentBet)
    {
        table->currentBet = player->totalBet;
    }
    PlaySFX(SOUND_ALL_IN);
    AddLog(log, LOG_SYSTEM,"ALL IN !!! : %d칩 (누적 베팅 %d)", amount, player->totalBet);
}

void Fold(Player* player, GameLog* log)
{
    player->isFold = true;
    PlaySFX(SOUND_FOLD);
    PlaySFX(SOUND_LOSE);
    AddLog(log, LOG_SYSTEM,"Fold 했습니다.");
}