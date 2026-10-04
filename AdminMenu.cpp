
#include "AdminMenu.h"
#include "Validator.h"
#include "Colors.h"
#include "UIHelper.h"
#include <iostream>

using namespace std;

/* ================= CONSTRUCTOR ================= */

AdminMenu::AdminMenu(
    Sorting &h,
    routing &r,
    ParcelTrackingSystem &t,
    CourierSystem &c)
    : heap(h), routeSystem(r), tracker(t), courier(c) {}

/* ================= AUTH ================= */

bool AdminMenu::authenticateAdmin()
{
    clearScreen();

    cout << CYAN << BOLD
         << "========================================\n"
         << "          ADMIN AUTHENTICATION          \n"
         << "========================================\n"
         << RESET;

    const string ADMIN_PASSWORD = "Usman@35";
    string password = Validation::checkPassword(
        ADMIN_PASSWORD.length(),
        "Enter Admin Password: ");

    if (password == ADMIN_PASSWORD)
    {
        cout << GREEN << "\nAccess Granted\n"
             << RESET;
        pauseScreen();
        return true;
    }

    cout << RED << "\nWrong Password\n"
         << RESET;
    pauseScreen();
    return false;
}

/* ================= MAIN ADMIN MENU ================= */

void AdminMenu::showAdminMenu()
{
    if (!authenticateAdmin())
        return;

    string opts[] = {
        "Add New Parcel", "Add New Rider", "Logistics Map",
        "Warehouse Inventory", "Intelligent Dispatch",
        "Transit Queue", "Delivered History", "View All Riders (Available/Busy)", "Logout"};

    while (true)
    {

        cout << MAGENTA << BOLD
             << "========================================\n"
             << "            ADMIN CONTROL PANEL         \n"
             << "========================================\n"
             << RESET;

        int choice = getMenuSelection("ADMIN DASHBOARD", opts, 9);
        clearScreen();

        switch (choice)
        {
        case 1:
        { // Add New Parcel
            clearScreen();
            setCursor(20, 5);
            cout << GREEN << BOLD << "--- SECURE PARCEL ENTRY ---" << RESET << endl;

            // 1. Show registered cities so the user knows valid Source/Dest
            routeSystem.displayAllCities();

            int id;
            do
            {
                id = Validation::inputIntegerUpto(4, "Enter Parcel ID: ");
                if (id == -1)
                    break;
                if (heap.idExists(id))
                {
                    cout << RED << "Error: ID " << id << " already exists!\n"
                         << RESET;
                }
            } while (heap.idExists(id));
            if (id == -1)
                break;

            string source = Validation::inputString(20, "Enter Source City  ");
            if (source == "0")
                break;

            if (!routeSystem.isValidCity(source))
            {
                cout << RED << "\n[!] Error: '" << source << "' is not on the Logistics Map.\n"
                     << "Please go to 'Manage Logistics Map' (Option 3) to add it first." << RESET << endl;
                pauseScreen();
                break;
            }

            string destination = Validation::inputString(20, "Enter Destination City  ");
            if (destination == "0")
                break;

            if (!routeSystem.isValidCity(destination))
            {
                cout << RED << "\n[!] Error: '" << destination << "' is not on the Logistics Map." << RESET << endl;
                pauseScreen();
                break;
            }

            float weight = Validation::inputFloat(5, "Enter Weight (kg): ");
            int priority = Validation::inputInteger(1, "Enter Priority (1-Overnight, 2-2Day, 3-Normal): ");
            Validation::checkPriority(priority);

            // Parcel p(id, weight, source, destination, priority);
            // heap.insert(p);
            // tracker.addParcel(&p);

            Parcel *p = new Parcel(id, weight, source, destination, priority);
            heap.insert(*p);
            tracker.addParcel(p);

            //==============
            string timestamp = getCurrentTime();
            tracker.addEvent(id, "Parcel registered at " + source + " warehouse", timestamp);

            cout << GREEN << "\n[Success] Parcel " << id << " added at " << timestamp << RESET << endl;

            cout << GREEN << "\n[Success] Parcel " << id << " added to warehouse." << RESET << endl;
            pauseScreen();
            break;
        }

        case 2: // Add New Rider
            courier.addRider();
            pauseScreen();
            break;

        case 3: // Manage Logistics Map
            manageRouting();
            break;

        case 4: // View Warehouse Inventory
            heap.displayHeap();
            pauseScreen();
            break;

        case 5: // Intelligent Dispatch
        {
            if (heap.isEmpty())
            {
                cout << YELLOW << "\n[!] No parcels available." << RESET << endl;
                pauseScreen();
                break;
            }

            Parcel topParcel = heap.getTop();
            int parcelId = topParcel.getId();

            // string dispatchTime = getCurrentTime();
            // tracker.addEvent(parcelId, " Dispatched from " + topParcel.getSource() + " to " + topParcel.getDestination(), dispatchTime);

            courier.assignParcel(heap, routeSystem, tracker);

            // cout << GREEN << "\nDispatch event recorded at " << dispatchTime << RESET << endl;
            pauseScreen();
            break;
        }

        case 6: // View & Manage Transit Queue
            clearScreen();
            cout << CYAN << BOLD << "--- TRANSIT MANAGEMENT (FIFO) ---" << RESET << endl;

            // Pass 'heap' (the parcel warehouse) and 'routeSystem' (the map)
            courier.viewTransitQueue(heap, routeSystem, tracker);

            pauseScreen();
            break;

        case 7: // NEW: View Delivered History
            clearScreen();
            cout << GREEN << BOLD << "--- DELIVERY HISTORY ---" << RESET << endl;
            courier.viewDeliveredHistory();
            // clearScreen();
            pauseScreen();
            break;
        case 8:
        {
            //  clearScreen();
            courier.displayAllRiders(); // Call your NEW function here
            pauseScreen();
            break;
        }

        case 9: // Back
            return;
        }
    }
}

/* ================= ROUTING MENU ================= */
void AdminMenu::manageRouting()
{
    string mapOptions[] = {
        "Add City",
        "Add Road",
        "Block Shortest Road",
        "Unblock A Road",
        "Show Available Shortest Path",
        "Delete City",
        "Show Full Logistics Network",
        "Back"};

    while (true)
    {
        int choice = getMenuSelection("LOGISTICS MAP CONTROL", mapOptions, 8);
        if (choice == 8) // BACK
            break;

        string src, dest;
        int dist;

        switch (choice)
        {
        case 1:
        { // Add City (Shows current list first)
            clearScreen();
            cout << CYAN << "--- CURRENT CITIES ---" << RESET << endl;
            for (int i = 0; i < routeSystem.cityCount; i++)
                cout << "- " << routeSystem.cities[i] << endl;

            src = Validation::inputString(20, "\nEnter New City or 0 to go back ");
            if (src == "CANCEL_INPUT")
                break;
            routeSystem.AddLocation(src); // Logic inside handles duplicate check
            pauseScreen();
            break;
        }
            // case 2:
            // { // Add Road
            //        clearScreen();
            //     src = Validation::inputString(20, "Source City: ");
            //     dest = Validation::inputString(20, "Destination City: ");
            //     dist = Validation::inputInteger(4, "Enter Distance: ");

            //     routeSystem.AddRoads(src, dest, dist);
            //     cout << GREEN << "\nRoad Added Successfully!" << RESET << endl;
            //     routeSystem.showAllRoadsBetween(src, dest);
            //     pauseScreen();
            //     break;
            // }

        case 2: // Add Road
        {
            clearScreen();
            cout << MAGENTA << BOLD << "============================================" << endl;
            cout << "           INFRASTRUCTURE: ADD ROAD         " << endl;
            cout << "============================================" << RESET << endl;

            // 1. Check if at least 2 cities exist to make a road
            if (routeSystem.cityCount < 2)
            {
                if (routeSystem.cityCount == 0)
                {
                    cout << YELLOW << BOLD << "\n[!] ERROR: No cities found in the registry." << RESET << endl;
                }
                else
                {
                    cout << YELLOW << BOLD << "\n[!] ERROR: Only one city registered. You need at least two cities to build a road." << RESET << endl;
                }
                cout << WHITE << "Press Enter to return to menu..." << RESET;
                cin.ignore();
                cin.get();
                break;
            }

            // 2. Display all registered cities so the user knows valid options
            routeSystem.displayAllCities();

            // 3. Get and Validate Source City
            src = Validation::inputString(20, "\nEnter Source City or press 0 to go back");
            if (src == "CANCEL_INPUT")
                break;
            if (routeSystem.getCityIndex(src) == -1)
            {
                cout << RED << BOLD << "[!] ERROR: City '" << src << "' is not registered on the map!" << RESET << endl;
                pauseScreen();
                break;
            }

            // 4. Get and Validate Destination City
            dest = Validation::inputString(20, "Enter Destination City: ");
            if (routeSystem.getCityIndex(dest) == -1)
            {
                cout << RED << BOLD << "[!] ERROR: City '" << dest << "' is not registered on the map!" << RESET << endl;
                pauseScreen();
                break;
            }

            if (src == dest)
            {
                cout << RED << "[!] ERROR: Source and Destination cannot be the same." << RESET << endl;
                pauseScreen();
                break;
            }

            // 5. Get Distance and build road
            dist = Validation::inputInteger(4, "Enter Road Distance (km): ");

            routeSystem.AddRoads(src, dest, dist);

            cout << GREEN << BOLD << "\n[✓] SUCCESS: Road established between " << src << " and " << dest << RESET << endl;
            routeSystem.showAllRoadsBetween(src, dest);
            pauseScreen();
            break;
        }
            // case 3:
            // { // Block Shortest Road
            //        clearScreen();
            //     src = Validation::inputString(20, "Source City: ");
            //     dest = Validation::inputString(20, "Destination City: ");

            //     routeSystem.showAllRoadsBetween(src, dest); // Show everything first
            //     int shortest = routeSystem.getShortestDistance(src, dest);

            //     if (shortest == -1)
            //     {
            //         cout << RED << "No available roads to block!" << RESET << endl;
            //     }
            //     else
            //     {
            //         cout << YELLOW << "\nShortest Available Road found: " << shortest << " km" << RESET << endl;
            //         char confirm;
            //         cout << "Block this road? (y/n): ";
            //         cin >> confirm;
            //         if (confirm == 'y' || confirm == 'Y')
            //         {
            //             routeSystem.blockSpecificRoad(src, dest, shortest);
            //         }
            //     }
            //     pauseScreen();
            //     break;
            // }
        case 3: // Block Shortest Road
        {
            clearScreen();
            // drawHeader(); // System Branding: LOGISTICS 360

            // 1. Guard: Check if ANY road is ACTIVE in the adjacency list
            bool anyActive = false;
            for (int i = 0; i < routeSystem.cityCount; i++)
            {
                Node *head = routeSystem.h->search(routeSystem.cities[i]);
                if (head)
                {
                    Node *n = head->next;
                    while (n)
                    {
                        // Only look for physical, unblocked road segments
                        if (n->dist != -1 && n->status)
                        {
                            anyActive = true;
                            break;
                        }
                        n = n->next;
                    }
                }
                if (anyActive)
                    break;
            }

            // 2. Early Exit: If the map is empty of active roads
            if (!anyActive)
            {
                cout << RED << BOLD << "\n [!] NO DIRECT ROADS PRESENT TO BLOCK " << RESET << endl;
                cout << WHITE << "\nPress Enter to return to the menu..." << RESET;
                cin.ignore();
                cin.get();
                break;
            }

            // 3. Display DIRECT connections only (Prevents the A-B-C summing issue)
            routeSystem.displayConnectedCitiesShortest();

            cout << WHITE << "\nIdentify the direct road segment to block:" << RESET << endl;

            // 0-to-go-back logic using the new signals
            src = Validation::inputString(20, "Source City");
            if (src == "CANCEL_INPUT")
                break;

            dest = Validation::inputString(20, "Destination City");
            if (dest == "CANCEL_INPUT")
                break;

            // 4. Validation: Strictly fetch direct segment distance
            // This ensures that A to C will return -1 if no direct road exists.
            int shortest = routeSystem.getShortestDistance(src, dest);

            if (shortest == -1)
            {
                cout << RED << BOLD << "\n[!] ERROR: No direct road found between " << src << " and " << dest << RESET << endl;
                cout << YELLOW << "Transit routes via other cities cannot be blocked as one segment." << RESET << endl;
            }
            else
            {
                cout << YELLOW << "\nTargeting Direct Segment: " << WHITE << BOLD << shortest << " km" << RESET << endl;

                string confirm = Validation::inputString(1, "Are you sure you want to BLOCK this road? (y/n): ");
                if (confirm == "y" || confirm == "Y")
                {
                    // Block the specific edge in the graph
                    routeSystem.blockSpecificRoad(src, dest, shortest);
                    cout << GREEN << BOLD << "[SUCCESS] Road segment has been taken offline." << RESET << endl;
                }
                else
                {
                    cout << CYAN << "[ABORTED] Blocking sequence cancelled." << RESET << endl;
                }
            }
            pauseScreen();
            break;
        }

            // case 4:
            // { // Unblock Road
            //        clearScreen();
            //     src = Validation::inputString(20, "Source City: ");
            //     dest = Validation::inputString(20, "Destination City: ");

            //     // 1. Show all roads (Report)
            //     routeSystem.showAllRoadsBetween(src, dest);

            //     // 2. Check if there is actually anything to unblock
            //     if (!routeSystem.hasBlockedRoads(src, dest))
            //     {
            //         cout << YELLOW << "\n[!] No roads are currently blocked between these cities." << RESET << endl;
            //     }
            //     else
            //     {
            //         // 3. Only ask if there is a blocked road
            //         char confirm;
            //         cout << "\nUnblock the recently blocked road? (y/n): ";
            //         cin >> confirm;

            //         if (confirm == 'y' || confirm == 'Y')
            //         {
            //             routeSystem.unblockRecentRoad(src, dest);
            //         }
            //     }
            //     pauseScreen();
            //     break;
            // }
        case 4: // Unblock Road
        {
            clearScreen();
            cout << MAGENTA << BOLD << "============================================" << endl;
            cout << "       RESTORATION PROTOCOL: BLOCKED ROADS  " << endl;
            cout << "============================================" << RESET << endl;

            // 1. Check if ANY road is blocked in the entire system
            bool anyBlocked = false;
            for (int i = 0; i < routeSystem.cityCount; i++)
            {
                Node *head = routeSystem.h->search(routeSystem.cities[i]);
                if (head)
                {
                    Node *n = head->next;
                    while (n)
                    {
                        if (n->dist != -1 && !n->status)
                        {
                            anyBlocked = true;
                            break;
                        }
                        n = n->next;
                    }
                }
                if (anyBlocked)
                    break;
            }

            // 2. If no blocked roads exist, show message and exit
            if (!anyBlocked)
            {
                cout << GREEN << BOLD << "\n ALL SYSTEMS CLEAR: No roads are currently blocked." << RESET << endl;
                cout << WHITE << "\nPress Enter to return to the menu..." << RESET;
                cin.ignore();
                cin.get();
                break; // Goes back to the main menu without asking for source/dest
            }

            // 3. If roads ARE blocked, show the list and ask for input
            routeSystem.displayBlockedRoads();

            cout << WHITE << "\nEnter the cities to re-connect:" << RESET << endl;
            src = Validation::inputString(20, "Source City or press 0 to go back    ");
            if (src == "CANCEL_INPUT")
                break;
            dest = Validation::inputString(20, "Destination City: ");

            routeSystem.unblockRecentRoad(src, dest);

            pauseScreen();
            break;
        }

        // case 5:
        // { // Show Shortest Path
        //        clearScreen();
        //     src = Validation::inputString(20, "Source City: ");
        //     dest = Validation::inputString(20, "Destination City: ");
        //     int d = routeSystem.shortPath(src, dest);
        //     if (d == -1)
        //         cout << RED << "NO ROAD IS AVAILABLE (PENDING STATUS)" << RESET << endl;
        //     else
        //         cout << GREEN << "Shortest Available Path: " << d << " km" << RESET << endl;
        //     pauseScreen();
        //     break;
        // }
        case 5: // Show Shortest Path
        {
            clearScreen();
            cout << MAGENTA << BOLD << "============================================" << endl;
            cout << "        ROUTING ENGINE: SHORTEST PATH       " << endl;
            cout << "============================================" << RESET << endl;

            // 1. Guard: Check if enough cities exist
            if (routeSystem.cityCount < 2)
            {
                cout << YELLOW << BOLD << "\n ERROR: Insufficient data. Need at least 2 cities." << RESET << endl;
                cout << WHITE << "Press Enter to return to menu..." << RESET;
                cin.ignore();
                cin.get();
                break;
            }

            // 2. Display all registered cities
            routeSystem.displayAllCities();

            // 3. Input with validation
            src = Validation::inputString(20, "\nEnter Starting City or press 0 to go back ");
            if (src == "CANCEL_INPUT")
                break;
            if (routeSystem.getCityIndex(src) == -1)
            {
                cout << RED << "[!] ERROR: City not found." << RESET << endl;
                pauseScreen();
                break;
            }

            dest = Validation::inputString(20, "Enter Destination City: ");
            if (routeSystem.getCityIndex(dest) == -1)
            {
                cout << RED << "[!] ERROR: City not found." << RESET << endl;
                pauseScreen();
                break;
            }

            // 4. Calculate Path
            int d = routeSystem.shortPath(src, dest);

            if (d == -1)
            {
                cout << RED << BOLD << "\n[!] NO PATH EXISTS: These cities are not connected by any active road." << RESET << endl;
            }
            else
            {
                cout << GREEN << BOLD << "\n[✓] OPTIMAL ROUTE FOUND: " << RESET
                     << WHITE << d << " km" << RESET << endl;
            }

            pauseScreen();
            break;
        }

        // case 6: // Delete City
        //    clearScreen();
        //         src = Validation::inputString(20, "City to Delete: ");
        //         routeSystem.deleteCity(src);
        //         pauseScreen();
        //         break;
        case 6:
        { // Delete City
            clearScreen();
            cout << MAGENTA << BOLD << "========================================" << endl;
            cout << "           CITY DELETION TOOL           " << endl;
            cout << "========================================" << RESET << endl;

            // 1. Check if cities exist and display them first
            if (routeSystem.cityCount == 0)
            {
                cout << YELLOW << BOLD << "\n[!] No cities are currently registered." << RESET << endl;
                cout << WHITE << "Press Enter to return to the menu..." << RESET;
                cin.ignore();
                cin.get();
                break; // Exit the case and go back to menu
            }

            // 2. If cities exist, show them so the user can see what to type
            routeSystem.displayAllCities();

            // 3. Now ask for the city name
            cout << "\n"
                 << WHITE << "Enter the name of the city to delete  or press 0 to go back " << RESET;
            string cityToDelete = Validation::inputString(20, "");
            if (cityToDelete == "CANCEL_INPUT")
                break;

            // 4. Call the actual deletion logic
            routeSystem.deleteCity(cityToDelete);

            pauseScreen();
            break;
        }

        case 7: // Show Full Logistics Network
            clearScreen();
            routeSystem.displayAllPaths();
            pauseScreen();
            break;
        }
    }
}