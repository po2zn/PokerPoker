#include <string.h>
#include <stdio.h>

#include "screen_buffer.h"
#include "../system/console.h"


void ClearBuffer(ScreenBuffer* buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            buffer->cells[y][x] = ' ';
        }

        buffer->cells[y][SCREEN_WIDTH] = '\0';
    }
}

void DrawText(ScreenBuffer* buffer, int x, int y, const char* text)
{
    if (buffer == NULL) return;
    if (text == NULL) return;

    if (y < 0 || y >= SCREEN_HEIGHT)
    {
        return;
    }

    if (x >= SCREEN_WIDTH)
    {
        return;
    }

    int len = strlen(text);

    for (int i = 0; i < len; i++)
    {
        int drawX = x + i;

        if (drawX >= 0 && drawX < SCREEN_WIDTH)
        {
            buffer->cells[y][drawX] = text[i];
        }
    }
}

void RenderBuffer(ScreenBuffer* buffer)
{
    if (buffer == NULL)
    {
        return;
    }

    ClearConsole();

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            putchar(buffer->cells[y][x]);
        }

        putchar('\n');
    }
}