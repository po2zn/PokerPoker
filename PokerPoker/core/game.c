#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "game.h"
#include "../audio/audio.h"

#include "../scene/menu_scene.h"
//#include "../ui/main_system_ui.h"
#include "../game/game_flow.h"

#define SOUND_VOLUME 50

void RunGame()
{
    system("mode con: cols=170 lines=50");
    srand((unsigned int)time(NULL));

    InitializeAudio(true);
    SetVolume(SOUND_VOLUME);
	PlayBGM();

    /*Sleep(10000);*/

    ShowMenuScene(); // PokerPoker ·Î°í

    ///*ShowMainSystemUI(); */

    RunPokerGame();
}
