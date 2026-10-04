#ifndef MENU_H

#define MENU_H

class Menu {
public:
    // Colors for the Terminal
    const char* RESET = "\033[0m";
    const char* RED = "\033[31m";
    const char* GREEN = "\033[32m";
    const char* YELLOW = "\033[33m";
    const char* BLUE = "\033[34m";
    const char* CYAN = "\033[36m";

    void displayHeader();
    void displayMainMenu();
    int getUserInput();
    void clearScreen();
};

#endif