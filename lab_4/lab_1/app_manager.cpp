#include "app_manager.h"
#include "myLine.h"
#include "explosion.h"
#include "winFunc.h"
#include "array_list.h"
#include "linked_list.h"
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
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
    bool flagExplosionChance = false;
    bool flagMinExplosionRadius = false;
    bool flagMaxExplosionRadius = false;
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

    while (!flagExplosionChance) {
        cout << "Explosion probability (1-1000): ";
        int p;
        cin >> p;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (p >= 1 && p <= 1000) {
            this->explosionChance = p;
            flagExplosionChance = true;
        }
        else {
            cout << "Error: probability must be from 1 to 1000.\n";
        }
    }

    while (!flagMinExplosionRadius) {
        cout << "Min explosion radius (1-10): ";
        int r;
        cin >> r;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (r >= 1 && r <= 10) {
            this->minExplosionRadius = r;
            flagMinExplosionRadius = true;
        }
        else {
            cout << "Error: min radius must be from 1 to 10.\n";
        }
    }

    while (!flagMaxExplosionRadius) {
        cout << "Max explosion radius (1-10): ";
        int r;
        cin >> r;
        if (cin.fail()) {
            cout << "Error: invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (r >= 1 && r <= 10 && r >= this->minExplosionRadius) {
            this->maxExplosionRadius = r;
            flagMaxExplosionRadius = true;
        }
        else {
            cout << "Error: max radius must be from 1 to 10 and >= min radius.\n";
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

    ArrayList<int> offsets; // ms offsets for line generation within a second
    offsets.reserve(this->frequency);
    auto generateOffsets = [&]() {
        offsets.clear();
        for (int i = 0; i < this->frequency; i++) {
            offsets.push_back(rand() % 1000);
        }
        if (offsets.size() > 1) {
            sort(offsets.begin(), offsets.end());
        }
    };

    generateOffsets();
    size_t nextIndex = 0;
    LinkedList<myLine> lines;          // active lines
    LinkedList<Explosion> explosions;  // active explosions

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
            lines.push_back(myLine(this->consoleWidth, this->consoleHeight, startCenterY, this->radius, this->velocity, this->epilepsy, centerX));
            nextIndex++;
        }

        for (size_t i = 0; i < lines.size();) {
            myLine& line = lines.at(i);
            line.setDimensions(this->consoleWidth, this->consoleHeight);
            line.update(now, this->explosionChance, this->minExplosionRadius, this->maxExplosionRadius, explosions);
            if (line.isFinished()) {
                lines.remove(i);
            }
            else {
                i++;
            }
        }

        for (size_t i = 0; i < explosions.size();) {
            Explosion& exp = explosions.at(i);
            exp.setDimensions(this->consoleWidth, this->consoleHeight);
            exp.update(now);
            if (exp.isFinished()) {
                explosions.remove(i);
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

app_manager::app_manager(char* frequencyArg, char* velocityArg, char* radiusArg, char* explosionChanceArg, char* minExplosionRadiusArg, char* maxExplosionRadiusArg, char* epilepsyArg) {
    setlocale(0, "");
    char _ep = epilepsyArg[0];
    int _f = atoi(frequencyArg);
    int _v = atoi(velocityArg);
    int _r = atoi(radiusArg);
    int _p = atoi(explosionChanceArg);
    int _minR = atoi(minExplosionRadiusArg);
    int _maxR = atoi(maxExplosionRadiusArg);

    this->frequency = _f;
    this->velocity = _v;
    this->radius = _r;
    this->explosionChance = _p;
    this->minExplosionRadius = _minR;
    this->maxExplosionRadius = _maxR;

    winFunc functional;
    functional.setConsoleSize();
    this->consoleHeight = functional.getCsHeight();
    this->consoleWidth = functional.getCsWidth();

    bool needInteractive = false;
    if (!(_f >= 1 && _f <= 30 &&
        _v >= 1 && _v <= 30 &&
        _r >= 1 && _r <= 50 &&
        _p >= 1 && _p <= 1000 &&
        _minR >= 1 && _minR <= 10 &&
        _maxR >= 1 && _maxR <= 10 &&
        _maxR >= _minR &&
        (_ep == 'Y' || _ep == 'y' || _ep == 'N' || _ep == 'n'))) {
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
