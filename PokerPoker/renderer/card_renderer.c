#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "card_renderer.h"
#include "screen_buffer.h"
#include "../data/card.h"
#include "../data/card_string.h"

void DrawTextCard(ScreenBuffer* buffer, int x, int y, Card* card)
{
    if (buffer == NULL) return;
    if (card == NULL) return;

    if (card->number < 1 || card->number > 13) return;
    if ((int)card->suit < 0 || (int)card->suit > 3) return;

    if (y < 0 || y + 4 >= SCREEN_HEIGHT) return;
    if (x < 0 || x + 11 >= SCREEN_WIDTH) return;

    const char* numStr = GetCardNumberString(card->number);
    const char* suitStr = GetCardSuitString(card->suit);

    if (numStr == NULL || suitStr == NULL) return;

    char line[32];

    DrawText(buffer, x, y + 0, "+---------+");

    snprintf(line, sizeof(line), "|%-2s       |", numStr);
    DrawText(buffer, x, y + 1, line);

    snprintf(line, sizeof(line), "|    %s    |", suitStr);
    DrawText(buffer, x, y + 2, line);

    snprintf(line, sizeof(line), "|       %-2s|", numStr);
    DrawText(buffer, x, y + 3, line);

    DrawText(buffer, x, y + 4, "+---------+");
}