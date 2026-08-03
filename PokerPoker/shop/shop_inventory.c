#include "shop_inventory.h"

ShopInventory gInventory;

void InitializeInventory()
{
    gInventory.isLuckyChip = false;
    gInventory.isVIPTable = false;
    gInventory.isMoreCard = false;
    gInventory.isUnderDraw = false;
    gInventory.isEnding = false;
}