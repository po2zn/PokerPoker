#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#include "card_swap.h"
#include "../data/card.h"
#include "../data/card_string.h"
#include "../data/deck.h"
#include "../data/player.h"
#include "../data/table.h"
#include "../system/console.h"

/* ──────────────────────────────────────────────
   키코드 (Windows 방향키: 0xE0 프리픽스)
────────────────────────────────────────────── */
#define KEY_UP            0x48
#define KEY_DOWN          0x50
#define KEY_ENTER         '\r'
#define KEY_ESC           0x1B
#define KEY_SPECIAL_PREFIX 0xE0

/* 한 화면에 보여줄 카드 행 수 */
#define VIEWPORT_SIZE 16

/* ──────────────────────────────────────────────
   무늬 / 숫자 이름
────────────────────────────────────────────── */
static const char* SuitName(int suit)
{
    switch (suit)
    {
    case 0: return "♠";
    case 1: return "♥";
    case 2: return "◆";
    case 3: return "♣";
    }

    return "?";
}

static const char* NumberName(int number)
{
    static const char* names[] =
    { "?", "A", "2", "3", "4", "5", "6",
      "7", "8", "9", "10", "J", "Q", "K" };
    if (number < 1 || number > 13) return "?";
    return names[number];
}

/* ──────────────────────────────────────────────
   카드를 무늬 우선 · 숫자 순으로 정렬한 인덱스 배열 생성
   순서: SPADE(0~12) → HEART(13~25) → DIAMOND(26~38) → CLUB(39~51)
────────────────────────────────────────────── */
static void BuildSortedIndex(Deck* deck, int outIndex[DECK_SIZE])
{
    int pos = 0;
    for (int suit = 0; suit < 4; suit++)
        for (int number = 1; number <= 13; number++)
            for (int i = 0; i < DECK_SIZE; i++)
                if (deck->cards[i].suit == suit && deck->cards[i].number == number)
                {
                    outIndex[pos++] = i;
                    break;
                }
}

/* ──────────────────────────────────────────────
   원형 연결 리스트 생성 (정렬 순서대로)
────────────────────────────────────────────── */
static CardNode* BuildCircularList(Deck* deck)
{
    CardNode* nodes = (CardNode*)malloc(sizeof(CardNode) * DECK_SIZE);
    if (nodes == NULL) return NULL;

    int sortedIndex[DECK_SIZE];
    BuildSortedIndex(deck, sortedIndex);

    for (int i = 0; i < DECK_SIZE; i++)
    {
        nodes[i].deckIndex = sortedIndex[i];
        nodes[i].isUsed = 0;
        nodes[i].prev = &nodes[(i - 1 + DECK_SIZE) % DECK_SIZE];
        nodes[i].next = &nodes[(i + 1) % DECK_SIZE];
    }

    return nodes;
}

/* ──────────────────────────────────────────────
   뷰포트 렌더링
   cursorPos : 현재 커서가 있는 노드 인덱스 (0~51)
   viewTop   : 뷰포트 첫 줄 인덱스
   nodes     : 정렬된 원형 리스트
────────────────────────────────────────────── */
static void RenderListUI(Deck* deck, CardNode* nodes,
    int cursorPos, int viewTop,
    Card* targetCard)
{
    ClearConsole();

    /* 교체 대상 카드 표시 */
    printf("\n");
    printf("  ┌────────────────────────────────────────────────┐\n");
    printf("  │  교체 대상 :  %-2s %-2s                            │\n",
        NumberName(targetCard->number),
        SuitName(targetCard->suit));
    printf("  │  ↑ ↓ 이동   Enter 선택   ESC 취소            │\n");
    printf("  ├───┬────────────────────────────────────────────┤\n");

    int viewBottom = viewTop + VIEWPORT_SIZE;
    if (viewBottom > DECK_SIZE) viewBottom = DECK_SIZE;

    for (int i = viewTop; i < viewBottom; i++)
    {
        Card c = deck->cards[nodes[i].deckIndex];

        /* 무늬 경계선: 스페이드 끝 / 하트 끝 / 다이아 끝 */
        if (i == viewTop && i > 0)
        {
            /* 뷰포트 중간에서 시작할 때 구분선 생략 — 자연스럽게 이어지도록 */
        }

        if (i == cursorPos)
            printf("  │ ▶ │  %2d.  %-2s %-2s                                │\n",
                i + 1, NumberName(c.number), SuitName(c.suit));
        else
            printf("  │   │  %2d.  %-2s %-2s                                │\n",
                i + 1, NumberName(c.number), SuitName(c.suit));

        /* 무늬 경계 구분선: 13번째 카드 뒤 (스페이드→하트, 하트→다이아, 다이아→클로버) */
        if ((i == 12 || i == 25 || i == 38) && i < viewBottom - 1)
            printf("  ├───┽────────────────────────────────────────────┤\n");
    }

    printf("  └───┴────────────────────────────────────────────┘\n");

    /* 스크롤 위치 표시 */
    printf("  ( %d ~ %d / 52 )\n\n", viewTop + 1, viewBottom);
}

/* ──────────────────────────────────────────────
   핵심 교체 함수
   swapTarget: 0 또는 1 (플레이어 핸드 인덱스)
   반환값: 1 성공, 0 취소
────────────────────────────────────────────── */
int SwapCard(Player* player, Deck* deck, int swapTarget)
{
    if (player == NULL || deck == NULL) return 0;

    CardNode* nodes = BuildCircularList(deck);
    if (nodes == NULL) return 0;

    int cursorPos = 0;   /* 현재 커서 위치 (0~51) */
    int viewTop = 0;   /* 뷰포트 상단 인덱스 */
    int result = 0;

    while (1)
    {
        RenderListUI(deck, nodes, cursorPos, viewTop, &player->hand[swapTarget]);

        int ch = _getch();

        if (ch == 0x00 || ch == KEY_SPECIAL_PREFIX)
        {
            ch = _getch();

            if (ch == KEY_UP)
            {
                cursorPos = (cursorPos - 1 + DECK_SIZE) % DECK_SIZE;

                /* 뷰포트 위로 스크롤 */
                if (cursorPos < viewTop)
                    viewTop = cursorPos;

                /* 원형: 51 → 0 로 넘어갈 때 뷰포트를 마지막 페이지로 */
                if (cursorPos == DECK_SIZE - 1)
                {
                    viewTop = DECK_SIZE - VIEWPORT_SIZE;
                    if (viewTop < 0) viewTop = 0;
                }
            }
            else if (ch == KEY_DOWN)
            {
                cursorPos = (cursorPos + 1) % DECK_SIZE;

                /* 뷰포트 아래로 스크롤 */
                if (cursorPos >= viewTop + VIEWPORT_SIZE)
                    viewTop = cursorPos - VIEWPORT_SIZE + 1;

                /* 원형: 51 → 0 으로 넘어갈 때 뷰포트를 처음으로 */
                if (cursorPos == 0)
                    viewTop = 0;
            }
            continue;
        }

        if (ch == KEY_ENTER)
        {
            player->hand[swapTarget] = deck->cards[nodes[cursorPos].deckIndex];

            ClearConsole();
            Card selected = player->hand[swapTarget];
            printf("\n  ★ 카드가  [ %-3s %s ]  로 교체되었습니다!\n\n",
                NumberName(selected.number), SuitName(selected.suit));
            Sleep(1000);

            result = 1;
            break;
        }

        if (ch == KEY_ESC)
        {
            ClearConsole();
            printf("\n  카드 교체를 취소했습니다.\n\n");
            Sleep(600);
            result = 0;
            break;
        }
    }

    free(nodes);
    return result;
}

/* ──────────────────────────────────────────────
   외부 진입점: 교체 대상 카드 먼저 선택
────────────────────────────────────────────── */
void OpenCardSwapMenu(Player* player, Table* table, Deck* deck)
{
    if (player == NULL || deck == NULL)
    {
        printf("  [오류] 카드 교체 불가: 잘못된 포인터\n");
        return;
    }

    ClearConsole();

    printf("\n");
    printf("  ┌──────────────────────────────────────────┐\n");
    printf("  │          교체할 카드를 선택하세요        │\n");
    printf("  ├──────────────────────────────────────────┤\n");
    printf("  │   1.  %-2s %-2s                              │\n",
        NumberName(player->hand[0].number), SuitName(player->hand[0].suit));
    printf("  │   2.  %-2s %-2s                              │\n",
        NumberName(player->hand[1].number), SuitName(player->hand[1].suit));
    printf("  │   3.  %-2s %-2s                              │\n",
        NumberName(player->hand[2].number), SuitName(player->hand[2].suit));
    printf("  │   0.  취소                               │\n");
    printf("  └──────────────────────────────────────────┘\n");
    printf("\n  선택 >> ");

    int choice;
    if (scanf_s("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        printf("  잘못된 입력입니다.\n");
        return;
    }

    if (choice == 0)
    {
        printf("  취소되었습니다.\n");
        return;
    }

    if (choice != 1 && choice != 2)
    {
        printf("  잘못된 입력입니다.\n");
        return;
    }

    SwapCard(player, deck, choice - 1);
}