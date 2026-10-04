#include <iostream>
#include "Menu.h"
#include "AdminMenu.h"
#include "UserMenu.h"
#include "Validator.h"
#include "UIhelper.h"
#include "SortingModule.h"
#include "routing.h"
#include "ParcelTracking.h"
#include "CourierSystem.h"

using namespace std;
int main() {
    // Systems Initialization
    Sorting parcelHeap; 
    routing routeSystem;
    ParcelTrackingSystem tracker;
    CourierSystem courier;

    string mainOptions[] = { "Admin Login", "User Login", "Exit System" };
    UserMenu user(tracker, parcelHeap, courier); 
    AdminMenu admin(parcelHeap, routeSystem, tracker, courier);

    while (true) {
        // Center the login menu in a 40-character wide window
        int choice = getMenuSelection("LOGISTICS MASTER v1.0", mainOptions, 3);
        switch (choice) {
            case 1: {
                admin.showAdminMenu();
                break;
            }
            case 2:
                user.showUserMenu();
                break;
            case 3:
                clearScreen();
                printCentered("SHUTTING DOWN SYSTEM...", 12, "\033[1;31m");
                return 0;
        }
    }
}