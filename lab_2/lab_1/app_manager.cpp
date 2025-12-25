#include "app_manager.h"
#include "myLine.h"
#include "winFunc.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
#include <algorithm>
using namespace std;

int app_manager::generateX() {
    int minX = this->radius;
    int maxX = this->consoleWidth - 1 - this->radius;
    if (maxX < minX) return this->radius;
    int x = minX + rand() % (maxX - minX + 1);
    return x;
}

void app_manager::getArguments() {
    bool flagFreq = false;
    bool flagVel = false;
    bool flagRadius = false;
    bool flagEpilepsy = false;

    while (!flagFreq) {
        cout << "Line frequency (1-30): ";
        int f;
        cin >> f;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (f >= 1 && f <= 30) {
            this->frequency = f;
            flagFreq = true;
        }
        else {
            cout << "Error: frequency must be from 1 to 30.\n";
        }
    }

    while (!flagVel) {
        cout << "Speed (1-30): ";
        int v;
        cin >> v;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (v >= 1 && v <= 30) {
            this->velocity = v;
            flagVel = true;
        }
        else {
            cout << "Error: speed must be from 1 to 30.\n";
        }
    }

    while (!flagRadius) {
        cout << "Line length: ";
        int r;
        cin >> r;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (r >= 1 && r <= 50) {
            this->radius = r;
            flagRadius = true;
        }
        else {
            cout << "Error: length must be >=1 and reasonably small.\n";
        }
    }

    while (!flagEpilepsy) {
        cout << "Epilepsy mode Y/N: ";
        char e;
        cin >> e;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
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
            cout << "Error: enter Y or N.\n";
        }
    }
}

void app_manager::startMove() {
    winFunc functional;
    functional.setConsoleSize();
    this->consoleHeight = functional.getCsHeight();
    this->consoleWidth = functional.getCsWidth();

    const int tickMs = 10;
    ULONGLONG secondStart = GetTickCount64();

    vector<int> offsets;
    offsets.reserve(this->frequency);
    auto generateOffsets = [&]() {
        offsets.clear();
        for (int i = 0; i < this->frequency; i++) {
            offsets.push_back(rand() % 1000);
        }
        sort(offsets.begin(), offsets.end());
    };

    generateOffsets();
    size_t nextIndex = 0;
    vector<myLine> lines;

    while (true) {
        ULONGLONG now = GetTickCount64();

        functional.setConsoleSize();
        this->consoleHeight = functional.getCsHeight();
        this->consoleWidth = functional.getCsWidth();

        while (now - secondStart >= 1000) {
            secondStart += 1000;
            generateOffsets();
            nextIndex = 0;
        }

        int elapsed = (int)(now - secondStart);
        while (nextIndex < offsets.size() && offsets[nextIndex] <= elapsed) {
            int centerX = generateX();
            int startCenterY = 1 - this->radius;
            lines.emplace_back(this->consoleWidth, this->consoleHeight, startCenterY, this->radius, this->velocity, this->epilepsy, centerX);
            nextIndex++;
        }

        for (size_t i = 0; i < lines.size();) {
            lines[i].setDimensions(this->consoleWidth, this->consoleHeight);
            lines[i].update(now);
            if (lines[i].isFinished()) {
                lines.erase(lines.begin() + i);
            }
            else {
                i++;
            }
        }

        Sleep(tickMs);
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

app_manager::app_manager(char* frequencyArg, char* velocityArg, char* radiusArg, char* epilepsyArg) {
    setlocale(0, "");
    char _ep = epilepsyArg[0];
    int _f = atoi(frequencyArg);
    int _v = atoi(velocityArg);
    int _r = atoi(radiusArg);

    this->frequency = _f;
    this->velocity = _v;
    this->radius = _r;

    winFunc functional;
    functional.setConsoleSize();
    this->consoleHeight = functional.getCsHeight();
    this->consoleWidth = functional.getCsWidth();

    bool needInteractive = false;
    if (!(_f >= 1 && _f <= 30 && _v >= 1 && _v <= 30 && _r >= 1 && _r <= 50 && (_ep == 'Y' || _ep == 'y' || _ep == 'N' || _ep == 'n'))) {
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
