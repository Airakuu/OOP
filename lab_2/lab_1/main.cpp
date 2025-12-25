#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <cstring>
#include "app_manager.h"

using namespace std;

void printHelp() {
    cout << "Usage:\n";
    cout << "  matrix.exe <frequency> <velocity> <length> <Y|N>\n\n";
    cout << "Parameters:\n";
    cout << "  frequency - lines per second (1..30)\n";
    cout << "  velocity  - movement speed (1..30)\n";
    cout << "  length    - line length\n";
    cout << "  Y         - epilepsy mode ON\n";
    cout << "  N         - epilepsy mode OFF\n\n";
    cout << "Example:\n";
    cout << "  matrix.exe 3 23 8 Y\n\n";
    cout << "Help:\n";
    cout << "  matrix.exe --help\n";
    cout << "  matrix.exe /?\n";
}

int main(int argc, char** argv) {

    // Initialize random generator
    srand((unsigned)time(NULL));
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Help mode
    if (argc == 2 &&
        (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "/?") == 0)) {
        printHelp();
        return 0;
    }

    // Interactive mode
    if (argc == 1) {
        app_manager manager;
        return 0;
    }

    // CLI mode
    if (argc == 5) {
        // argv[1] - frequency
        // argv[2] - velocity
        // argv[3] - length
        // argv[4] - epilepsy (Y/N)
        app_manager manager(argv[1], argv[2], argv[3], argv[4]);
        return 0;
    }

    // Invalid parameters
    cout << "Invalid arguments.\n";
    cout << "Use:\n";
    cout << "  matrix.exe --help\n";

    return 1;
}
