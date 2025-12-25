#pragma once

class app_manager
{
private:
    int consoleWidth;
    int consoleHeight;
    int radius;      // радиус ромба (length)
    int velocity;    // символов в секунду (1..30)
    bool epilepsy;   // true - случайный цвет для каждого ромба, false - зеленый

public:
    void getArguments();
    int generateX(); // случайный X-центр для ромба
    void startMove();
    app_manager(char* velocity, char* radius, char* epilepsy);
    app_manager();
};
