#include "myLine.h"
#include "symbol.h"
#include "winFunc.h"
#include <iostream>
#include <Windows.h>
#include <stdlib.h>

using namespace std;

/*
    Vertical line pattern with alternating rows:
    even rows: single symbol at centerX
    odd rows:  two symbols at centerX - 1 and centerX + 1
*/

bool myLine::moveLine() {
    winFunc wf;
    wf.setConsoleSize();
    csHeight = wf.getCsHeight();
    csWidth = wf.getCsWidth();

    wf.setColor(this->epilepsy);

    symbol symb;

    int height = radius;
    int prevTopY = this->centerY - 1;

    auto drawAt = [&](int topY, bool erase) {
        for (int row = 0; row < height; row++) {
            int y = topY + row;
            if (y < 0 || y >= csHeight) continue;

            bool single = (row % 2 == 0);
            if (single) {
                if (centerX >= 0 && centerX < csWidth) {
                    if (erase) symb.deleteSymbol(centerX, y);
                    else symb.printSymbol(centerX, y);
                }
            }
            else {
                int xLeft = centerX - 1;
                int xRight = centerX + 1;
                if (xLeft >= 0 && xLeft < csWidth) {
                    if (erase) symb.deleteSymbol(xLeft, y);
                    else symb.printSymbol(xLeft, y);
                }
                if (xRight >= 0 && xRight < csWidth) {
                    if (erase) symb.deleteSymbol(xRight, y);
                    else symb.printSymbol(xRight, y);
                }
            }
        }
    };

    while (this->centerY <= csHeight) {
        drawAt(prevTopY, true);
        drawAt(centerY, false);

        prevTopY = centerY;
        centerY++;
        Sleep(velocityMs);
    }

    drawAt(prevTopY, true);

    return true;
}

myLine::myLine(int width, int startCenterY, int radius, int velocity, bool epilepsy, int centerX) {
    this->csWidth = width;
    this->centerY = startCenterY;
    this->radius = radius;
    this->centerX = centerX;
    this->velocityMs = 1000 / max(1, velocity);
    this->epilepsy = epilepsy;
}
