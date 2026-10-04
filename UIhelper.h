#ifndef UIHELPER_H
#define UIHELPER_H

#include"WinFix.h"

#include <iostream>
#include <string>
#include <limits>
#include <ctime>

using namespace std;

// --- UI Engine Functions ---
// --- Advanced UI Additions ---
void drawHeader(); // Displays the LOGISTICS 360 title at the top
void drawShadow(int x, int y, int w, int h); // Makes the window look 3D
void clearWorkArea(); // Clears the center space without flickering the header
void setCursor(int x, int y);
void hideCursor();
void showCursor();
// Fixed: drawWindow is now defined to create the "Container" for your menus
void drawWindow(int x, int y, int w, int h, const string& title, int colorCode);
int getMenuSelection(string title, string options[], int total);
void printCentered(string text, int y, const string& color);
string centerText(string text, int width);

// --- Utility ---
inline void clearScreen() { system("cls"); }

inline void pauseScreen() {
    // Uses coordinate 22 to keep it at the bottom of the centered window
    printCentered("Press Enter to continue...", 22, "\033[1;36m");
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    std::cin.get();
}

string getCurrentTime();
#endif
