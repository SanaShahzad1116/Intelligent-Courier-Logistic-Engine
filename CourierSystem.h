#ifndef COURIER_SYSTEM_H
#define COURIER_SYSTEM_H

#include <iostream>
#include <string>
#include"routing.h"
#include"SortingModule.h"
#include "ParcelTracking.h"
using namespace std;

/* ================= DATA MODELS ================= */

struct Rider {
    string id, name, license;
    int maxCap, load;
    bool available;

    Rider(string i, string n, string l, int c)
        : id(i), name(n), license(l),
          maxCap(c), load(0), available(true) {}
};

// struct CourierParcel {
//     string id, dest, status, riderID;
//     int priority;
//     float weight;

//     CourierParcel(string i, string d, int p, float w)
//         : id(i), dest(d), priority(p),
//           weight(w), status("Pending"), riderID("") {}
// };
struct CourierParcel {
    string id, dest, source, status, riderID; // Added 'source' here
    int priority;
    float weight;

    // Updated constructor to accept source
    CourierParcel(string i, string d, string src, int p, float w)
        : id(i), dest(d), source(src), priority(p),
          weight(w), status("Pending"), riderID("") {}
};

/* ================= QUEUE ================= */

struct QNode {
    CourierParcel* p;
    QNode* next;
    QNode(CourierParcel* x) : p(x), next(nullptr) {}
};

class CourierParcelQueue {
    QNode* front;
    QNode* rear;
public:
    CourierParcelQueue();
    void enqueue(CourierParcel*);
    CourierParcel* dequeue();
    CourierParcel* search(string);
    bool isEmpty();
    void display(string);
};

/* ================= UNDO STACK ================= */

struct SNode {
    string op, pid, rid;
    int prevLoad;
    SNode* next;
};

class CourierUndoStack {
    SNode* top;
public:
    CourierUndoStack();
    void push(string, string, string, int);
    SNode* pop();
    bool isEmpty();
};

/* ================= HASH TABLE ================= */

template<typename T>
class HashTable {
    struct Node {
        string key;
        T* value;
        Node* next;
    };

    Node* table[100];
    int hash(string);

public:
    HashTable();
    void insert(string, T*);
    T* search(string);
};

/* ================= MIN HEAP ================= */

class CourierRiderHeap {
    void heapifyUp(int);
    void heapifyDown(int);
    public:
    Rider* arr[50];
    int size;
    CourierRiderHeap();
    void insert(Rider*);
    Rider* getMin();
    void update(Rider*);
};

/* ================= MAIN SYSTEM ================= */

class CourierSystem {
    HashTable<Rider> riders;
    HashTable<CourierParcel> parcels;

    // Fix: Declare each queue only once
    CourierParcelQueue pickupQ, transitQ, deliveredHistory; 
    CourierRiderHeap heap;
    CourierUndoStack undo;

    ParcelTrackingSystem* trackerRef;

public:
    CourierSystem();
    // CourierSystem(ParcelTrackingSystem* tracker);

    void addRider();
    void addParcel();
    void assignParcel(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker);
    void deliveryAttempt();
    void undoLast();
    void reportMissing();
    void displayAllRiders();
    void display();
    
    // NEW FUNCTIONS
    void viewTransitQueue(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker);     // Show parcels on the road
    void completeDelivery(Sorting& parcelHeap, routing& routeSystem, ParcelTrackingSystem& tracker);     // Process the FIFO delivery
    // void displayAllRiders();
    void viewDeliveredHistory(); // Show completed tasks
    //================
    bool isParcelInTransit(string parcelId);
    bool isParcelDelivered(string parcelId);
    CourierParcel* getTransitParcelInfo(string parcelId); // Optional: Extra info ke liye
 
    // Add these 3 functions:
    Rider* findRider(string riderId);          // Find rider by ID
    void freeRider(string riderId);            // Make rider available
    bool removeFromTransit(string parcelId);   // Remove parcel from transit
};

#endif
