#pragma once
#pragma once
#ifndef CARD_RENDERER_H
#define CARD_RENDERER_H

#include "../data/card.h"
#include "screen_buffer.h"

void DrawTextCard(ScreenBuffer* buffer, int x, int y, Card* card);

#endif