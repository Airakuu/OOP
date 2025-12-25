#include "app_manager.h"
#include "myLine.h"
#include "winFunc.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
using namespace std;

int app_manager::generateX() {
    int minX = this->radius;
    int maxX = this->consoleWidth - 1 - this->radius;
    if (maxX < minX) return this->radius;
    int x = minX + rand() % (maxX - minX + 1);
    return x;
}

void app_manager::getArguments() {
    bool flagVel = false;
    bool flagRadius = false;
    bool flagEpilepsy = false;
    while (!flagVel) {
        cout << "Скорость (символов в секунду) 1-30: ";
        int v;
        cin >> v;
        if (cin.fail()) {
            cout << "Ошибка: некорректный ввод.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (v >= 1 && v <= 30) {
            this->velocity = v;
            flagVel = true;
        }
        else {
            cout << "Ошибка: скорость должна быть от 1 до 30.\n";
        }
    }

    while (!flagRadius) {
        cout << "Длина линии: ";
        int r;
        cin >> r;
        if (cin.fail()) {
            cout << "Ошибка: некорректный ввод.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (r >= 1 && r <= 50) {
            this->radius = r;
            flagRadius = true;
        }
        else {
            cout << "Ошибка: длина должна быть >=1 и разумно мала.\n";
        }
    }

    while (!flagEpilepsy) {
        cout << "Режим эпилепсии Y/N (вкл/выкл): ";
        char e;
        cin >> e;
        if (cin.fail()) {
            cout << "Ошибка: некорректный ввод.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (e == 'Y' || e == 'y') {
            this->epilepsy = true;
            flagEpilepsy = true;
        }
        else if (e == 'N' || e == 'n') {
            this->epilepsy = false;
            flagEpilepsy = true;
        }
        else {
            cout << "Ошибка: введите Y или N.\n";
        }
    }
}

void app_manager::startMove() {
    bool cont = true;
    while (cont) {
        int centerX = generateX();
        // Start so bottom row is at the top edge (Y=0)
        // to make a new line appear immediately
        int startCenterY = 1 - this->radius;
        myLine diamond(this->consoleWidth, startCenterY, this->radius, this->velocity, this->epilepsy, centerX);
        cont = diamond.moveLine();
    }
}

app_manager::app_manager() {
    setlocale(0, "");
    winFunc functional;
    getArguments();
    functional.cleanConsole();
    functional.changeCursor();
    functional.setConsoleSize();
    this->consoleHeight = functional.getCsHeight();
    this->consoleWidth = functional.getCsWidth();
    startMove();
}

app_manager::app_manager(char* velocityArg, char* radiusArg, char* epilepsyArg) {
    setlocale(0, "");
    char _ep = epilepsyArg[0];
    int _v = atoi(velocityArg);
    int _r = atoi(radiusArg);

    this->velocity = _v;
    this->radius = _r;

    winFunc functional;
    functional.setConsoleSize();
    this->consoleHeight = functional.getCsHeight();
    this->consoleWidth = functional.getCsWidth();

    bool needInteractive = false;
    if (!(_v >= 1 && _v <= 30 && _r >= 1 && _r <= 50 && (_ep == 'Y' || _ep == 'y' || _ep == 'N' || _ep == 'n'))) {
        needInteractive = true;
    }
    else {
        this->epilepsy = (_ep == 'Y' || _ep == 'y');
    }

    if (needInteractive) getArguments();

    functional.cleanConsole();
    functional.changeCursor();
    startMove();
}
