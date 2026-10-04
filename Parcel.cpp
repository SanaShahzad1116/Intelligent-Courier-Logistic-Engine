#include "Parcel.h"
#include <iostream>
#include<string>
#include"Colors.h"
#include<iomanip>
using namespace std;

Parcel::Parcel() : id(0), weight(0.0), destination(" "), priority(3)
{
}

Parcel::Parcel(int id, float weight, string source ,string destination, int priority)
{
    setId(id);
    setWeight(weight);
    setDestination(destination);
    setPriority(priority);
    this->source = source;
}

int Parcel::getId() const { return id; }
float Parcel::getWeight() const { return weight; }
string Parcel::getDestination() const { return destination; }
int Parcel::getPriority() const { return priority; }

void Parcel::setId(int id)
{
    if (id > 0)
        this->id = id;
}

void Parcel::setWeight(float weight)
{
    if (weight > 0)
        this->weight = weight;
}

string Parcel::getSource(){
    return source;
}

void Parcel::setDestination(string destination)
{
    if (!destination.empty())
        this->destination = destination;
}

void Parcel::setPriority(int priority)
{
this->priority=priority;

  }
string Parcel::showPriority() const {
    if (priority == 1)
        return "OverNight";
    else if (priority == 2)
        return "2 Days";
    else
        return "Normal";
}
    

// void Parcel::displayParcel() const
// {
//     cout << "[ID: " << id << " | Weight: " << weight
//          << "kg | Dest: " << destination
//          << " | Priority: " << showPriority() << "]" << endl;
// }

void Parcel::displayParcel() const
{
    // 1. Determine Color and Label based on Priority
    string priorityColor;
    string priorityLabel = showPriority();

    if (priorityLabel == "High" || priorityLabel == "1") {
        priorityColor = RED ;
    } else if (priorityLabel == "Medium" || priorityLabel == "2") {
        priorityColor = YELLOW;
    } else {
        priorityColor = GREEN ;
    }

    // 2. Modern Card UI
    cout << CYAN << "┌──────────────────────────────────────────────────┐" << RESET << endl;
    
    // Row 1: ID and Priority Badge
    cout << CYAN << "│ " << WHITE << BOLD << "ID: " << left << setw(15) << id 
         << RESET << CYAN << "│ " << "Priority: " << priorityColor << setw(12) << priorityLabel << RESET << CYAN << " │" << endl;
    
    cout << CYAN << "├────────────────────────┬─────────────────────────┤" << RESET << endl;
    
    // Row 2: Weight and Destination
    cout << CYAN << "│ " << WHITE << "Weight: " << RESET << left << setw(12) << (to_string(weight) + " kg")
         << CYAN << "│ " << WHITE << "Dest: " << RESET << left << setw(16) << destination << CYAN << " │" << endl;
    
    cout << CYAN << "└────────────────────────┴─────────────────────────┘" << RESET << endl;
}