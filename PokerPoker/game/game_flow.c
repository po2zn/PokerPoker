#include <stdio.h>

#include "../ai/ai.h"
#include "../ai/ai_update.h"

#include "../data/card_printer.h"
#include "../data/deck.h"
#include "../data/player.h"
#include "../data/table.h"

#include "../dealer/dealer.h"

#include "../poker/hand_evaluator.h"

#include "../enum/action_type.h"

static ActionType PlayerTurn(Player* player, Table* table);
static void ApplyAIAction(AIContext *ai, Player *aiPlayer, Table *table);

void ShowCommunityCards(Table* table)
{
    printf("\n=== Community Cards ===\n");

    for (int i = 0; i < table->revealedCardCount; i++)
    {
        PrintCard(table->communityCards[i]);
        printf(" ");
    }

    printf("\n");
}

void ShowPlayerCards(Player* player)
{
    printf("\n=== PLAYER CARD ===\n");

    for (int i = 0; i < 2; i++)
    {
        PrintCard(player->hand[i]);

        printf(" ");
    }

    printf("\n");
}

// 임시
void ShowAICards(Player* ai) 
{
    printf("\n=== AI CARD ===\n");

    for (int i = 0; i < 2; i++)
    {
        PrintCard(ai->hand[i]);

        printf(" ");
    }

    printf("\n");
}

static void PrintTableState(Player* player, Player* ai, Table* table)
{
    printf("\n[pot: %d]  [currentBet: %d]  [playerChip: %d]  [AIChip: %d]\n",
        table->pot, table->currentBet, player->chip, ai->chip);
}

static void Showdown(Player* player, Player* aiPlayer, Table* table)
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

    // player
    Card playerCards[7];

    playerCards[0] = player->hand[0];
    playerCards[1] = player->hand[1];

    Card aiCards[7];

    aiCards[0] = aiPlayer->hand[0];
    aiCards[1] = aiPlayer->hand[1];

    // 커뮤니티
    for (int i = 0; i < 5; i++)
    {
        playerCards[i + 2] = table->communityCards[i];
        aiCards[i + 2] = table->communityCards[i];
    }

    HandResult playerBest = FindBestHand(playerCards, 7);
    HandResult aiBest = FindBestHand(aiCards, 7);

    // 족보
    printf("\nPlayer Hand : %s\n", GetHandRankName(playerBest.rank));
    printf("AI Hand : %s\n", GetHandRankName(aiBest.rank));

    // 승패
    int whoseWinner = CompareHands(playerBest, aiBest);

    printf("\n=== RESULT ===\n");

    // 플레이어 승리
    if (whoseWinner > 0)
    {
        printf("PLAYER WIN!\n");

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
        printf("AI WIN!\n");
        aiPlayer->chip += table->pot;
    }
    else
    {
        printf("DRAW!\n");
        int split = table->pot / 2;

        player->chip += split;
        aiPlayer->chip += split;
    }

    table->pot = 0;

    printf("\nPlayer Chip : %d\n", player->chip);
    printf("AI Chip : %d\n", aiPlayer->chip);
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

static void ApplyAIAction(AIContext *ai, Player *aiPlayer, Table* table) 
{
    switch (ai->selectedAction)
    {
    case ACTION_FOLD:
        Fold(aiPlayer);
        break;

    case ACTION_CHECK:
        Check(aiPlayer, table);
        break;

    case ACTION_CALL:
        Call(aiPlayer, table);
        break;

    case ACTION_RAISE:
        Bet(aiPlayer, table, ai->raiseAmount);
        break;

    case ACTION_ALL_IN:
        AllIn(aiPlayer, table);
        break;
    }
}

static bool BettingPhase(Player* player, Player* aiPlayer, AIContext* ai, Table* table, Deck* deck)
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
        ActionType playerAction = PlayerTurn(player, table);

        if (playerAction == ACTION_FOLD)
        {
            aiPlayer->chip += table->pot;
            table->pot = 0;
            return false;
        }

        UpdateAI(ai, aiPlayer, table, deck, playerAction);
        ApplyAIAction(ai, aiPlayer, table);

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

void PlayRound(Player *player, Player *aiplayer, AIContext *ai)
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

    //카드 배분
    DealCards(&deck, player);
    DealCards(&deck, aiplayer);
    SetCommunityCards(&deck, &table);

    SetBlind(player, aiplayer, &table, 10);
    PrintTableState(player, aiplayer, &table);
    ShowPlayerCards(player);



    printf("\n=== PRE FLOP ===\n");
    if (!BettingPhase(player, aiplayer, ai, &table, &deck)) return;

    /*
        FLOP
    */
    ResetBettingRound(player, aiplayer, &table);
    printf("\n=== FLOP ===\n");
    PrintTableState(player, aiplayer, &table);

    RevealFlop(&table);
    ShowCommunityCards(&table);
    PrintTableState(player, aiplayer, &table);
    if (!BettingPhase(player, aiplayer, ai, &table, &deck)) return;

    /*
        TURN
    */
    ResetBettingRound(player, aiplayer, &table);
    printf("\n=== TURN ===\n");

    RevealTurn(&table);
    ShowCommunityCards(&table);
    PrintTableState(player, aiplayer, &table);
    if (!BettingPhase(player, aiplayer, ai, &table, &deck)) return;

    /*
        RIVER
    */
    ResetBettingRound(player, aiplayer, &table);
    printf("\n=== RIVER ===\n");

    RevealRiver(&table);
    ShowCommunityCards(&table);
    PrintTableState(player, aiplayer, &table);
    if (!BettingPhase(player, aiplayer, ai, &table, &deck)) return;
    /*
        쇼다운
    */
    printf("\n=== SHOWDOWN ===\n");

    ShowAICards(aiplayer);
    Showdown(player, aiplayer, &table);
}

static ActionType PlayerTurn(Player* player, Table* table)
{
    int choice;

    while (1)
    {
        printf("\n=== PLAYER TURN ===\n");
        printf("[내 칩: %d]  [내 누적 베팅: %d]  [콜 기준 : %d]  [팟: %d]\n",
            player->chip, player->totalBet, table->currentBet, table->pot);

        printf("1. Fold\n");
        printf("2. Call\n");
        printf("3. Raise (현재 기준 %d 초과로 입력)\n", table->currentBet);
        printf("4. Check\n");
        printf("5. All In\n");

        int needChip = table->currentBet - player->totalBet; // 0 - 40

        if (scanf_s("%d", &choice) != 1)
        {
            printf("\n잘못된 입력입니다.\n");

            while (getchar() != '\n');

            continue;
        }

        switch (choice)
        {
        case 1: // Fold

            Fold(player);
            return ACTION_FOLD;

        case 2: // Call
        {
            if (needChip <= 0)
            {
                printf("\nAlready Call State (Pls Action:Check)\n");

                continue;
            }

            Call(player, table);

            printf("\nPLAYER CALL : %d\n", needChip);
            return ACTION_CALL;
        }

        case 3: // raise
        {
            int target;
            printf("\n레이즈 목표 총액(절대값, 현재 기준 %d 초과): ", table->currentBet);

            if (scanf_s("%d", &target) != 1)
            {
                printf("\n잘못된 입력입니다.\n");

                while (getchar() != '\n');
                continue;
            }

            if (target <= table->currentBet)
            {
                printf("\n현재 베팅(%d)보다 큰 값을 입력해야 합니다.\n", table->currentBet);
                continue;
            }

            Bet(player, table, target);
            return ACTION_RAISE;
        }

        case 4: // check

            if (table -> currentBet <= player -> totalBet)
            {
                printf("\n체크할 수 없습니다.\n");
                continue;
            }

            Check(player, table);
            printf("\n[system] 플레이어가 체크했습니다. PLAYER CHECK\n");

            return ACTION_CHECK;

        case 5:
            AllIn(player, table);
            return ACTION_ALL_IN;

        default:
            printf("\n잘못된 입력입니다.\n");
            continue;

        }
    }
}

void RunPokerGame()
{
    Player player;
    Player aiPlayer;
    AIContext ai;

    InitializePlayer(&player);
    InitializePlayer(&aiPlayer);
    InitializeAI(&ai);

    player.aiLevel = 0;
    aiPlayer.aiLevel = 1;

    while(player.chip > 0 && aiPlayer.chip > 0)
    {
        PlayRound(&player, &aiPlayer, &ai);
    }

    printf("\n=== GAME END ===\n");

    if (player.chip <= 0)
    {
        printf("AI WIN FINAL\n");
    }
    else
    {
        printf("PLAYER WIN FINAL\n");

        printf("Earn Coin : %d\n", player.chip);
        //TODO : 코인 시스템 구현하기
    }
}

