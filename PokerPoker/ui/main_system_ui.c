//#include <stdio.h>
//
//#include "main_system_ui.h"
//
//#include "../system/input.h"
//#include "../system/console.h"
//
//#include "../data/player.h"
//#include "../data/table.h"
//
//#include "../poker/betting_system.h"
//
//void ShowMainSystemUI()
//{
//    Player player;
//    Table table;
//
//    InitializePlayer(&player);
//    InitializeTable(&table);
//
//    while (1)
//    {
//        ClearConsole();
//
//        printf("========== POKER ==========\n");
//
//        printf("Player Chip : %d\n", player.chip);
//        printf("Pot         : %d\n", table.pot);
//        printf("Current Bet : %d\n", table.currentBet);
//
//        printf("\n");
//
//        printf("1. Check\n");
//        printf("2. Bet\n");
//        printf("3. Call\n");
//        printf("4. Fold\n");
//        printf("Q. Exit\n");
//
//        printf("===========================\n");
//
//        char input = GetInputKey();
//
//        switch (input)
//        {
//        case '1':
//            Check(&table);
//            break;
//
//        case '2':
//        {
//            int amount;
//
//            printf("\n베팅 금액 입력: ");
//            scanf_s("%d", &amount);
//
//            Bet(&player, &table, amount);
//
//            break;
//        }
//
//        case '3':
//            Call(&player, &table);
//            break;
//
//        case '4':
//            Fold(&player);
//            break;
//
//        /*case '5':
//            printf("\n현재 칩: %d\n", player.chip);
//            break;*/
//
//        case 'q':
//        case 'Q':
//            return;
//
//        default:
//            printf("\n잘못된 입력입니다.\n");
//            break;
//        }
//
//        printf("\n계속하려면 아무 키나 누르세요...");
//        WaitAnyKey();
//    }
//}