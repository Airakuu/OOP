#include "symbol.h"
#include "winFunc.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>

void symbol::printSymbol(int X, int Y) {
    winFunc functional;
    char symb = (char)(rand() % 94 + 33); // ???????? ??????
    functional.GotoXY(X, Y);
    putc(symb, stdout);
}

void symbol::deleteSymbol(int X, int Y) {
    winFunc functional;
    functional.GotoXY(X, Y);
    putc(' ', stdout);
}
