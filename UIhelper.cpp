#include "UIHelper.h"
#include <conio.h>
#include <iomanip>
#include <ctime>

void setCursor(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void drawWindow(int x, int y, int w, int h, const string& title, int colorCode) {
    SetConsoleOutputCP(65001); // Ensure UTF-8 support for borders
    string colorStr = "\033[1;" + to_string(colorCode) + "m";
    
    // Use strings "" instead of ' ' to prevent multi-character warnings
    setCursor(x, y);
    cout << colorStr << "╔";
    for (int i = 0; i < w - 2; i++) cout << "═";
    cout << "╗" << "\033[0m";
    
    for (int i = 1; i < h - 1; i++) {
        setCursor(x, y + i);
        cout << colorStr << "║" << string(w - 2, ' ') << "║" << "\033[0m";
    }
    
    setCursor(x, y + h - 1);
    cout << colorStr << "╚";
    for (int i = 0; i < w - 2; i++) cout << "═";
    cout << "╝" << "\033[0m";
    
    // Centered Title
    int titlePos = x + (w - (int)title.length()) / 2;
    setCursor(titlePos, y);
    cout << "\033[1;33m " << title << " \033[0m";
}

int getMenuSelection(string title, string options[], int total) {
    hideCursor();
    int selected = 0;
    int winWidth = 54;
    int winHeight = total + 6;
    int screenWidth = 80; 
    int startX = (screenWidth - winWidth) / 2; // Center horizontal
    int startY = 5; // Center vertical

    clearScreen();

    while (true) {
        // Draw the frame once per loop update
        drawWindow(startX, startY, winWidth, winHeight, title, 35);

        for (int i = 0; i < total; i++) {
            setCursor(startX + 4, startY + 3 + i);
            if (i == selected) {
                // High-contrast selection bar
                cout << "\033[1;42;37m  ► " << left << setw(winWidth - 12) << options[i] << " \033[0m";
            } else {
                cout << "\033[1;37m    " << left << setw(winWidth - 12) << options[i] << "\033[0m";
            }
        }

        // Instruction footer
        printCentered("Use ↑ ↓ to Navigate • Enter to Select", startY + winHeight, "\033[1;36m");

        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + total) % total;
            else if (key == 80) selected = (selected + 1) % total;
        } else if (key == 13) return selected + 1;
    }
}

void printCentered(string text, int y, const string& color) {
    int x = (80 - text.length()) / 2;
    setCursor(x, y);
    cout << color << text << "\033[0m";
}

//====== moving from usermenu====
// Helper to center text within the window
string centerText(string text, int width) {
    int padding = (width - text.length()) / 2;
    string result = string(padding, ' ') + text;
    result += string(width - result.length(), ' ');
    return result;
}

string getCurrentTime() {
    time_t now = time(0);
    tm* local_time = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", local_time);
    return string(buffer);
}


// #include "UIHelper.h"
// #include "Colors.h"
// #include <conio.h>
// #include <iomanip>
// #include <iostream>

// using namespace std;

// // --- Internal Helper Function ---
// // Fixes the multi-character warning by printing UTF-8 strings in a loop
// void printRepeat(string sym, int count) {
//     for (int i = 0; i < count; i++) {
//         cout << sym;
//     }
// }

// // --- Core Position & Visibility ---
// void setCursor(int x, int y) {
//     COORD coord = { (SHORT)x, (SHORT)y };
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
// }

// void hideCursor() {
//     HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//     CONSOLE_CURSOR_INFO info;
//     info.dwSize = 100;
//     info.bVisible = FALSE;
//     SetConsoleCursorInfo(consoleHandle, &info);
// }

// void showCursor() {
//     HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
//     CONSOLE_CURSOR_INFO info;
//     info.dwSize = 20;
//     info.bVisible = TRUE;
//     SetConsoleCursorInfo(consoleHandle, &info);
// }

// // --- Brand Dashboard (The Header) ---
// void drawHeader() {
//     setCursor(0, 0);
//     // Draw background bar for the brand name
//     cout << "\033[48;5;235m" << string(80, ' ') << RESET << endl; 
    
//     setCursor(2, 0);
//     cout << MAGENTA << BOLD << "LOGISTICS 360" << RESET << WHITE << " | " << BOLD << "GLOBAL CARGO ENGINE" << RESET;
    
//     // Display dynamic system time
//     string t = getCurrentTime();
//     setCursor(80 - (int)t.length() - 2, 0);
//     cout << CYAN << t << RESET;
    
//     setCursor(0, 1);
//     cout << GRAY;
//     printRepeat("═", 80); // Professional double-line separator
//     cout << RESET << endl;
// }

// // --- 3D Shadow Effect ---
// void drawShadow(int x, int y, int w, int h) {
//     // Draws a vertical shadow on the right
//     for (int i = 1; i <= h; i++) {
//         setCursor(x + w, y + i);
//         cout << "\033[90m█" << RESET; 
//     }
//     // Draws a horizontal shadow at the bottom
//     setCursor(x + 1, y + h);
//     cout << "\033[90m";
//     printRepeat("▀", w);
//     cout << RESET;
// }

// // --- Advanced Window Drawing ---
// void drawWindow(int x, int y, int w, int h, const string& title, int colorCode) {
//     SetConsoleOutputCP(65001); // UTF-8 mode for sharp borders
//     string colorStr = "\033[1;" + to_string(colorCode) + "m";
    
//     drawShadow(x, y, w, h); 

//     // Top border
//     setCursor(x, y);
//     cout << colorStr << "╔";
//     printRepeat("═", w - 2);
//     cout << "╗" << RESET;
    
//     // Side borders
//     for (int i = 1; i < h - 1; i++) {
//         setCursor(x, y + i);
//         cout << colorStr << "║" << string(w - 2, ' ') << "║" << RESET;
//     }
    
//     // Bottom border
//     setCursor(x, y + h - 1);
//     cout << colorStr << "╚";
//     printRepeat("═", w - 2);
//     cout << "╝" << RESET;
    
//     // Centered Title within the top border
//     int titlePos = x + (w - (int)title.length()) / 2;
//     setCursor(titlePos - 1, y);
//     cout << YELLOW << BOLD << " " << title << " " << RESET;
// }

// // --- Interactive Selection Engine ---
// int getMenuSelection(string title, string options[], int total) {
//     hideCursor();
//     int selected = 0;
//     int winWidth = 56;
//     int winHeight = total + 6;
//     int screenWidth = 80; 
//     int startX = (screenWidth - winWidth) / 2;
//     int startY = 6; 

//     while (true) {
//         clearScreen();
//         drawHeader(); // Branding persists on all menu screens
//         drawWindow(startX, startY, winWidth, winHeight, title, 34); // Royal Blue Theme

//         for (int i = 0; i < total; i++) {
//             setCursor(startX + 4, startY + 3 + i);
//             if (i == selected) {
//                 // High-contrast highlight for the active selection
//                 cout << "\033[1;44;37m  ► " << left << setw(winWidth - 12) << options[i] << "  " << RESET;
//             } else {
//                 cout << WHITE << "    " << left << setw(winWidth - 12) << options[i] << RESET;
//             }
//         }

//         // Standardized navigation instructions
//         printCentered("NAVIGATE: ↑ ↓  •  SELECT: ENTER", startY + winHeight + 1, CYAN);

//         int key = _getch();
//         if (key == 0 || key == 224) { // Arrow key handling
//             key = _getch();
//             if (key == 72) selected = (selected - 1 + total) % total; // Up
//             else if (key == 80) selected = (selected + 1) % total;     // Down
//         } else if (key == 13) { // Enter key
//             showCursor();
//             return selected + 1;
//         }
//     }
// }

// // --- Positioning Utilities ---
// void printCentered(string text, int y, const string& color) {
//     int x = (80 - (int)text.length()) / 2;
//     setCursor(x, y);
//     cout << color << text << RESET;
// }

// string centerText(string text, int width) {
//     int padding = (width - (int)text.length()) / 2;
//     if (padding < 0) padding = 0;
//     string result = string(padding, ' ') + text;
//     result += string(width - (int)result.length(), ' ');
//     return result;
// }

// string getCurrentTime() {
//     time_t now = time(0);
//     tm* local_time = localtime(&now);
    
//     char buffer[80];
//     strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", local_time);
//     return string(buffer);
// }