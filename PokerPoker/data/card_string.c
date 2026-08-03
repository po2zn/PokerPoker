#include "card_string.h"
#include "stdio.h"

const char* GetCardNumberString(int number)
{
    static const char* Cardnumbers[] = {"?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    if (number < 1 || number > 13)
    {
        return "?";
    }

    const char* result = Cardnumbers[number];
    return result != NULL ? result : "?";
}

const char* GetCardSuitString(int suit)
{
    static const char* suits[] = { "♠", "♥", "◆", "♣" };
    if (suit < 0 || suit > 3) return "?";
    return suits[suit];
}