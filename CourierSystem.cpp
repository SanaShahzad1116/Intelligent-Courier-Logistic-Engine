#include "CourierSystem.h"
#include"Validator.h"
#include"routing.h"
#include"Colors.h"
#include"iomanip"
#include "UIHelper.h"
#include<string>
/* ================= QUEUE ================= */

CourierParcelQueue::CourierParcelQueue() { front = rear = nullptr; }

void CourierParcelQueue::enqueue(CourierParcel* p) {
    QNode* n = new QNode(p);
    if (!rear) front = rear = n;
    else { rear->next = n; rear = n; }
}

CourierParcel* CourierParcelQueue::dequeue() {
    if (!front) return nullptr;
    QNode* t = front;
    CourierParcel* p = t->p;
    front = front->next;
    if (!front) rear = nullptr;
    delete t;
    return p;
}

CourierParcel* CourierParcelQueue::search(string id) {
    QNode* c = front;
    while (c) {
        if (c->p->id == id) return c->p;
        c = c->next;
    }
    return nullptr;
}

bool CourierParcelQueue::isEmpty() { return front == nullptr; }

void CourierParcelQueue::display(string name) {
    if (isEmpty()) {
        cout << YELLOW << "\n[!] " << name << " is currently empty." << RESET << endl;
        return;
    }

    cout << CYAN << BOLD << "\n============================================================" << endl;
    cout << "                " << name << " STATUS REPORT" << endl;
    cout << "============================================================" << RESET << endl;
    
    // Table Header - Using only existing struct members
    cout << YELLOW << BOLD 
         << left << setw(10) << "ID" 
         << setw(12) << "Rider ID"
         << setw(15) << "Destination" 
         << setw(12) << "Status" << RESET << endl;
    cout << CYAN << "------------------------------------------------------------" << RESET << endl;

    QNode* c = front;
    while (c) {
        cout << WHITE << left 
             << setw(10) << c->p->id 
             << setw(12) << (c->p->riderID == "" ? "Pending" : c->p->riderID)
             << setw(15) << c->p->dest 
             << GREEN << setw(12) << c->p->status << RESET << endl;
        
        c = c->next;
    }
    cout << CYAN << "------------------------------------------------------------\n" << RESET << endl;
}

/* ================= STACK ================= */

CourierUndoStack::CourierUndoStack() { top = nullptr; }

void CourierUndoStack::push(string o, string p, string r, int l) {
    SNode* n = new SNode{ o,p,r,l,top };
    top = n;
}

SNode* CourierUndoStack::pop() {
    if (!top) return nullptr;
    SNode* t = top;
    top = top->next;
    return t;
}

bool CourierUndoStack::isEmpty() { return top == nullptr; }

/* ================= HASH ================= */

template<typename T>
HashTable<T>::HashTable() {
    for (int i = 0; i < 100; i++) table[i] = nullptr;
}

template<typename T>
int HashTable<T>::hash(string k) {
    int h = 0;
    for (char c : k) h = (h * 31 + c) % 100;
    return h;
}

template<typename T>
void HashTable<T>::insert(string k, T* v) {
    int i = hash(k);
    Node* n = new Node{ k,v,table[i] };
    table[i] = n;
}

template<typename T>
T* HashTable<T>::search(string k) {
    int i = hash(k);
    Node* c = table[i];
    while (c) { if (c->key == k) return c->value; c = c->next; }
    return nullptr;
}

/* ================= HEAP ================= */

CourierRiderHeap::CourierRiderHeap() { size = 0; }

void CourierRiderHeap::insert(Rider* r) {
    arr[size] = r;
    heapifyUp(size++);
}

void CourierRiderHeap::heapifyUp(int i) {
    while (i && arr[(i - 1) / 2]->load > arr[i]->load) {
        swap(arr[i], arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Rider* CourierRiderHeap::getMin() { return size ? arr[0] : nullptr; }

void CourierRiderHeap::update(Rider* r) {
    for (int i = 0; i < size; i++)
        if (arr[i] == r) { heapifyUp(i); heapifyDown(i); }
}

void CourierRiderHeap::heapifyDown(int i) {
    int l = 2 * i + 1, r = 2 * i + 2, s = i;
    if (l < size && arr[l]->load < arr[s]->load) s = l;
    if (r < size && arr[r]->load < arr[s]->load) s = r;
    if (s != i) { swap(arr[i], arr[s]); heapifyDown(s); }
}

/* ================= SYSTEM ================= */

CourierSystem::CourierSystem() {}
//CourierSystem::CourierSystem(ParcelTrackingSystem* tracker) 
  //  : trackerRef(tracker) {} 
    
void CourierSystem::addRider() {
    // clearScreen();
    displayAllRiders(); // Show current list at the top for reference

    string id, n, l; 

    cout << MAGENTA << BOLD << "\n========================================" << endl;
    cout << "          REGISTER NEW RIDER            " << endl;
    cout << "========================================" << RESET << endl;

    // 1. UNIQUE ID VALIDATION (Using Integer for input, String for storage)
    while (true) {
        // Validation::inputInteger ensures user types a number
        int tempID = Validation::inputIntegerUpto(4, "Enter Rider ID (Numbers only): ");
        if (tempID == -1)
        {
            return;
        }
        
        // Convert to string to check your HashTable and match your Struct
        id = to_string(tempID);

        if (riders.search(id) == nullptr) {
            break; // Unique ID found
        }
        cout << RED << "[!] Error: ID " << id << " is already taken. See list above." << RESET << endl;
    }

    // 2. DATA INPUT
    // We get the name as a string
    n = Validation::inputString(25, "Enter Rider Name: ");

    // For License, we use inputInteger again to force numeric entry
    int tempLicense = Validation::inputInteger(3, "Enter License Number (Numbers only): ");
    l = to_string(tempLicense);

    // 3. CREATE RIDER OBJECT
    // Matches your struct: Rider(string id, string name, string license, int capacity)
    Rider* r = new Rider(id, n, l, 1); 

    // IMPORTANT: Set initial state
    r->load = 0;        // 0 means Available - keeps them at top of Min-Heap
    r->available = true;

    // 4. STORAGE
    riders.insert(id, r); // Store in Hash Table
    heap.insert(r);       // Store in Min-Heap

    cout << GREEN << BOLD << "\n[SUCCESS] Rider '" << n << "' (ID: " << id << ") is now AVAILABLE." << RESET << endl;
}

// void CourierSystem::addParcel() {
//     string id, d; int p; float w;
//     cout << "ID Dest Priority Weight: ";
//     cin >> id >> d >> p >> w;
//     CourierParcel* pa = new CourierParcel(id, d, p, w);
//     parcels.insert(id, pa);
//     pickupQ.enqueue(pa);
// }

void CourierSystem::addParcel() {
    routing routeSystem;
    routeSystem.displayAllCities();

    int id, p;
    float w;
    string source, dest;

    cout << MAGENTA << BOLD << "\n========================================" << endl;
    cout << "          REGISTER NEW PARCEL           " << endl;
    cout << "========================================" << RESET << endl;

    // 1. Using your Validation class for inputs
    id = Validation::inputInteger(1, "Enter Parcel ID (Numbers only): ");

    // Check for duplicate IDs in the HashTable
    if (parcels.search(to_string(id)) != nullptr) {
        cout << RED << "[!] Error: Parcel ID " << id << " already exists." << RESET << endl;
        return;
    }

    // 2. Capture all required data including Source
    source = Validation::inputString(20, "Enter Source City: ");
    dest = Validation::inputString(20, "Enter Destination City: ");
    w = Validation::inputFloat(1, "Enter Weight (kg): ");
    
    cout << "Select Priority (1: Overnight, 2: 2-Day, 3: Normal)" << endl;
    p = Validation::inputInteger(1, "Priority: ");

    // 3. Create the Parcel object
    // Note: If you cannot pass parcelHeap here, this object must be 
    // handled by the calling function or stored in a class-level heap.
    Parcel newParcel(id, w, source, dest, p);

    // 4. Update the CourierSystem's internal tracking
    // We create a CourierParcel with the source to fix the "Road Blocked" issue
    CourierParcel* cp = new CourierParcel(to_string(id), dest, source, p, w);
    parcels.insert(to_string(id), cp);

    // 5. Add to the local pickup queue as a fallback
    pickupQ.enqueue(cp);

    cout << GREEN << BOLD << "\n[SUCCESS] Parcel " << id << " added successfully from " << source << "." << RESET << endl;
}

// void CourierSystem::assignParcel() {
//     CourierParcel* p = pickupQ.dequeue();
//     if (!p) { cout << "No parcel\n"; return; }

//     Rider* r = heap.getMin();
//     if (!r) { cout << "No rider\n"; return; }

//     undo.push("ASSIGN", p->id, r->id, r->load);

//     p->status = "Transit";
//     p->riderID = r->id;
//     r->load += p->weight;

//     heap.update(r);
//     transitQ.enqueue(p);

//     cout << "Assigned " << p->id << " -> " << r->name << endl;
// }

void CourierSystem::assignParcel(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker) {
    // 1. Check if the Parcel Heap (Sorting) has anything
    if (parcelHeap.isEmpty()) {
        cout << YELLOW << "\n[!] No parcels available in the sorting heap." << RESET << endl;
        return;
    }

    // 2. Peek at the top parcel using getTop()
    Parcel p = parcelHeap.getTop(); 

    // 3. Check Rider Heap size
    if (heap.size == 0) {
        cout << RED << "\n[!] No riders registered in the system." << RESET << endl;
        return;
    }
    
    Rider* r = heap.getMin(); 

    // 4. Validate Rider Availability (Load 0 means available)
    if (r->load != 0) {
        cout << RED << "\n[!] All riders are currently BUSY (Status: Busy)." << RESET << endl;
        return;
    }

    // 5. Display the Match to the Admin
    cout << CYAN << BOLD << "\n--- INTELLIGENT DISPATCH SUGGESTION ---" << RESET << endl;
    // Use Getters here because members are private in Parcel class
    cout << "PARCEL ID: " << p.getId() << " | Priority: " << p.showPriority() << endl;
    cout << "ROUTE:     " << p.getSource() << " -> TO: " << p.getDestination() << endl;
    cout << "RIDER:     " << r->name << " (ID: " << r->id << ")" << endl;
    cout << "---------------------------------------" << endl;

    // 6. Calculate Route using shortPath
    cout << YELLOW << "Calculating Shortest Route..." << RESET << endl;
    int dist = routeSystem.shortPath(p.getSource(), p.getDestination());
    
    if (dist == -1) {
        cout << RED << "[!] Error: No valid road exists between these cities!" << RESET << endl;
        return;
    }
    cout << GREEN << "Shortest Distance: " << dist << " km" << RESET << endl;

    // 7. Admin Confirmation
    string choice;
    // cout << WHITE << "\nConfirm assignment and start delivery? (y/n): ";
    choice=Validation::inputString(1,"\nConfirm assignment and start delivery? (y/n):");

    if (choice == "y" || choice == "Y") {
        // A. Remove parcel from your Sorting Heap (Warehouse)
        parcelHeap.extractMax(); 

        // B. Update Rider Status
        r->load = 1;         // Mark as Busy
        r->available = false;
        heap.update(r);      // Re-sort rider heap

        // C. Convert Parcel to CourierParcel for the Transit Queue
        // CourierParcel(id, dest, priority, weight)
        // CourierParcel* cp = new CourierParcel(
        //     to_string(p.getId()), 
        //     p.getDestination(), 
        //     p.getPriority(), 
        //     p.getWeight()
        // );

        // Conversion logic within assignParcel or addParcel
CourierParcel* cp = new CourierParcel(
    to_string(p.getId()),   // Parcel ID to String
    p.getDestination(),     // Destination city
    p.getSource(),          // Added: Source city to prevent "Road Blocked" errors
    p.getPriority(),        // Priority level
    p.getWeight()           // Parcel weight
);
        cp->riderID = r->id;
        cp->status = "In Transit";

        // D. Move to Transit Queue (FIFO)
        transitQ.enqueue(cp);
        string dispatchTime = getCurrentTime();
        tracker.addEvent(p.getId(), "Dispatched from " + p.getSource() + " to " + p.getDestination(), dispatchTime);

        cout << GREEN << BOLD << "\n[SUCCESS] Parcel " << p.getId() << " is now IN TRANSIT with " << r->name << " at " << dispatchTime << "."<< RESET << endl;
    } else {
        cout << YELLOW << "[!] Dispatch cancelled." << RESET << endl;
    }
}



void CourierSystem::deliveryAttempt() {
    string id; cout << "Parcel ID: "; cin >> id;
    CourierParcel* p = transitQ.search(id);
    if (!p) return;

    Rider* r = riders.search(p->riderID);
    undo.push("DELIVER", p->id, r->id, r->load);

    r->load -= p->weight;
    p->status = "Delivered";
    heap.update(r);
}

void CourierSystem::undoLast() {
    SNode* s = undo.pop();
    if (!s) return;

    CourierParcel* p = parcels.search(s->pid);
    Rider* r = riders.search(s->rid);

    p->status = "Pending";
    r->load = s->prevLoad;
    heap.update(r);

    pickupQ.enqueue(p);
}

void CourierSystem::reportMissing() {
    string id; cin >> id;
    CourierParcel* p = parcels.search(id);
    if (p) p->status = "Missing";
}

void CourierSystem::display() {
    pickupQ.display("PICKUP");
    transitQ.display("TRANSIT");
}


void CourierSystem::viewTransitQueue(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker) {
    if (transitQ.isEmpty()) {
        cout << YELLOW << "\n[!] No parcels are currently on the road." << RESET << endl;
        return;
    }
    
    // Displays the queue using the corrected display (without .source)
    transitQ.display("CURRENT TRANSIT (FIFO)"); 
    
    string choice;
    // cout << "\nWould you like to mark a parcel as DELIVERED? (y/n): ";
   choice=Validation::inputString(1,"\nWould you like to mark a parcel as DELIVERED? (y/n):  ");
    
    if (choice == "y" || choice == "Y") {
        // Pass the objects to the next function to check for "Lost" status
        completeDelivery(parcelHeap, routeSystem, tracker); 
    }
}

void CourierSystem::completeDelivery(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker) {
    if (transitQ.isEmpty()) {
        cout << YELLOW << "\n[!] No parcels are currently in transit." << RESET << endl;
        return;
    }

    // 1. Get the oldest parcel from the FIFO Transit Queue
    CourierParcel* p = transitQ.dequeue(); 
    if (!p) return;

    // 2. LOGICAL CHECK: Is the destination still reachable?
    // We use shortPath to check if the road is currently available or blocked
    int pathExists = routeSystem.shortPath(p->source, p->dest);

    if (pathExists == -1) {
        // --- PARCEL IS LOST DURING TRANSIT ---
        cout << RED << BOLD << "\n[!!!] DELIVERY FAILED: Road to " << p->dest << " is BLOCKED!" << RESET << endl;
        cout << RED << "Parcel " << p->id << " is considered LOST in transit." << RESET << endl;

        // A. Free the Rider so they can return to the warehouse
        Rider* r = riders.search(p->riderID);
        if (r) {
            r->load = 0;
            r->available = true;
            heap.update(r); // Re-heapify rider to make them available again
        }

        // B. Return Parcel to Warehouse (Sorting Heap)
        // We re-insert it so it waits for its turn based on PRIORITY again
        Parcel returnP(stoi(p->id), p->weight, p->source, p->dest, p->priority);
        parcelHeap.insert(returnP);

        cout << YELLOW << "Action: Parcel " << p->id << " has been returned to the Warehouse Heap." << RESET << endl;
        delete p; // Clean up the transit pointer
        return;
    }

    // 3. SUCCESSFUL DELIVERY
    p->status = "Delivered";
    deliveredHistory.enqueue(p);

    
    string deliveryTime = getCurrentTime();
    tracker.addEvent(stoi(p->id), "Delivered successfully to " + p->dest, deliveryTime);
    
    // Free the Rider
    Rider* r = riders.search(p->riderID);
    if (r) {
        r->load = 0;
        r->available = true;
        heap.update(r);
    }

    cout << GREEN << BOLD << "\n[SUCCESS] Parcel " << p->id << " delivered successfully to " << p->dest << "!" << RESET << endl;
}

void CourierSystem::viewDeliveredHistory() {

   

    if (deliveredHistory.isEmpty()) {
        cout << YELLOW << "\n[!] No parcels have been delivered yet." << RESET << endl;
        return;
    }
    deliveredHistory.display("COMPLETED DELIVERIES"); // Show everything
}


void CourierSystem::displayAllRiders() {
    cout<<"displayRider function called"<<endl;
    cout << CYAN << BOLD << "\n==============================================" << endl;
    cout << "           CURRENT REGISTERED RIDERS          " << endl;
    cout << "==============================================" << RESET << endl;

    if (heap.size == 0) {
        cout << YELLOW << "         No riders found in system.           " << RESET << endl;
    } else {
        // Table Header
        cout << YELLOW << BOLD 
             << left << setw(10) << "ID" 
             << setw(20) << "Name" 
             << setw(15) << "Status" << RESET << endl;
        cout << CYAN << "----------------------------------------------" << RESET << endl;

        // Table Rows
        for (int i = 0; i < heap.size; i++) {
            Rider* r = heap.arr[i]; 
            
            // Highlight Available in Green, Busy in Red
            string statusText = (r->load == 0) ? "Available" : "Busy";
            string statusColor = (r->load == 0) ? GREEN : RED;

            cout << WHITE 
                 << left << setw(10) << r->id 
                 << setw(20) << r->name 
                 << statusColor << setw(15) << statusText << RESET << endl;
        }
    }
    cout << CYAN << "----------------------------------------------\n" << RESET << endl;
}

bool CourierSystem::isParcelInTransit(string parcelId) {
    return (transitQ.search(parcelId) != nullptr);
}

bool CourierSystem::isParcelDelivered(string parcelId) {
    return (deliveredHistory.search(parcelId) != nullptr);
}

CourierParcel* CourierSystem::getTransitParcelInfo(string parcelId) {
    return transitQ.search(parcelId);
}



// Inside your CourierSystem class in CourierSystem.cpp
// void CourierSystem::displayAllRiders() {
//     cout << "\n--- Current Registered Riders ---" << endl;
    
//     // Example: assuming you have a collection of Rider objects
//     // You should use the actual logic used in your displayAllRiders() function
//     // but here is a simple structure:
    
//     // Check if rider data exists
//     if (heap.size == 0) {
//         cout << "No riders registered." << endl;
//         return;
//     }

//     cout << left << setw(10) << "ID" << setw(20) << "Name" << "Status" << endl;
//     cout << "--------------------------------------------" << endl;

//     for (int i = 0; i < heap.size; i++) {
//         Rider* r = heap.arr[i];
//         string status = (r->load == 0) ? "Available" : "Busy";
//         cout << left << setw(10) << r->id << setw(20) << r->name << status << endl;
//     }
// }

Rider* CourierSystem::findRider(string riderId) {
    return riders.search(riderId);
}

void CourierSystem::freeRider(string riderId) {
    Rider* r = riders.search(riderId);
    if (r) {
        r->load = 0;
        r->available = true;
        heap.update(r);
    }
}

bool CourierSystem::removeFromTransit(string parcelId) {
    // Use existing transitQ.search() to find parcel
    CourierParcel* p = transitQ.search(parcelId);
    if (!p) return false;
    
    // Create temporary queue
    CourierParcelQueue tempQueue;
    bool found = false;
    
    // Transfer all parcels except the one to remove
    while (!transitQ.isEmpty()) {
        CourierParcel* current = transitQ.dequeue();
        if (current->id == parcelId) {
            found = true;
            delete current; // Delete the parcel
        } else {
            tempQueue.enqueue(current);
        }
    }
    
    // Restore remaining parcels
    while (!tempQueue.isEmpty()) {
        transitQ.enqueue(tempQueue.dequeue());
    }
    
    return found;
}
