#include "table.h"

void InitializeTable(Table* table)
{
    table->pot = 0;
    table->currentBet = 0;
    table->revealedCardCount = 0;
    table->sidePot = 0;
}
