#pragma once
#include <Windows.h>
#include <vector>
#include "figure.h"
#include "explosion.h"

class myLine : public figure
{
private:
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
    ULONGLONG nextMoveAt;

    void drawAt(int topY, int drawRadius, bool erase);

public:
    void update(ULONGLONG nowMs, int explosionChance, int minExplosionRadius, int maxExplosionRadius, std::vector<Explosion>& explosions);
    void setDimensions(int width, int height);
    bool isFinished() const;
    void draw() override;
    void erase() override;
    myLine(int width, int height, int startCenterY, int radius, int velocity, bool epilepsy, int centerX);
};
