#pragma once
#include <Windows.h>

class myLine
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
    ULONGLONG nextMoveAt;

    void drawAt(int topY, bool erase);

public:
    void update(ULONGLONG nowMs);
    void setDimensions(int width, int height);
    bool isFinished() const;
    myLine(int width, int height, int startCenterY, int radius, int velocity, bool epilepsy, int centerX);
};
