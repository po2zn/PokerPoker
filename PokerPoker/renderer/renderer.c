#include <stdio.h>

#include "renderer.h"

#include "screen_buffer.h"
#include "table_renderer.h"
#include "log_renderer.h"

static void DrawLife(ScreenBuffer* buffer, int life)
{
    // 화면 우측 상단 고정 위치 (x=0, y=0 은 테이블과 겹치므로 여백 확보)
    DrawText(buffer, 2, 0, "LIFE: ");

    char hearts[32] = { 0 };
    for (int i = 0; i < life && i < 10; i++)
    {
        DrawText(buffer, 8 + (i * 2), 0, "♥");  //
    }
}

void DrawGameLOGO(const char *gmaelog) 
{
	printf("%s", gmaelog);
}

void RenderGame(Player *player, Player *aiPlayer, Table* table, GameLog *log)
{
    ScreenBuffer buffer;

    ClearBuffer(&buffer);
    DrawLife(&buffer, player->LIFE);

    DrawPokerTable(&buffer, player, aiPlayer, table);
    DrawSystemLogs(&buffer, log);
    DrawActionLogs(&buffer, log);

    RenderBuffer(&buffer);
}


void RenderGameReveal(Player* player, Player* aiPlayer, Table* table, GameLog *log)
{
    ScreenBuffer buffer;

    ClearBuffer(&buffer);

    DrawPokerTableReveal(&buffer, player, aiPlayer, table);
    DrawSystemLogs(&buffer, log);
    DrawActionLogs(&buffer, log);
    RenderBuffer(&buffer);
}