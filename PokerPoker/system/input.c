#include <conio.h>

#include "input.h"

void WaitAnyKey()
{
    _getch();
}

char GetInputKey() {
    return _getch();
}