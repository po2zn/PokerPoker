#pragma once
#ifndef LOG_RENDERER_H
#define LOG_RENDERER_H

#include "screen_buffer.h"
#include "../system/game_log.h"

void DrawSystemLogs(ScreenBuffer* buffer, GameLog* log);
void DrawActionLogs(ScreenBuffer* buffer, GameLog* log);

#endif