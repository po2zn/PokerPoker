#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "ai_update.h"

#include "win_rate_calculator.h"

#include "../system/game_log.h"

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

static void ShowStateMessage(AIState state, GameLog *log)
{
    int tmp = rand() % 3;
    switch (state)
    {
    case AI_ANALYZE:
        if(tmp == 0) { AddLog(log, LOG_SYSTEM, "[AI] 패 분석 중"); }
        else if(tmp == 1) { AddLog(log, LOG_SYSTEM, "[AI] 매우 흥미로움을 느끼는 중"); }
        else if (tmp == 2) { AddLog(log, LOG_SYSTEM, "[AI] 당신의 심리 분석 중"); }
        break;

    case AI_DECIDE:
        if(tmp == 0) { AddLog(log, LOG_SYSTEM, "[AI] 네 패가 뻔히 보인다. 인간."); }
        else if(tmp == 1) { AddLog(log, LOG_SYSTEM, "[AI] 이건 인류 역사상 신의 한 수."); }
        else if(tmp == 2) { AddLog(log, LOG_SYSTEM, "[AI] 미래를 훔쳐봤다."); }
        break;

    case AI_ACT:
        if(tmp == 0 || tmp == 1) { AddLog(log, LOG_SYSTEM, "[AI] 지금은 이렇게 해야겠군 하하."); }
        else { AddLog(log, LOG_SYSTEM, "[AI] 신중한 결정입니다. 삐릭..."); }
        break;
    }
}

// 
static void ShowThinkingAnimation(int thinkTime, GameLog* log)
{
    int seconds = thinkTime / 1000;
    char dots[16] = { 0 };
    for (int i = 0; i < seconds && i < 15; i++)
    {
        dots[i] = '.';
        Sleep(400);
    }
    AddLog(log, LOG_SYSTEM, "%s", dots);  // 로그 1회만 추가
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

void UpdateAI(AIContext* ai, Player* aiPlayer, Table* table, Deck* deck, int playerAction, GameLog *log)
{
    if (ai == NULL) return;

    // 분석
    ai->state = AI_ANALYZE;

    ShowStateMessage(ai->state, log);

    ShowThinkingAnimation(GetThinkTime(aiPlayer->aiLevel), log);

    ai->winRate = CalculateWinRate(aiPlayer, table, deck);
    /*AddLog(log, LOG_SYSTEM, "WinRate : %f", ai->winRate);*/
    
    // 블러핑
    if (ShouldBluff(aiPlayer->aiLevel))
    {
        int tmp = rand() % 5;
        if(tmp == 0) { AddLog(log, LOG_SYSTEM, "[AI] 푸하하하! 이거 제가 딜러를 매수한 듯 싶습니다!"); }
        else if(tmp == 1) { AddLog(log, LOG_SYSTEM, "[AI] 아무래도 이건 도박인 것 같군요."); }
        else if(tmp == 2) { AddLog(log, LOG_SYSTEM, "[AI] 삐릭삐릭..."); }
        else if(tmp == 3) { AddLog(log, LOG_SYSTEM, "[AI] 이 순간은!"); }
        else if(tmp == 4) { AddLog(log, LOG_SYSTEM, "[AI] 카드를 가리시죠? 당신의 패가 다 보입니다."); }
        else { printf("블러핑 에러"); }
        
        ai->isBluffing = false;

        if (ShouldBluff(aiPlayer->aiLevel))
        {
            ai->isBluffing = true;

            AddLog(log, LOG_SYSTEM, "[System] 상대를 흔들려고 합니다.");
        }
    }

    // 결정
    ai->state = AI_DECIDE;

    ShowStateMessage(ai->state, log);

    ai->selectedAction = DecideAIAction(ai, aiPlayer, table, deck, playerAction);

    // 행동
    ai->state = AI_ACT;

    ShowStateMessage(ai->state, log);
    ExecuteAIAction(ai, aiPlayer, table, log);

    ai->state = AI_IDLE;
}