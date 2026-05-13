#include <stdlib.h>

#include "deck.h"

void InitializeDeck(Deck* deck)
{
    int index = 0;

    for (int suit = 0; suit < 4; suit++)
    {
        for (int number = 1; number <= 13; number++)
        {
            deck->cards[index].suit = suit;
            deck->cards[index].number = number;

            index++;
        }
    }

    deck->topIndex = 0;
}

void ShuffleDeck(Deck* deck) // bubble
{
    // TODO : 혹시 모르니 나중에 다른 곳으로 옮겨야 할 수도 있음.
    // 아직까지는 정상 작동

    for (int i = 0; i < DECK_SIZE; i++)
    {
        int randomIndex = rand() % DECK_SIZE;

        Card temp = deck->cards[i];

        deck->cards[i] = deck->cards[randomIndex];
        deck->cards[randomIndex] = temp;
    }
}

Card DrawCard(Deck* deck)
{
    return deck->cards[deck->topIndex++];
}