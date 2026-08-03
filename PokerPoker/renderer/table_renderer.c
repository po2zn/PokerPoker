#include "table_renderer.h"
#include "card_renderer.h"

#include "../data/player.h"

void DrawHiddenCard(ScreenBuffer* buffer, int x, int y)
{
    DrawText(buffer, x, y + 0, "+-------+");
    DrawText(buffer, x, y + 1, "ㅣ  ?  ㅣ");
    DrawText(buffer, x, y + 2, "ㅣ  ?  ㅣ");
    DrawText(buffer, x, y + 3, "ㅣ  ?  ㅣ");
    DrawText(buffer, x, y + 4, "+-------+");
}

void DrawPokerTableReveal(ScreenBuffer* buffer, Player* player, Player* aiPlayer, Table* table)
{
    DrawText(buffer, 5, 1, "================= TEXAS HOLD'EM =================");
    
    DrawText(buffer, 15, 5, "AI");

    DrawTextCard(buffer, 15, 7, &aiPlayer->hand[0]);
    DrawTextCard(buffer, 25, 7, &aiPlayer->hand[1]);

    DrawText(buffer, 15, 15, "COMMUNITY");

    for (int i = 0; i < table->revealedCardCount; i++)
    {
        DrawTextCard(buffer, 15 + (i * 10), 18, &table->communityCards[i]);
    }

    DrawText(buffer, 15, 28, "PLAYER");

    for (int i = 0; i < player->handCardCount; i++)
    {
        DrawTextCard(buffer, 15 + (i * 10), 30, &player->hand[i]);
    }
}

void DrawPokerTable(ScreenBuffer* buffer, Player *player, Player *aiPlayer, Table* table)
{
    DrawText(buffer, 5, 1, "================= TEXAS HOLD'EM =================");

    DrawText(buffer, 15, 5, "AI");

    DrawHiddenCard(buffer, 15, 7);
    DrawHiddenCard(buffer, 25, 7);

    DrawText(buffer, 15, 15, "COMMUNITY");
    for (int i = 0; i < table->revealedCardCount; i++)
    {
        DrawTextCard(buffer, 15 + (i * 10), 18, &table->communityCards[i]);
    }
    
    DrawText(buffer, 15, 28, "PLAYER");

    for (int i = 0; i < player->handCardCount; i++) 
    {
        DrawTextCard(buffer, 15 + (i * 10), 30, &player->hand[i]);
    }
}