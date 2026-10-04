#ifndef ROUTING_H
#define ROUTING_H

#include <iostream>
#include <string>
using namespace std;

/* ================= NODE ================= */

class Node
{
public:
    string dest;
    int dist;
    bool status;
    Node* next;

    Node(string d, int dis, bool s = true);
};

/* ================= HASH TABLE ================= */

class Hash
{
public:
    static const int size = 13;
    Node* Table[size];

void remove(string city);

    Hash();
    void insert(string key, string data, int dis);
    int hashfunction(string key);
    Node* search(string key);
    ~Hash();
};

/* ================= ROUTING SYSTEM ================= */
class routing {
public:
    static const int MAX = 50;
    string cities[MAX];
    int cityCount;
    Hash* h;

    routing();

    void AddLocation(string city);
    void AddRoads(string source, string dest, int dis);
    void blockRoad(string src, string des);
    int  shortPath(string src, string dest);
    void displayShortRoute(string src, string dest);
    void deleteCity(string city);
    void updatedistance(string src, string dest, int dis);
    
    // --- ONLY ADDING THESE ---
    void blockSpecificRoad(string src, string des, int dist); 
    void showAllRoadsBetween(string src, string dest); 

    int getShortestDistance(string src, string dest);
    void unblockRecentRoad(string src, string dest) ;

    bool hasBlockedRoads(string src, string dest);
    bool isValidCity(string city);
    void displayAllPaths();
    void displayAllCities();
    void displayConnectedCitiesShortest();
    void displayBlockedRoads();
    int getCityIndex(string city);

private:
    int getMinIndex(int dist[], bool visited[]);
};

#endif
