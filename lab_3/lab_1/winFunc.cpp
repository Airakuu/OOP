#include "winFunc.h"
#include <iostream>
#include <Windows.h>
#include <stdlib.h>
using namespace std;

void winFunc::setConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        csWidth = 80;
        csHeight = 25;
        return;
    }
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        csWidth = 80;
        csHeight = 25;
        return;
    }
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    this->csWidth = width;
    this->csHeight = height;
}

int winFunc::getCsWidth() {
    return this->csWidth;
}

int winFunc::getCsHeight() {
    return this->csHeight;
}

bool winFunc::changeCursor() {
    CONSOLE_CURSOR_INFO cci;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return FALSE;
    if (!GetConsoleCursorInfo(hStdOut, &cci)) return FALSE;
    cci.bVisible = FALSE;
    if (!SetConsoleCursorInfo(hStdOut, &cci)) return FALSE;
    return TRUE;
}

void winFunc::cleanConsole() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    if (!GetConsoleScreenBufferInfo(console, &screen)) return;

    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void winFunc::GotoXY(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hStdOut, coord);
}

void winFunc::setColor(bool epilepsy) {
    if (epilepsy) {
        int color = rand() % 15 + 1;
        setColorValue((short)color);
    }
    else {
        setColorValue(10);
    }
}

void winFunc::setColorValue(short color) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)color);
}

winFunc::~winFunc() {
    // можно вернуть цвет по умолчанию при выходе, но не требуется
}
