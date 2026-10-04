#include "Menu.h"
#include "Validator.h" //
#include <iostream>

using namespace std;

void Menu::clearScreen() {
    // This cleans the terminal screen based on the operating system
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Menu::displayHeader() {
    cout << CYAN << "===========================================" << endl;
    cout << "     SWIFTEX INTELLIGENT LOGISTICS         " << endl;
    cout << "===========================================" << RESET << endl;
}

void Menu::displayMainMenu() {
    displayHeader();
    cout << YELLOW << "1. Parcel Sorting (My Task)" << endl;
    cout << "2. Routing System" << endl;
    cout << "3. Tracking System" << endl;
    cout << "4. Courier Operations" << endl;
    cout << RED << "5. Exit System" << RESET << endl;
    cout << CYAN << "-------------------------------------------" << RESET << endl;
}

int Menu::getUserInput() {
    int choice= Validation::inputInteger(1, "Please select an option: ");
    return choice;
}