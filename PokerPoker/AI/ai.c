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

    if (P >= 70.0f)
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

void ExecuteAIAction(AIContext* ai, Player* aiPlayer, Table* table)
{
    if (ai == NULL) return;
    if (aiPlayer == NULL) return;
    if (table == NULL) return;

    ai->state = AI_ACT;

    switch (ai->selectedAction)
    {
    case ACTION_FOLD:
        printf("\n[AI] Fold\n");
        break;

    case ACTION_CALL:
        printf("\n[AI] Call\n");
        break;

    case ACTION_CHECK:
        printf("\n[AI] Check\n");
        break;

    case ACTION_RAISE:
        printf("\n[AI] Raise\n");
        break;

    case ACTION_ALL_IN:
        printf("\n[AI] ALL IN !!!\n");
        break;
    }

    ai->state = AI_IDLE;
}