#include <stdbool.h>
#include <string.h>

#include "hand_evaluator.h"

static int GetCardValue(int number)
{
    if (number == 1)
    {
        return 14;
    }

    return number;
}

static void CountNumbers(Card cards[], int cardCount, int numberCount[])
{
    memset(numberCount, 0, sizeof(int) * 15);

    for (int i = 0; i < cardCount; i++)
    {
        int value = GetCardValue(cards[i].number);

        numberCount[value]++;
    }
}

static bool IsFlush(Card cards[], int cardCount)
{
    int suitCount[4] = { 0 };

    for (int i = 0; i < cardCount; i++)
    {
        suitCount[cards[i].suit]++;
    }

    for (int i = 0; i < 4; i++)
    {
        if (suitCount[i] >= 5)
        {
            return true;
        }
    }

    return false;
}

static bool IsStraight(int numberCount[])
{
    int consecutive = 0;

    for (int i = 2; i <= 14; i++)
    {
        if (numberCount[i] > 0)
        {
            consecutive++;

            if (consecutive >= 5)
            {
                return true;
            }
        }
        else
        {
            consecutive = 0;
        }
    }

    return false;
}

static bool IsRoyalFlush(Card cards[], int cardCount)
{
    int suitCount[4][15] = { 0 };

    for (int i = 0; i < cardCount; i++)
    {
        int suit = cards[i].suit;

        int value = GetCardValue(cards[i].number);

        suitCount[suit][value] = 1;
    }

    for (int suit = 0; suit < 4; suit++)
    {
        if
            (
                suitCount[suit][10] &&
                suitCount[suit][11] &&
                suitCount[suit][12] &&
                suitCount[suit][13] &&
                suitCount[suit][14]
                )
        {
            return true;
        }
    }

    return false;
}

static void FillKicker(HandResult* result, int numberCount[])
{
    int index = 0;

    for (int i = 14; i >= 2; i--) // 강함 역순 
    {
        if (numberCount[i] > 0)
        {
            for (int j = 0; j < numberCount[i]; j++)
            {

                if (i == result->primaryValue)
                {
                    continue;
                }

                if (i == result->secondaryValue)
                {
                    continue;
                }

                result->kicker[index++] = i;

                if (index >= 5)
                {
                    return;
                }
            }
        }
    }
}

HandResult EvaluateHand(Card cards[], int cardCount)
{
    HandResult result;
    bool firstPairFound = false;

    result.rank = HIGH_CARD;
    result.primaryValue = 0;
    result.secondaryValue = 0;

    for (int i = 0; i < 5; i++)
    {
        result.kicker[i] = 0;
    }

    int numberCount[15];

    CountNumbers(cards, cardCount, numberCount);

    bool flush = IsFlush(cards, cardCount);
    bool straight = IsStraight(numberCount);
    bool three = false;
    bool four = false;

    int pairCount = 0;

    // 족보 탐색
    for (int i = 14; i >= 2; i--)
    {
        if (numberCount[i] == 4)
        {
            four = true;
            result.primaryValue = i;
        }

        else if (numberCount[i] == 3)
        {
            three = true;
            result.primaryValue = i;
        }

        else if (numberCount[i] == 2)
        {
            pairCount++;

            if (result.primaryValue == 0)
            {
                result.primaryValue = i;
                firstPairFound = true;
            }

            else
            {
                result.secondaryValue = i;
            }
        }
    }

    if (straight && flush)
    {
        if (IsRoyalFlush(cards, cardCount))
        {
            result.rank = ROYAL_FLUSH;
        }
        else
        {
            result.rank = STRAIGHT_FLUSH;
        }
    }

    else if (four)
    {
        result.rank = FOUR_OF_A_KIND;
    }

    else if (three && pairCount >= 1)
    {
        result.rank = FULL_HOUSE;
    }

    else if (flush)
    {
        result.rank = FLUSH;
    }

    else if (straight)
    {
        result.rank = STRAIGHT;
    }

    else if (three)
    {
        result.rank = THREE_OF_A_KIND;
    }

    else if (pairCount >= 2)
    {
        result.rank = TWO_PAIR;
    }

    else if (pairCount == 1)
    {
        result.rank = ONE_PAIR;
    }

    else
    {
        result.rank = HIGH_CARD;

        for (int i = 14; i >= 2; i--)
        {
            if (numberCount[i] > 0)
            {
                result.primaryValue = i;
                break;
            }
        }
    }

    FillKicker(&result, numberCount);

    return result;
}

const char* GetHandRankName(HandRank rank)
{
    switch (rank)
    {
    case HIGH_CARD:
        return "High Card";

    case ONE_PAIR:
        return "One Pair";

    case TWO_PAIR:
        return "Two Pair";

    case THREE_OF_A_KIND:
        return "Three of a Kind";

    case STRAIGHT:
        return "Straight";

    case FLUSH:
        return "Flush";

    case FULL_HOUSE:
        return "Full House";

    case FOUR_OF_A_KIND:
        return "Four of a Kind";

    case STRAIGHT_FLUSH:
        return "Straight Flush";

    case ROYAL_FLUSH:
        return "Royal Flush";
    }

    return "Unknown";
}

int CompareHands(HandResult player, HandResult ai)
{
    // 족보
    if (player.rank > ai.rank)
    {
        return 1;
    }

    if (player.rank < ai.rank)
    {
        return -1;
    }

    // 1순위
    if (player.primaryValue > ai.primaryValue)
    {
        return 1;
    }

    if (player.primaryValue < ai.primaryValue)
    {
        return -1;
    }


    // 2순위 
    if (player.secondaryValue > ai.secondaryValue)
    {
        return 1;
    }

    if (player.secondaryValue < ai.secondaryValue)
    {
        return -1;
    }

    
    // 키커
    for (int i = 0; i < 5; i++)
    {
        if (player.kicker[i] > ai.kicker[i])
        {
            return 1;
        }

        if (player.kicker[i] < ai.kicker[i])
        {
            return -1;
        }
    }

    return 0;
}

HandResult FindBestHand(Card cards[], int cardCount)
{
    HandResult bestResult;

    // 최악 상태
    bestResult.rank = HIGH_CARD;
    bestResult.primaryValue = 0;
    bestResult.secondaryValue = 0;

    for (int i = 0; i < 5; i++)
    {
        bestResult.kicker[i] = 0;
    }

    for (int a = 0; a < cardCount - 4; a++)
    {
        for (int b = a + 1; b < cardCount - 3; b++)
        {
            for (int c = b + 1; c < cardCount - 2; c++)
            {
                for (int d = c + 1; d < cardCount - 1; d++)
                {
                    for (int e = d + 1; e < cardCount; e++)
                    {
                        Card selected[5];

                        selected[0] = cards[a];
                        selected[1] = cards[b];
                        selected[2] = cards[c];
                        selected[3] = cards[d];
                        selected[4] = cards[e];

                        HandResult current = EvaluateHand(selected, 5);

                        // 강한 패 검증
                        if (CompareHands(current, bestResult) > 0)
                        {
                            bestResult = current;
                        }
                    }
                }
            }
        }
    }

    return bestResult;
}