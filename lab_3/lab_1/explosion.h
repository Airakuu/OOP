#pragma once
#include <Windows.h>
#include "figure.h"

class Explosion : public figure
{
private:
    int centerX;
    int centerY;
    int csWidth;
    int csHeight;
    int currentRadius;
    int minRadius;
    int maxRadius;
    bool started;
    bool finished;
    ULONGLONG nextStepAt;

    void drawRadius(int radius, bool erase);

public:
    Explosion(int width, int height, int centerX, int centerY, int minRadius, int maxRadius);
    void update(ULONGLONG nowMs);
    void setDimensions(int width, int height);
    bool isFinished() const;
    void draw() override;
    void erase() override;
};
