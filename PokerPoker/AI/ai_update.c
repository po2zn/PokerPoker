#include <stdio.h>
#include <stdlib.h>

#include "ai_update.h"

#include "win_rate_calculator.h"


static int GetThinkTime(int aiLevel)
{
    switch (aiLevel)
    {
    case 1:
        return 5000;

    case 2:
        return 3000;

    case 3:
        return 1000;
    }

    return 2000;
}

static void ShowStateMessage(AIState state)
{
    int tmp = rand() % 3;
    switch (state)
    {
    case AI_ANALYZE:
        if(tmp == 0) { printf("\n[AI] 패 분석 중"); }
        else if(tmp == 1) { printf("\n[AI] 매우 흥미로움을 느끼는 중"); }
        else if (tmp == 2) { printf("\n[AI] 분석 중"); }
        break;

    case AI_DECIDE:
        if(tmp == 0) { printf("\n[AI] 네 패가 뻔히 보인다. 인간."); }
        else if(tmp == 1) { printf("\n[AI] 이건 인류 역사상 신의 한 수."); }
        else if(tmp == 2) { printf("\n[AI] 미래를 훔쳐봤다."); }
        break;

    case AI_ACT:
        if(tmp == 0 || tmp == 1) { printf("\n[AI] 지금은 이렇게 해야겠군 하하."); }
        else { printf("\n[AI] 신중한 결정입니다. 삐릭..."); }
        break;
    }
}

static void ShowThinkingAnimation(int thinkTime)
{
    int seconds = thinkTime / 1000;

    for (int i = 0; i < seconds; i++)
    {
        printf(".");
        fflush(stdout);

        Sleep(400);
    }

    printf("\n");
}

static bool ShouldBluff(int aiLevel)
{
    int chance = 0;

    switch (aiLevel)
    {
    case 1:
        chance = 15;
        break;

    case 2:
        chance = 55;
        break;

    case 3:
        chance = 100;
        break;
    }

    return (rand() % 100) < chance;
}

void UpdateAI(AIContext* ai, Player* aiPlayer, Table* table, Deck* deck, int playerAction)
{
    if (ai == NULL) return;

    // 분석
    ai->state = AI_ANALYZE;

    ShowStateMessage(ai->state);

    ShowThinkingAnimation(GetThinkTime(aiPlayer->aiLevel));

    ai->winRate = CalculateWinRate(aiPlayer, table, deck);

    
    // 블러핑
    if (ShouldBluff(aiPlayer->aiLevel))
    {
        int tmp = rand() % 5;
        if(tmp == 0) { printf("\n[AI] 푸하하하! 이거 제가 딜러를 매수한 듯 싶습니다!\n"); }
        else if(tmp == 1) { printf("\n[AI] 아무래도 이건 도박인 것 같군요.\n"); }
        else if(tmp == 2) { printf("\n[AI] 삐릭삐릭...\n"); }
        else if(tmp == 3) { printf("\n[AI] 이 순간은!\n"); }
        else if(tmp == 4) { printf("\n[AI] 카드를 가리시죠? 당신의 패가 다 보입니다.\n"); }
        else { printf("\n블러핑 에러\n"); }
        
        ai->isBluffing = false;

        if (ShouldBluff(aiPlayer->aiLevel))
        {
            ai->isBluffing = true;

            printf("\n[system] 상대를 흔들려고 합니다.\n");
        }
    }

    // 결정
    ai->state = AI_DECIDE;

    ShowStateMessage(ai->state);

    ai->selectedAction = DecideAIAction(ai, aiPlayer, table, deck, playerAction);

    // 행동
    ai->state = AI_ACT;

    ShowStateMessage(ai->state);
    ExecuteAIAction(ai, aiPlayer, table);

    ai->state = AI_IDLE;
}