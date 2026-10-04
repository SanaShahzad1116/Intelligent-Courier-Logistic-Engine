#ifndef Sorting_H
#define Sorting_H
#include"Parcel.h"

class Sorting{
    private:
    Parcel* arr;
    int size;//it shows the current elements present in th heap or array
    int total_size;//it shows the total size 
    public:
    Sorting();

    Sorting(int heapsize);
    ~Sorting();

    void insert(const Parcel& newParcel);
    void heapifyUp(int index);
    void heapifyDown(int index);
    Parcel getTop();
    void Delete();
    Parcel extractMax();
    void grow();
    void displayHeap() const;  // displays all parcels in the heap
   bool idExists(int id) const;
   bool isEmpty() const;

};

#endif