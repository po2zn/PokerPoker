#include "card_string.h"

const char* GetCardNumberString(int number)
{
    switch (number)
    {
        case 1:
            return "A";

        case 11:
            return "J";

        case 12:
            return "Q";

        case 13:
            return "K";

        default:
        {
            static char buffer[3];

            sprintf_s(buffer, sizeof(buffer), "%d", number);

            return buffer;
        }
    }
}

const char* GetCardSuitString(int suit)
{
    switch (suit)
    {
    case 0:
        return "♠";

    case 1:
        return "♥";

    case 2:
        return "◆";

    case 3:
        return "♣";
    }

    return "?";
}