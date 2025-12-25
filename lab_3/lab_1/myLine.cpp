#include "myLine.h"
#include "symbol.h"
#include "winFunc.h"
#include <Windows.h>
#include <stdlib.h>

using namespace std;

/*
    Vertical line pattern with alternating rows:
    even rows: single symbol at centerX
    odd rows:  two symbols at centerX - 1 and centerX + 1
*/

void myLine::drawAt(int topY, int drawRadius, bool erase) {
    symbol symb;
    if (!erase) {
        winFunc wf;
        wf.setColorValue(this->colorAttr);
    }

    for (int row = 0; row < drawRadius; row++) {
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
}

void myLine::update(ULONGLONG nowMs, int explosionChance, int minExplosionRadius, int maxExplosionRadius, std::vector<Explosion>& explosions) {
    if (finished) return;
    if (!started) {
        started = true;
        nextMoveAt = nowMs;
    }

    while (!finished && nowMs >= nextMoveAt) {
        drawAt(prevTopY, radius, true);

        bool doExplode = false;
        if (!explosionStarted && explosionChance > 0) {
            int roll = rand() % 1000 + 1;
            doExplode = (roll <= explosionChance);
        }

        if (doExplode) {
            int bottomY = centerY + radius - 1;
            explosions.emplace_back(csWidth, csHeight, centerX, bottomY, minExplosionRadius, maxExplosionRadius);
            explosionStarted = true;
        }

        if (explosionStarted) {
            radius -= 1;
            if (radius <= 0) {
                finished = true;
                return;
            }
        }

        drawAt(centerY, radius, false);

        prevTopY = centerY;
        centerY++;
        nextMoveAt += velocityMs;

        if (centerY > csHeight) {
            drawAt(prevTopY, radius, true);
            finished = true;
        }
    }
}

void myLine::setDimensions(int width, int height) {
    csWidth = width;
    csHeight = height;
}

bool myLine::isFinished() const {
    return finished;
}

void myLine::draw() {
    drawAt(centerY, radius, false);
}

void myLine::erase() {
    drawAt(prevTopY, radius, true);
}

myLine::myLine(int width, int height, int startCenterY, int radius, int velocity, bool epilepsy, int centerX) {
    this->csWidth = width;
    this->csHeight = height;
    this->centerY = startCenterY;
    this->radius = radius;
    this->centerX = centerX;
    this->velocityMs = 1000 / (velocity > 0 ? velocity : 1);
    this->epilepsy = epilepsy;
    this->colorAttr = (short)(epilepsy ? (rand() % 15 + 1) : 10);
    this->prevTopY = centerY - 1;
    this->started = false;
    this->finished = false;
    this->explosionStarted = false;
    this->nextMoveAt = 0;
}
