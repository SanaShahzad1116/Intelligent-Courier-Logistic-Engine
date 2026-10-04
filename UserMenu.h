#ifndef USERMENU_H
#define USERMENU_H

#include "ParcelTracking.h"  
#include "SortingModule.h"   
#include "CourierSystem.h"  
#include <string>
using namespace std;

class UserMenu {
private:
    ParcelTrackingSystem& pts;  
    Sorting& warehouseHeap;    
    CourierSystem& courierSys;  

public:
    
    UserMenu(ParcelTrackingSystem& trackerRef, Sorting& heapRef, CourierSystem& courierRef);
    void showUserMenu();
    void trackParcelLocation(int parcelId);
};

#endif