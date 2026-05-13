#include <stdio.h>
#include <time.h>

#include "game.h"
#include "../scene/menu_scene.h"
//#include "../ui/main_system_ui.h"
#include "../game/game_flow.h"

void RunGame()
{
    srand((unsigned int)time(NULL));

    ShowMenuScene(); // PokerPoker ·Î°í
    //ShowMainSystemUI(); 

    RunPokerGame();
}