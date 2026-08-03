#pragma once

#ifndef SHOP_INVENTORY_H
#define SHOP_INVENTORY_H

#include <stdbool.h>

typedef struct ShopInventory
{
    bool isLuckyChip;
    bool isVIPTable;
    bool isMoreCard;
    bool isUnderDraw;
    bool isEnding;

} ShopInventory;

extern ShopInventory gInventory;

void InitializeInventory();

#endif