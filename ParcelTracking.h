#ifndef PARCEL_TRACKING_H
#define PARCEL_TRACKING_H

#include <iostream>
#include <string>
#include "Parcel.h"  

using namespace std;

// FORWARD DECLARATIONS (Add these before class definitions)
class Sorting;         // Forward declaration
class CourierSystem;   // Forward declaration

/* ================= PARCEL EVENT ================= */

class ParcelEvent {
public:
    string description;
    string timestamp;
    ParcelEvent* next;

    ParcelEvent(const string& d, const string& t)
        : description(d), timestamp(t), next(nullptr) {}
};

/* ================= UNDO STACK ================= */

class UndoStackNode {
public:
    ParcelEvent* event;
    UndoStackNode* next;

    UndoStackNode(ParcelEvent* e) : event(e), next(nullptr) {}
};

class ParcelUndoStack {
private:
    UndoStackNode* top;

public:
    ParcelUndoStack() : top(nullptr) {}
    void push(ParcelEvent* e);
    ParcelEvent* pop();
    bool isEmpty() const;
};

/* ================= HASH NODE ================= */

class ParcelTrackNode {
public:
    Parcel* parcel;              // 🔹 YOUR Parcel
    ParcelEvent* eventHead;
    ParcelUndoStack undo;
    ParcelTrackNode* next;

    ParcelTrackNode(Parcel* p)
        : parcel(p), eventHead(nullptr), next(nullptr) {}
};

/* ================= TRACKING SYSTEM ================= */

class ParcelTrackingSystem {
private:
    static const int TABLE_SIZE = 100;
    ParcelTrackNode* table[TABLE_SIZE];

    int hashFunction(int parcelId);
    ParcelTrackNode* findNode(int parcelId);

public:
    ParcelTrackingSystem();
    ~ParcelTrackingSystem();

    void addParcel(Parcel* p);
    void addEvent(int parcelId, string desc, string time);
    void undoLastEvent(int parcelId);

    void showCurrentStatus(int parcelId);
    void showParcelHistory(int parcelId);
    bool parcelExists(int parcelId) {
        return (findNode(parcelId) != nullptr);
    }
    bool cancelDelivery(int parcelId, Sorting& warehouseHeap, CourierSystem& courierSys);
};

#endif
