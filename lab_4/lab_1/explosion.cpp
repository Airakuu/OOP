#include "explosion.h"
#include "symbol.h"
#include "winFunc.h"
#include <stdlib.h>

void Explosion::drawRadius(int radius, bool erase) {
    if (radius < 0) return;

    symbol symb;
    if (!erase) {
        winFunc wf;
        wf.setColorValue((short)(rand() % 15 + 1));
    }

    if (radius == 0) {
        if (centerX >= 0 && centerX < csWidth && centerY >= 0 && centerY < csHeight) {
            if (erase) symb.deleteSymbol(centerX, centerY);
            else symb.printSymbol(centerX, centerY);
        }
        return;
    }

    for (int dy = -radius; dy <= radius; dy++) {
        int dx = radius - abs(dy);
        int x1 = centerX - dx;
        int x2 = centerX + dx;
        int y = centerY + dy;
        if (y < 0 || y >= csHeight) continue;
        if (x1 >= 0 && x1 < csWidth) {
            if (erase) symb.deleteSymbol(x1, y);
            else symb.printSymbol(x1, y);
        }
        if (dx != 0 && x2 >= 0 && x2 < csWidth) {
            if (erase) symb.deleteSymbol(x2, y);
            else symb.printSymbol(x2, y);
        }
    }
}

void Explosion::update(ULONGLONG nowMs) {
    if (finished) return;
    if (!started) {
        started = true;
        stepTimes.clear();
        stepIndex = 0;
        // Start ms schedule for explosion animation.
        stepTimes.push_back(nowMs);
    }

    const int stepMs = 500;
    while (!finished && stepIndex < stepTimes.size() && nowMs >= stepTimes[stepIndex]) {
        if (currentRadius >= minRadius) {
            erase();
        }

        currentRadius++;
        if (currentRadius < minRadius) {
            currentRadius = minRadius;
        }
        if (currentRadius > maxRadius) {
            finished = true;
            break;
        }

        draw();
        ++stepIndex;
        if (!finished) {
            stepTimes.push_back(stepTimes[stepIndex - 1] + stepMs);
        }
    }
}

void Explosion::setDimensions(int width, int height) {
    csWidth = width;
    csHeight = height;
}

bool Explosion::isFinished() const {
    return finished;
}

void Explosion::draw() {
    drawRadius(currentRadius, false);
}

void Explosion::erase() {
    drawRadius(currentRadius, true);
}

Explosion::Explosion(int width, int height, int centerX, int centerY, int minRadius, int maxRadius) {
    this->csWidth = width;
    this->csHeight = height;
    this->centerX = centerX;
    this->centerY = centerY;
    this->minRadius = minRadius;
    this->maxRadius = maxRadius;
    this->currentRadius = minRadius - 1;
    this->started = false;
    this->finished = false;
    this->stepIndex = 0;
}
