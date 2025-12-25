#pragma once
class winFunc
{
private:
    int csWidth;
    int csHeight;
public:
    void setConsoleSize();
    bool changeCursor();
    void cleanConsole();
    void GotoXY(int X, int Y);
    void setColor(bool epilepsy); // устанавливает цвет консоли: либо зеленый, либо случайный (для ромба)
    int getCsWidth();
    int getCsHeight();
    ~winFunc();
};
