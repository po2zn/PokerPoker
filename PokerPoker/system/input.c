#include <conio.h>

#include "input.h"

void WaitAnyKey()
{
    _getch();
}

char GetInputKey() {
    return _getch();
}

int SetSettings() {
    char ch = _getch();
    if (ch == 'q' || ch == 'Q') return 1;
    return 0;
}