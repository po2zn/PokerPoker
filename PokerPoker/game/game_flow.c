#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "../audio/audio.h"

#include "../ai/ai.h"
#include "../ai/ai_update.h"

#include "../data/card_printer.h"
#include "../data/deck.h"
#include "../data/player.h"
#include "../data/table.h"

#include "../dealer/dealer.h"

#include "../poker/hand_evaluator.h"
#include "../poker/betting_system.h"

#include "../enum/action_type.h"

#include "../renderer/renderer.h"
#include "../renderer/screen_buffer.h"

#include "../system/input.h"
#include "../system/game_log.h"

#include "../shop/shop.h"
#include "../shop/shop_effect.h"
#include "../shop/coin.h"
#include "../shop/shop_inventory.h"

#include "card_swap.h"

static ActionType PlayerTurn(Player* player, Player *aiPlayer, Table* table, Deck* deck, GameLog *log);
static void ApplyAIAction(AIContext *ai, Player *aiPlayer, Table *table, GameLog *log);
// 임시
void ShowCommunityCards(Table* table, GameLog *log)
{
    AddLog(log, LOG_SYSTEM, "=== Community Cards ===");

    for (int i = 0; i < table->revealedCardCount; i++)
    {
        PrintCard(table->communityCards[i]);
        printf(" ");
    }

    printf("\n");
}
// 임시
void ShowPlayerCards(Player* player, GameLog *log)
{
    AddLog(log, LOG_SYSTEM, "=== PLAYER CARD ===");

    for (int i = 0; i < player->handCardCount; i++)
    {
        PrintCard(player->hand[i]);

        printf(" ");
    }

    printf("\n");
}

// 임시
void ShowAICards(Player* ai, GameLog *log)
{
    AddLog(log, LOG_SYSTEM, "=== AI CARD ===");

    for (int i = 0; i < ai->handCardCount; i++)
    {
        PrintCard(ai->hand[i]);

        printf(" ");
    }

    printf("\n");
}

static void PrintTableState(Player* player, Player* ai, Table* table, GameLog *log)
{
    AddLog(log, LOG_ACTION, "[pot: %d]  [currentBet: %d]  [playerChip: %d]  [AIChip: %d]",
        table->pot, table->currentBet, player->chip, ai->chip);
}

static void Showdown(Player* player, Player* aiPlayer, Table* table, GameLog *log)
{
    if (player->totalBet != aiPlayer->totalBet)
    {
        int ex;
        // 환급
        if (aiPlayer->totalBet < player->totalBet)
        {
            ex = player->totalBet - aiPlayer->totalBet;
            player->chip += ex;
            table->pot -= ex;
        }
        else 
        {
            ex = aiPlayer->totalBet - player->totalBet;
            aiPlayer->chip += ex;
            table->pot -= ex;
        }
    }
    // ==========================================
    // player
    int playerTotalCards = player->handCardCount + 5;
    Card playerCards[8];

    for (int i = 0; i < player->handCardCount; i++)
    {
        playerCards[i] = player->hand[i];
    }

    for (int i = 0; i < 5; i++)
    {
        playerCards[player->handCardCount + i] = table->communityCards[i];
    }
    // ==========================================
    // ai
    int aiTotalCards = aiPlayer->handCardCount + 5;
    Card aiCards[8];

    for (int i = 0; i < aiPlayer->handCardCount; i++)
    {
        aiCards[i] = aiPlayer->hand[i];
    }

    for (int i = 0; i < 5; i++)
    {
        aiCards[aiPlayer->handCardCount + i] = table->communityCards[i];
    }
    // ==========================================
    

    // 커뮤니티
    for (int i = 0; i < 5; i++)
    {
        playerCards[i + 2] = table->communityCards[i];
        aiCards[i + 2] = table->communityCards[i];
    }

    HandResult playerBest = FindBestHand(playerCards, playerTotalCards);
    HandResult aiBest = FindBestHand(aiCards, aiTotalCards);

    // 족보
    AddLog(log, LOG_SYSTEM, "Player Hand : %s", GetHandRankName(playerBest.rank));
    AddLog(log, LOG_SYSTEM, "AI Hand : %s", GetHandRankName(aiBest.rank));

    // 승패
    int whoseWinner = CompareHands(playerBest, aiBest);

    AddLog(log, LOG_SYSTEM, "=== RESULT ===");

    // 플레이어 승리
    if (whoseWinner > 0)
    {
        AddLog(log, LOG_SYSTEM, "PLAYER WIN!");

        player->chip += table->pot;
        if (aiPlayer->chip > 0)
        {
            aiPlayer->aiLevel++;

            if (aiPlayer->aiLevel > 3)
            {
                aiPlayer->aiLevel = 3;
            }
        }
    }
    // ai 승리
    else if (whoseWinner < 0)
    {
        AddLog(log, LOG_SYSTEM, "AI WIN!");
        aiPlayer->chip += table->pot;
    }
    else
    {
        AddLog(log, LOG_SYSTEM, "DRAW!");
        int split = table->pot / 2;

        player->chip += split;
        aiPlayer->chip += split;
    }

    table->pot = 0;

    AddLog(log, LOG_SYSTEM, "Player Chip : %d", player->chip);
    AddLog(log, LOG_SYSTEM, "AI Chip : %d", aiPlayer->chip);
}

static void ResetBettingRound(Player* player, Player* aiPlayer, Table* table)
{
    table->currentBet = 0;
    player->totalBet = 0;
    aiPlayer->totalBet = 0;
}

static bool IsBettingFinished(Player* player, Player* aiPlayer, Table* table, bool bothActed)
{
    if (player->isFold || aiPlayer->isFold) return true;
    if (player->chip == 0 || aiPlayer->chip == 0) return true;

    if (!bothActed) return false;

    if ((player->totalBet == table->currentBet) && (aiPlayer->totalBet == table->currentBet))
        return true;

    return false;
}

static void ApplyAIAction(AIContext *ai, Player *aiPlayer, Table* table, GameLog *log) 
{
    switch (ai->selectedAction)
    {
    case ACTION_FOLD:
        Fold(aiPlayer, log);
        break;

    case ACTION_CHECK:
        Check(aiPlayer, table, log);
        break;

    case ACTION_CALL:
        Call(aiPlayer, table, log);
        break;

    case ACTION_RAISE:
        Bet(aiPlayer, table, ai->raiseAmount, log);
        break;

    case ACTION_ALL_IN:
        AllIn(aiPlayer, table, log);
        break;
    }
}

static bool BettingPhase(Player* player, Player* aiPlayer, AIContext* ai, Table* table, Deck* deck, GameLog *log)
{
    if (player->isFold)
    {
        aiPlayer->chip += table->pot;
        table -> pot = 0;
        return false;
    }

    if (aiPlayer->isFold)
    {
        player->chip += table->pot;
        table->pot = 0;
        return false;
    }

    if (player->chip == 0 || aiPlayer->chip == 0) return true;
    bool bothActed = false;

    while (1)
    {
        ActionType playerAction = PlayerTurn(player, aiPlayer, table, deck, log);

        if (playerAction == ACTION_FOLD)
        {
            aiPlayer->chip += table->pot;
            table->pot = 0;
            return false;
        }

        UpdateAI(ai, aiPlayer, table, deck, playerAction, log);
        ApplyAIAction(ai, aiPlayer, table, log);

        if (aiPlayer->isFold)
        {
            player->chip += table->pot;
            table->pot = 0;
            return false;
        }

        bothActed = true;

        /* 종료 판정 */
        if (IsBettingFinished(player, aiPlayer, table, bothActed)) break;
    }

    return true;
}

void PlayRound(Player *player, Player *aiplayer, AIContext *ai, GameLog *log)
{
    Deck deck;
    Table table;

    InitializeDeck(&deck);
    ShuffleDeck(&deck);
    InitializeTable(&table);
    
    // 라운드 초기화
    player->isFold = false;
    aiplayer->isFold = false;
    player->totalBet = 0;
    aiplayer->totalBet = 0;
    player->hasUsedCardSwap = false;
    
    //카드 배분
    DealCards(&deck, player);
    DealCards(&deck, aiplayer);
    SetCommunityCards(&deck, &table);

    SetBlind(player, aiplayer, &table, 10);
    AddLog(log, LOG_ACTION, "<======= PLAYER TURN =======>");
    AddLog(log, LOG_ACTION, "[내 칩: %d]  [내 누적 베팅: %d]  [콜 기준 : %d]  [팟: %d]",
        player->chip, player->totalBet, table.currentBet, table.pot);

    AddLog(log, LOG_ACTION, "1. Fold");
    AddLog(log, LOG_ACTION, "2. Call");
    AddLog(log, LOG_ACTION, "3. Raise (현재 기준 %d 초과로 입력)", table.currentBet);
    AddLog(log, LOG_ACTION, "4. Check");
    AddLog(log, LOG_ACTION, "5. All In");
    if (gInventory.isMoreCard)
    {
        AddLog(log, LOG_ACTION, "6. Change Card");
    }

    ProcessVIPTable(&table);

    RenderGame(player, aiplayer, &table, log);
    PrintTableState(player, aiplayer, &table, log);    
    AddLog(log, LOG_SYSTEM, "<======= PRE FLOP =======>");
    if (!BettingPhase(player, aiplayer, ai, &table, &deck, log)) return;
    

    /*
        FLOP
    */
    ResetBettingRound(player, aiplayer, &table);
    if(!gInventory.isVIPTable) RevealFlop(&table);
    RenderGame(player, aiplayer, &table, log);
    AddLog(log, LOG_SYSTEM, "<======= FLOP =======>");
    /*PrintTableState(player, aiplayer, &table, log);*/
    if (!BettingPhase(player, aiplayer, ai, &table, &deck, log)) return;

    /*
        TURN
    */
    ResetBettingRound(player, aiplayer, &table);
    if (!gInventory.isVIPTable)RevealTurn(&table);
    RenderGame(player, aiplayer, &table, log);
    AddLog(log, LOG_SYSTEM, "<======= TURN =======>");
    /*PrintTableState(player, aiplayer, &table, log);*/
    if (!BettingPhase(player, aiplayer, ai, &table, &deck, log)) return;

    /*
        RIVER
    */
    ResetBettingRound(player, aiplayer, &table);
    if (!gInventory.isVIPTable)RevealRiver(&table);
    RenderGame(player, aiplayer, &table, log);
    AddLog(log, LOG_SYSTEM, "<======= RIVER =======>");
    /*PrintTableState(player, aiplayer, &table, log);*/
    if (!BettingPhase(player, aiplayer, ai, &table, &deck, log)) return;

    /*
        쇼다운
    */
    RenderGame(player, aiplayer, &table, log);
    AddLog(log, LOG_SYSTEM, "<======= SHOWDOWN =======>");

    ShowAICards(aiplayer, log);
    Showdown(player, aiplayer, &table, log);
    RenderGameReveal(player, aiplayer, &table, log);
    printf("아무 키나 눌러 넘기기.\n");
    WaitAnyKey();
}

static ActionType PlayerTurn(Player* player, Player *aiPlayer, Table* table, Deck* deck, GameLog *log)
{
    int choice;

    while (1)
    {
        AddLog(log, LOG_ACTION, "<======= PLAYER TURN =======>");
        AddLog(log, LOG_ACTION, "[내 칩: %d]  [내 누적 베팅: %d]  [콜 기준 : %d]  [팟: %d]",
            player->chip, player->totalBet, table->currentBet, table->pot);

        AddLog(log, LOG_ACTION, "1. Fold");
        AddLog(log, LOG_ACTION, "2. Call");
        AddLog(log, LOG_ACTION, "3. Raise (현재 기준 %d 초과로 입력)", table->currentBet);
        AddLog(log, LOG_ACTION, "4. Check");
        AddLog(log, LOG_ACTION, "5. All In");
        if (gInventory.isMoreCard)
        {
            AddLog(log, LOG_ACTION, "6. Change Card");
        }

        int needChip = table->currentBet - player->totalBet; // 0 - 40

        if (scanf_s("%d", &choice) != 1)
        {
            AddLog(log, LOG_SYSTEM, "잘못된 입력입니다.");

            while (getchar() != '\n');

            continue;
        }

        switch (choice)
        {
        case 1: // Fold

            Fold(player, log);
            return ACTION_FOLD;

        case 2: // Call
        {
            if (needChip <= 0)
            {
                AddLog(log, LOG_SYSTEM, "Already Call State (Pls Action:Check)");

                continue;
            }

            Call(player, table, log);

            AddLog(log, LOG_SYSTEM, "PLAYER CALL : %d", needChip);
            return ACTION_CALL;
        }

        case 3: // raise
        {
            int target;
            AddLog(log, LOG_SYSTEM, "레이즈 목표 총액(절대값, 현재 기준 %d 초과): ", table->currentBet);

            if (scanf_s("%d", &target) != 1)
            {
                AddLog(log, LOG_ACTION, "잘못된 입력입니다.");

                while (getchar() != '\n');
                continue;
            }

            if (target <= table->currentBet)
            {
                AddLog(log, LOG_SYSTEM, "현재 베팅(%d)보다 큰 값을 입력해야 합니다.", table->currentBet);
                continue;
            }

            Bet(player, table, target, log);
            return ACTION_RAISE;
        }

        case 4: // check 
            // 초기 상태  // currnetBet  = 10 //totalbet = 0 // <- 체크 오류

            if (table -> currentBet < player -> totalBet)
            {
                AddLog(log, LOG_ACTION, "체크할 수 없습니다.");
                continue;
            }

            if (Check(player, table, log))
            {
                AddLog(log, LOG_SYSTEM, "[system] 플레이어가 체크했습니다. PLAYER CHECK");
                return ACTION_CHECK;
            }
            else continue;

        case 5:
            AllIn(player, table, log);
            return ACTION_ALL_IN;
        case 6:

            if (!gInventory.isMoreCard)
            {
                AddLog(log, LOG_SYSTEM, "아이템이 없습니다.");
                continue;
            }
            if (player->hasUsedCardSwap)
            {
                AddLog(log, LOG_SYSTEM, "이번 라운드에 이미 카드 교체를 사용했습니다.");
                continue;
            }
            OpenCardSwapMenu(player, table, deck);
            player->hasUsedCardSwap = true;
            RenderGame(player, aiPlayer, table, log);
            AddLog(log, LOG_SYSTEM, "카드 변경 완료.");
            continue;

        }
    }
}

// 함수의 스택이 경고 수준에 이를 정도로 가득 찼는데, 이건 어떻게 해결하면...
// 기능을 분리?
// 일단은 터지지 않을 정도로만 만들어두자.
void RunPokerGame()
{

    Player player;
    Player aiPlayer;
    AIContext ai;
    GameLog log;

    player.aiLevel = 0;
    player.LIFE = 5;
    InitializeCurrency();
    InitializeAudio(true);
    
    while (1) 
    {
        InitializePlayer(&player);
        InitializePlayer(&aiPlayer);
        InitializeAI(&ai);
        InitializeLog(&log);
        
        aiPlayer.aiLevel = 1;

        while (player.chip > 0 && aiPlayer.chip > 0)
        {
            PlayRound(&player, &aiPlayer, &ai, &log);
        }

        AddLog(&log, LOG_SYSTEM, "<======= GAME END =======>");

        if (player.chip <= 0)
        {
            AddLog(&log, LOG_SYSTEM, "AI WIN FINAL");

            if(player.LIFE <= 0)
            {
                AddLog(&log, LOG_SYSTEM, "GAME OVER");
            
                exit(1);
			}
            else 
            {
                player.LIFE--;
            }
            
            PlaySFX(SOUND_LOSE);
        }
        else
        {
            AddLog(&log, LOG_SYSTEM, "PLAYER WIN FINAL");

            AddCoin(100);

            AddLog(&log, LOG_SYSTEM, "Earn Coin : %d", player.chip);
            AddLog(&log, LOG_SYSTEM, "Current Coin : %d", gCoin.coin);

            OpenShop();
        }
    }
}

