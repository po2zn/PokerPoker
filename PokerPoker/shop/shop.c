#include <stdio.h>
#include <stdlib.h>

#include "shop.h"
#include "item.h"
#include "shop_inventory.h"
#include "coin.h"

#include "../renderer/renderer.h"
#include "../resource/logo.h"

static Item items[] =
{
    {"럭키 코인!", 50},                    // 1판 100코인  -> 50코인
    {"마음의 눈으로 보는 법", 80},       // 50코인 -> 2판 150코인 -> 70코인
    {"한 장 더?", 120},                // 70코인 -> 3판 170코인 -> 50코인
    {"손은 눈보다 빠르다", 200},        // 4판 150코인 -> 5판 250코인 -> 50코인.
    {"엔딩", 250}                     // 6판 150코인 -> 7판 250코인 -> 8판 350코인. // 이론상 최대 8판 승리
};

static int itemCount = sizeof(items) / sizeof(items[0]);

void OpenShop()
{
    int choice;

    while (1)
    {
        printf("\n=== SHOP ===\n");
        printf("MY COIN : %d\n", gCoin.coin);

        for (int i = 0; i < itemCount; i++)
        {
            printf("%d. %s (%d coin)\n", i + 1, items[i].name, items[i].price);
        }

        printf("0. Exit\n");

        scanf_s("%d", &choice);

        if (choice == 0)
        {
            break;
        }

        if (choice < 1 || choice > itemCount)
        {
            printf("\nInvalid Item\n");
            continue;
        }

        Item selected = items[choice - 1];

        if (!UseCoin(selected.price))
        {
            printf("\nNot Enough Coin\n");
            continue;
        }

        if (choice == 5) {
            if (gInventory.isLuckyChip && gInventory.isMoreCard
                && gInventory.isUnderDraw && gInventory.isVIPTable)
            {
                DrawGameLOGO(ENDING_LOGO);
                exit(1);
            }
            else
            {
                printf("\n[경고!] 아직은 엔딩을 구매하실 수 없습니다.\n");
                printf("[경고!] 모든 아이템을 구매하시면 엔딩을 해금하실 수 있습니다.\n");
                continue;
            }
        }

		ReduceCoin(selected.price);

        switch (choice)
        {
        case 1:
            gInventory.isLuckyChip = true;
            break;

        case 2:
            gInventory.isVIPTable = true;
            break;

        case 3:
            gInventory.isUnderDraw = true;
            break;

        case 4:
            gInventory.isMoreCard = true;
            break;

        case 5:
            gInventory.isEnding = true;
            break;
        }

        printf("\nBUY SUCCESS : %s\n", selected.name);
    }
}