#include"SortingModule.h"
#include<iostream>
#include<algorithm>
using namespace std;

Sorting::Sorting(){
size=0;
arr= new Parcel[10]; //default
total_size=10;
}

Sorting::Sorting(int heapsize){
    this->size=0;
      this->arr=new Parcel[heapsize];
      this->total_size=heapsize;
}

Sorting::~Sorting() {
    if (arr != nullptr) {
        delete[] arr;
    }
}


Parcel Sorting::getTop() {
    if (size > 0) return arr[0];
    
    // Warning: Ensure your Parcel struct has a default constructor: Parcel() {}
    return Parcel(); 
}
bool Sorting::isEmpty() const {
    return size == 0;
}

void Sorting::grow() {
    int newCapacity = total_size * 2;
    Parcel* newArr = new Parcel[newCapacity];
    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }
    delete[] arr;
    arr = newArr;
    total_size = newCapacity;
}


// bool Sorting::isEmpty() const {
//     return size == 0;
// }

Parcel Sorting::extractMax() {
    if (size == 0) {
        cout << "No element to extract" << endl;
        return Parcel(); // default parcel
    }

    Parcel top = arr[0];
    arr[0] = arr[size - 1];
    size--;

    if (size > 0) {
        heapifyDown(0);
    }

    return top;
}

void Sorting::heapifyUp(int index){

  while(index>0 && arr[(index-1)/2].getPriority()>arr[index].getPriority()){
    swap(arr[(index-1)/2],arr[index]);
    index=(index-1)/2;
  }
  
}

void Sorting::heapifyDown(int index) {
    while (true) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < size && arr[left].getPriority() <arr[largest].getPriority()) {
            largest = left;
        }
        if (right < size && arr[right].getPriority() < arr[largest].getPriority()) {
            largest = right;
        }
        if (largest == index) {
            break;
        }
        swap(arr[index], arr[largest]);
        index = largest;
    }
}


void Sorting::Delete(){
    if(size==0){
      cout<<"Heap is empty "<<endl;
      return ;
    }
    arr[0]=arr[size-1];
    size--;
    if(size!=0){
      heapifyDown(0);
    }
}

void Sorting::insert(const Parcel& newParcel){
     if(size==total_size){
       //we will resize it before it fulls
       grow();
     }

     arr[size]=newParcel;
     heapifyUp(size);
     size++;
}
void Sorting::displayHeap() const {
    if (size == 0) {
        cout << "Heap is empty!" << endl;
        return;
    }

    cout << "Current Parcels in Heap:\n";
    for (int i = 0; i < size; i++) {
        arr[i].displayParcel(); // use your Parcel display function
    }
}



bool Sorting::idExists(int id) const {
    for (int i = 0; i < size; i++) {
        if (arr[i].getId() == id) {
            return true;
        }
    }
    
    return false;
}




