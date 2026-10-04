
#include "UserMenu.h"
#include "UIHelper.h"
#include "Validator.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>
#include <conio.h>

using namespace std;

UserMenu::UserMenu(ParcelTrackingSystem& trackerRef, 
                   Sorting& heapRef, 
                   CourierSystem& courierRef)
    : pts(trackerRef), warehouseHeap(heapRef), courierSys(courierRef) {}


void UserMenu::trackParcelLocation(int parcelId) {
    cout << CYAN << "\n" << string(40, '=') << RESET << endl;
    cout << BOLD << "   PARCEL LOCATION TRACKER   \n" << RESET;
    cout << CYAN << string(40, '=') << RESET << endl;
    
    string strId = to_string(parcelId);
    bool found = false;
    
    // 1. CHECK WAREHOUSE (Module 1 - Sorting Heap)
    if (warehouseHeap.idExists(parcelId)) {
        cout << GREEN << " CURRENT LOCATION: WAREHOUSE\n" << RESET;
        cout << "   • Status: Awaiting Dispatch\n";
        cout << "   • Storage: Priority Sorting Heap\n";
        cout << "   • Action: Will be assigned to rider soon\n";
        found = true;
    }
    
    // 2. CHECK TRANSIT (Module 4 - Courier System)
    else if (courierSys.isParcelInTransit(strId)) {
        cout << YELLOW << " CURRENT LOCATION: IN TRANSIT\n" << RESET;
        cout << "   • Status: On the Road\n";
        cout << "   • Queue: Courier Transit Queue (FIFO)\n";
        cout << "   • Action: Being delivered to destination\n";
        
        //  Rider info
        CourierParcel* p = courierSys.getTransitParcelInfo(strId);
        if (p && p->riderID != "") {
            cout << "   • Assigned Rider ID: " << p->riderID << endl;
        }
        found = true;
    }
    
    // 3. CHECK DELIVERED (Module 4 - Delivered History)
    else if (courierSys.isParcelDelivered(strId)) {
        cout << GREEN << "CURRENT LOCATION: DELIVERED\n" << RESET;
        cout << "   • Status: Successfully Delivered\n";
        found = true;
    }
    
    else {
        cout << BLUE << " CURRENT LOCATION: IN TRACKING SYSTEM\n" << RESET;
        // Show current status from Module 3
        pts.showCurrentStatus(parcelId);
        found = true;
    }
    
    if (!found) {
        cout << RED << "\n PARCEL NOT FOUND IN SYSTEM!\n" << RESET;
    }
    
    cout << CYAN << "\n" << string(40, '=') << RESET << endl;
}
/* ================= MAIN USER MENU ================= */

void UserMenu::showUserMenu()
{
    string opts[] = {
        "Track Parcel Status",
        "View Parcel Timeline", 
        "Cancel Delivery",
        "Back to Main Menu"
    };

    while (true)
    {
        int choice = getMenuSelection("USER DASHBOARD", opts, 4); 
        clearScreen();

        switch (choice)
        {
        case 1: // Track Parcel Status
        {
            int id = Validation::inputIntegerUpto(4, "Enter Parcel ID: ");
            trackParcelLocation(id);
            pauseScreen();
            break;
        }

        case 2: // View Parcel Timeline
        {
            int id = Validation::inputIntegerUpto(4, "Enter Parcel ID: ");
            clearScreen();
            cout << CYAN << "\n" << string(50, '=') << RESET << endl;
            cout << BOLD << "     PARCEL TIMELINE VIEWER     \n" << RESET;
            cout << CYAN << string(50, '=') << RESET << endl;
    
            pts.showParcelHistory(id);  
            pauseScreen();
            break;
        }
        

        case 3: // Cancel Delivery
        {
            int id = Validation::inputIntegerUpto(4, "Enter Parcel ID to cancel delivery: ");
            if (id == -1) break;
    
            clearScreen();
            cout << CYAN << "\n" << string(50, '=') << RESET << endl;
            cout << BOLD << "     CANCEL DELIVERY REQUEST     \n" << RESET;
            cout << CYAN << string(50, '=') << RESET << endl;
    
            pts.cancelDelivery(id, warehouseHeap, courierSys);
            pauseScreen();
            break;
        }
        case 4:
            return;
            
        default:
            cout << RED << "Invalid choice!" << RESET << endl;
            pauseScreen();
            break;
        }
    }
}