#pragma once

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

public:
    bool moveLine();
    myLine(int width, int startCenterY, int radius, int velocity, bool epilepsy, int centerX);
};
