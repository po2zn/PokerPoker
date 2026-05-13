#include <stdbool.h>
#include <stdlib.h>

#include "win_rate_calculator.h"

#include "../poker/hand_evaluator.h"

bool IsSameCard(Card A, Card B) 
{
	return A.number == B.number && A.suit == B.suit;
}

int CompareHandResult(HandResult a, HandResult b) 
{
    // 족보
    if (a.rank > b.rank)
        return 1;

    if (a.rank < b.rank)
        return -1;

    // 1 우선순위
    if (a.primaryValue > b.primaryValue)
        return 1;

    if (a.primaryValue < b.primaryValue)
        return -1;

    // 2 우선순위
    if (a.secondaryValue > b.secondaryValue)
        return 1;

    if (a.secondaryValue < b.secondaryValue)
        return -1;

    // 키커
    for (int i = 0; i < 5; i++)
    {
        if (a.kicker[i] > b.kicker[i])
            return 1;

        if (a.kicker[i] < b.kicker[i])
            return -1;
    }

    return 0;
}

float CalculateWinRate(Player* aiPlayer, Table* table, Deck* deck)
{
    int simulationCount = 1000;

    int win = 0;
    int draw = 0;

    for (int sim = 0; sim < simulationCount; sim++)
    {
        bool used[52] = { false };

        Card aiCards[7];
        Card enemyCards[7];

        aiCards[0] = aiPlayer->hand[0];
        aiCards[1] = aiPlayer->hand[1];

        for (int i = 0; i < 52; i++)
        {
            if (IsSameCard(deck->cards[i], aiPlayer->hand[0]) || IsSameCard(deck->cards[i], aiPlayer->hand[1]))
            {
                used[i] = true;
            }
        }

        int enemyCount = 0;

        while (enemyCount < 2)
        {
            int r = rand() % 52;

            if (used[r]) continue;

            used[r] = true;
            enemyCards[enemyCount++] = deck->cards[r];
        }

        int revealed = table->revealedCardCount;

        for (int i = 0; i < revealed; i++)
        {
            aiCards[i + 2] = table->communityCards[i];
            enemyCards[i + 2] = table->communityCards[i];

            for (int j = 0; j < 52; j++)
            {
                if (IsSameCard(
                    deck->cards[j],
                    table->communityCards[i]))
                {
                    used[j] = true;
                }
            }
        }

        int current = revealed + 2;

        while (current < 7)
        {
            int r = rand() % 52;

            if (used[r]) continue;

            used[r] = true;

            aiCards[current] = deck->cards[r];
            enemyCards[current] = deck->cards[r];

            current++;
        }


        HandResult aiResult = EvaluateHand(aiCards, 7);
        HandResult enemyResult = EvaluateHand(enemyCards, 7);

        int compare = CompareHandResult(aiResult, enemyResult);

        if (compare > 0)
        {
            win++;
        }
        else if (compare == 0)
        {
            draw++;
        }
    }

    return ((win + draw * 0.5f) / simulationCount) * 100.0f;
}