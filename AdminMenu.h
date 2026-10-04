#ifndef ADMINMENU_H
#define ADMINMENU_H

#include "SortingModule.h"
#include "routing.h"
#include "ParcelTracking.h"
#include "CourierSystem.h"

class AdminMenu {
private:
    Sorting& heap;
    routing& routeSystem;
    ParcelTrackingSystem& tracker;
    CourierSystem& courier;
private:
    void routingMenu();
    void courierMenu();
    bool authenticateAdmin();
    void manageRouting();

public:
    AdminMenu(
        Sorting& heapRef,
        routing& routeRef,
        ParcelTrackingSystem& trackerRef,
        CourierSystem& courierRef
    );

    void showAdminMenu();
};

#endif