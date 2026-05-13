#include <stdio.h>

#include "menu_scene.h"

#include "../system/renderer.h"
#include "../system/input.h"
#include "../system/console.h"

#include "../resource/logo.h"

void ShowMenuScene()
{
    ClearConsole();

    DrawText(GAME_LOGO);

    printf("\n       아무 키나 눌러 시작\n");
    WaitAnyKey();
}