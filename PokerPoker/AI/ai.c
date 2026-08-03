#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ai.h"

void InitializeAI(AIContext* ai)
{
    ai->state = AI_IDLE;
    ai->winRate = 0.0f;
    ai->selectedAction = ACTION_CHECK;
    ai->raiseAmount = 0;
    ai->isBluffing = false;
}

ActionType DecideAIAction(AIContext* ai, Player* aiPlayer, Table* table, Deck* deck, int playerAction)
{
    // 수많은 검토의 검토의 검토의 검토의 오류의 검토의 검토의 오류의 예외의 예외의 예외의
    // 예외!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (ai == NULL)
    {
        return ACTION_FOLD;
    }

    if (aiPlayer == NULL)
    {
        return ACTION_FOLD;
    }

    if (table == NULL)
    {
        return ACTION_FOLD;
    }

    if (aiPlayer->isFold)
    {
        ai->selectedAction = ACTION_FOLD;
        return ACTION_FOLD;
    }

    if (ai->winRate > 100.0f)
    {
        ai->winRate = 100.0f;
    }

    if (ai->winRate < 0.0f)
    {
        ai->winRate = 0.0f;
    }

    ai->raiseAmount = 0;

    float P = ai->winRate;

    int randomValue = rand() % 100;

    if (ai->isBluffing)
    {
        if (P < 40.0f)
        {
            ai->raiseAmount = table->currentBet * 2;

            if (ai->raiseAmount <= table->currentBet) ai->raiseAmount = table->currentBet + 10;

            ai->selectedAction = ACTION_RAISE;
            return ACTION_RAISE;
        }
    }

    if (playerAction == ACTION_ALL_IN) 
    { 
        if (aiPlayer->chip <= 0)
        {
            ai->selectedAction = ACTION_FOLD;
            return ACTION_FOLD;
        }

        ai->selectedAction = ACTION_ALL_IN;
        return ACTION_ALL_IN; 
    }

    if (playerAction == ACTION_CHECK)
    {
        if (P < 30.0f)
        {
            ai->selectedAction = ACTION_CHECK;

            return ACTION_CHECK;
        }

        if (randomValue < 40)
        {
            ai->selectedAction = ACTION_CHECK;

            return ACTION_CHECK;
        }
    }

    if (playerAction == ACTION_RAISE)
    {
        if (P < 45.0f)
        {
            ai->selectedAction = ACTION_CALL;

            return ACTION_CALL;
        }

        if (randomValue < 40)
        {
            ai->selectedAction = ACTION_FOLD;

            return ACTION_FOLD;
        }

        if (randomValue < 15)
        {
            ai->selectedAction = ACTION_ALL_IN;

            return ACTION_ALL_IN;
        }
    }

    if (P >= 60.0f)
    {
        if (aiPlayer->aiLevel >= 3)
        {
            if (randomValue < 20)
            {
                ai->selectedAction = ACTION_ALL_IN;

                return ACTION_ALL_IN;
            }
        }

        ai->raiseAmount = table->currentBet + 10;

        ai->selectedAction = ACTION_RAISE;

        return ACTION_RAISE;
    }

    if (P >= 50.0f)
    {
        if (randomValue < 70)
        {
            ai->selectedAction = ACTION_CALL;
        }

        else
        {
            ai->raiseAmount = table->currentBet + 5;

            ai->selectedAction = ACTION_RAISE;
        }

        return ai->selectedAction;
    }

    if (P >= 30.0f)
    {
        if (randomValue < 60)
        {
            ai->selectedAction = ACTION_CALL;
        }

        else if (randomValue < 85)
        {
            ai->selectedAction = ACTION_CHECK;
        }

        else
        {
            ai->raiseAmount = table->currentBet + 5;

            ai->selectedAction = ACTION_RAISE;
        }

        return ai->selectedAction;
    }

    if (P >= 10.0f)
    {
        if (randomValue < 50)
        {
            ai->selectedAction = ACTION_FOLD;
        }

        else if (randomValue < 80)
        {
            ai->selectedAction = ACTION_CALL;
        }

        else
        {
            ai->selectedAction = ACTION_CHECK;
        }

        return ai->selectedAction;
    }

    if (aiPlayer->aiLevel >= 3)
    {
        if (randomValue < 5)
        {
            ai->selectedAction = ACTION_ALL_IN;

            return ACTION_ALL_IN;
        }
    }

    ai->selectedAction = ACTION_FOLD;

    return ACTION_FOLD;
}

void ExecuteAIAction(AIContext* ai, Player* aiPlayer, Table* table, GameLog* log) {
    if (ai == NULL)      return;
    if (aiPlayer == NULL) return;
    if (table == NULL)   return;

    ai->state = AI_ACT;

    switch (ai->selectedAction) {
    case ACTION_FOLD:
        AddLog(log, LOG_SYSTEM, "[AI] Fold");
        break;
    case ACTION_CALL:
        AddLog(log, LOG_SYSTEM, "[AI] Call");
        break;
    case ACTION_CHECK:
        AddLog(log, LOG_SYSTEM, "[AI] Check");
        break;
    case ACTION_RAISE:
        AddLog(log, LOG_SYSTEM, "[AI] Raise");
        break;
    case ACTION_ALL_IN:
        AddLog(log, LOG_SYSTEM, "[AI] ALL IN !!!");
        break;
    }

    ai->state = AI_IDLE;
}