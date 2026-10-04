#include "ParcelTracking.h"
#include "Colors.h"
#include "CourierSystem.h"
#include "SortingModule.h"
#include "UIhelper.h"


/* ================= UNDO STACK ================= */

void ParcelUndoStack::push(ParcelEvent* e) {
    UndoStackNode* n = new UndoStackNode(e);
    n->next = top;
    top = n;
}

ParcelEvent* ParcelUndoStack::pop() {
    if (!top) return nullptr;
    UndoStackNode* t = top;
    ParcelEvent* e = t->event;
    top = t->next;
    delete t;
    return e;
}

bool ParcelUndoStack::isEmpty() const {
    return top == nullptr;
}

/* ================= TRACKING SYSTEM ================= */

ParcelTrackingSystem::ParcelTrackingSystem() {
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = nullptr;
}

ParcelTrackingSystem::~ParcelTrackingSystem() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ParcelTrackNode* curr = table[i];
        while (curr) {
            ParcelTrackNode* temp = curr;
            curr = curr->next;

            ParcelEvent* e = temp->eventHead;
            while (e) {
                ParcelEvent* et = e;
                e = e->next;
                delete et;
            }
            delete temp;
        }
    }
}

int ParcelTrackingSystem::hashFunction(int parcelId) {
    return parcelId % TABLE_SIZE;
}

ParcelTrackNode* ParcelTrackingSystem::findNode(int parcelId) {
    int idx = hashFunction(parcelId);
    ParcelTrackNode* curr = table[idx];
    while (curr) {
        if (curr->parcel->getId() == parcelId)
            return curr;
        curr = curr->next;
    }
    return nullptr;
}

/* ================= PUBLIC FUNCTIONS ================= */

void ParcelTrackingSystem::addParcel(Parcel* p) {
    int idx = hashFunction(p->getId());
    ParcelTrackNode* node = new ParcelTrackNode(p);
    node->next = table[idx];
    table[idx] = node;
}

void ParcelTrackingSystem::addEvent(int parcelId, string desc, string time) {
    ParcelTrackNode* node = findNode(parcelId);
    if (!node) {
        cout << "Parcel not found!\n";
        return;
    }

    ParcelEvent* e = new ParcelEvent(desc, time);

    if (!node->eventHead)
        node->eventHead = e;
    else {
        ParcelEvent* c = node->eventHead;
        while (c->next) c = c->next;
        c->next = e;
    }

    node->undo.push(e);
}

void ParcelTrackingSystem::undoLastEvent(int parcelId) {
    ParcelTrackNode* node = findNode(parcelId);
    if (!node || node->undo.isEmpty()) {
        cout << "Nothing to undo.\n";
        return;
    }

    ParcelEvent* last = node->undo.pop();

    if (node->eventHead == last)
        node->eventHead = nullptr;
    else {
        ParcelEvent* c = node->eventHead;
        while (c->next != last) c = c->next;
        c->next = nullptr;
    }

    delete last;
}

void ParcelTrackingSystem::showCurrentStatus(int parcelId) {
    ParcelTrackNode* node = findNode(parcelId);
    if (!node || !node->eventHead) {
        cout << "No status available.\n";
        return;
    }

    ParcelEvent* c = node->eventHead;
    while (c->next) c = c->next;

    cout << "Current Status: " << c->description
         << " at " << c->timestamp << endl;
}
void ParcelTrackingSystem::showParcelHistory(int parcelId) {
    ParcelTrackNode* node = findNode(parcelId);
    
    if (!node) {
        cout << RED << "\n ERROR: Parcel ID " << parcelId << " not found!\n" << RESET;
        return;
    }

    ParcelEvent* c = node->eventHead;
    
    if (!c) {
        cout << YELLOW << "\n No tracking history available for Parcel ID " << parcelId << "\n" << RESET;
        return;
    }

    cout << CYAN << "\n════════════════════════════════════════════\n";
    cout << "          TIMELINE - PARCEL ID: " << parcelId << "\n";
    cout << "════════════════════════════════════════════\n" << RESET;
    
    int i = 1;
    while (c) {
        // Simple numbering (no icons)
        cout << i++ << ". " << c->description << "\n";
        cout << "   └─  " << MAGENTA << c->timestamp << RESET << "\n\n";
        c = c->next;
    }
    
    cout << CYAN << "────────────────────────────────────────────\n" << RESET;
    cout << "Total events: " << GREEN << (i-1) << RESET << endl;
}

// void ParcelTrackingSystem::showAllTrackedParcels() {
//     for (int i = 0; i < TABLE_SIZE; i++) {
//         ParcelTrackNode* c = table[i];
//         while (c) {
//             c->parcel->displayParcel();
//             c = c->next;
//         }
//     }
// }


bool ParcelTrackingSystem::cancelDelivery(int parcelId, Sorting& warehouseHeap, CourierSystem& courierSys) {
    string strId = to_string(parcelId);
    
    // 1. CHECK IF PARCEL IS IN WAREHOUSE
    if (warehouseHeap.idExists(parcelId)) {
        cout << YELLOW << "\n Parcel is in WAREHOUSE\n" << RESET;
        cout << "   • Status: Not yet dispatched\n";
        cout << "   • Action: Cancel delivery not needed. Parcel is still in warehouse.\n";
        return false;
    }
    
    // 2. CHECK IF PARCEL IS IN TRANSIT
    else if (courierSys.isParcelInTransit(strId)) {
        cout << YELLOW << "\n Parcel is IN TRANSIT\n" << RESET;
        
        // Get parcel info from transit
        CourierParcel* transitParcel = courierSys.getTransitParcelInfo(strId);
        
        if (transitParcel) {
            // Free the rider using CourierSystem function
            courierSys.freeRider(transitParcel->riderID);
            
            // Create Parcel object from transit parcel data
            Parcel returnParcel(
                stoi(transitParcel->id),
                transitParcel->weight,
                transitParcel->source,
                transitParcel->dest,
                transitParcel->priority
            );
            
            // Add back to warehouse
            warehouseHeap.insert(returnParcel);
            
            // Remove from transit
            bool removed = courierSys.removeFromTransit(strId);
            
            if (removed) {
                // Add event to timeline
                string cancelTime = getCurrentTime();
                addEvent(parcelId, "Delivery CANCELLED by user - Returned to warehouse", cancelTime);
                
                // Get rider for display
                Rider* rider = courierSys.findRider(transitParcel->riderID);
                
                cout << GREEN << "\n[SUCCESS] Delivery cancelled!\n" << RESET;
                cout << "   • Parcel returned to warehouse\n";
                if (rider) {
                    cout << "   • Rider " << rider->id << " (" << rider->name << ") marked as AVAILABLE\n";
                }
                cout << "   • Event recorded in timeline\n";
                cout << "   • Time: " << cancelTime << endl;
                return true;
            }
        }
        return false;
    }
    
    // 3. CHECK IF PARCEL IS DELIVERED
    else if (courierSys.isParcelDelivered(strId)) {
        cout << RED << "\n Parcel is already DELIVERED\n" << RESET;
        cout << "   • Status: Successfully delivered\n";
        cout << "   • Action: Cannot cancel delivery after parcel is delivered\n";
        cout << "   • Sorry, this action cannot be performed\n";
        return false;
    }
    
    // 4. PARCEL NOT FOUND
    else {
        cout << RED << "\n PARCEL NOT FOUND IN SYSTEM!\n" << RESET;
        cout << "   • Please check Parcel ID and try again\n";
        return false;
    }
}