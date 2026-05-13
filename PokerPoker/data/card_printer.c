#include <stdio.h>

#include "card_printer.h"
#include "card_string.h"

void PrintCard(Card card)
{
    printf("[%s %s]",
        GetCardNumberString(card.number),
        GetCardSuitString(card.suit));
}