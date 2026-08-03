#pragma once
#ifndef CARD_SWAP_H
#define CARD_SWAP_H

#include "../data/player.h"
#include "../data/table.h"
#include "../data/deck.h"

// 원형 연결 리스트 노드
typedef struct CardNode
{
    int          deckIndex;   // deck->cards[deckIndex]
    int          isUsed;      // 예약 (현재 항상 0, 확장용)
    struct CardNode* prev;
    struct CardNode* next;
} CardNode;

// 핵심 교체 함수: swapTarget = 0 or 1 (플레이어 핸드 인덱스)
// 반환값: 교체 성공 1, 취소/실패 0
int SwapCard(Player* player, Deck* deck, int swapTarget);

// 외부 진입점: 교체 대상 카드 선택 → 새 카드 선택 전체 흐름
// table은 현재 미사용이나 시그니처 호환을 위해 유지
void OpenCardSwapMenu(Player* player, Table* table, Deck* deck);

#endif