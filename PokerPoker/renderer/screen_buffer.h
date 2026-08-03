#pragma once
#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H

#define SCREEN_WIDTH 170
#define SCREEN_HEIGHT 40

typedef struct ScreenBuffer
{
    char cells[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
} ScreenBuffer;

void ClearBuffer(ScreenBuffer* buffer);

void DrawText(ScreenBuffer* buffer, int x, int y, const char* text);

void RenderBuffer(ScreenBuffer* buffer);

#endif