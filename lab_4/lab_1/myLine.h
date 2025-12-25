#pragma once
#include <Windows.h>
#include "linked_list.h"
#include "array_list.h"
#include "figure.h"
#include "explosion.h"

class myLine : public figure
{
private:
    struct SymbolPos {
        int x;
        int y;
    };

    int radius;
    int centerX;
    int centerY;
    int csWidth;
    int csHeight;
    int velocityMs;
    bool epilepsy;
    short colorAttr;
    int prevTopY;
    bool started;
    bool finished;
    bool explosionStarted;
    LinkedList<SymbolPos> symbols;     // store drawn symbol positions for line
    ArrayList<ULONGLONG> moveTimes;    // ms schedule of line steps
    size_t moveIndex;

    void drawAt(int topY, int drawRadius, bool erase);

public:
    void update(ULONGLONG nowMs, int explosionChance, int minExplosionRadius, int maxExplosionRadius, LinkedList<Explosion>& explosions);
    void setDimensions(int width, int height);
    bool isFinished() const;
    void draw() override;
    void erase() override;
    myLine(int width, int height, int startCenterY, int radius, int velocity, bool epilepsy, int centerX);
};
