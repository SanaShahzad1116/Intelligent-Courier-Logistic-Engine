#include "routing.h"
#include<iomanip>
#include"Colors.h"
#include"Validator.h"

/* ================= NODE ================= */

Node::Node(string d, int dis, bool s)
{
    dest = d;
    dist = dis;
    status = s;
    next = nullptr;
}

/* ================= HASH ================= */

Hash::Hash()
{
    for (int i = 0; i < size; i++)
        Table[i] = nullptr;
}

void Hash::remove(string city) {
    int index = hashfunction(city); // Use your existing hash function

    // If the bucket is empty, nothing to do
    if (Table[index] == nullptr) return;

    // We need to find the specific head node for this city in the bucket
    Node* current = Table[index];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->dest == city) {
            // Found the city entry in the hash table
            if (prev == nullptr) {
                // It was the first node in the bucket chain
                Table[index] = current->next;
            } else {
                // It was in the middle or end of the chain
                prev->next = current->next;
            }
            // Note: The actual linked list nodes were deleted in routing::deleteCity
            // Here we just remove the reference from the Hash Table
            return;
        }
        prev = current;
        current = current->next;
    }
}

// void Hash::insert(string key, string data, int dis)
// {
//     int index = hashfunction(key);

//     if (!Table[index])
//         Table[index] = new Node(key, -1, true);

//     Node* ptr = Table[index];

//     while (ptr)
//     {
//         if (ptr->dest == key && ptr->dist == -1)
//         {
//             Node* end = ptr;
//             while (end->next)
//                 end = end->next;

//             if (dis != -1)
//                 end->next = new Node(data, dis);

//             return;
//         }
//         ptr = ptr->next;
//     }

//     ptr = Table[index];
//     while (ptr->next)
//         ptr = ptr->next;

//     ptr->next = new Node(key, -1);
//     if (dis != -1)
//         ptr->next->next = new Node(data, dis);
// }



void Hash::insert(string key, string data, int dis)
{
    int index = hashfunction(key);
    if (!Table[index])
        Table[index] = new Node(key, -1, true);
    Node* ptr = Table[index];

    while (ptr)
    {
        if (ptr->dest == key && ptr->dist == -1)
        {
            Node* end = ptr;
            while (end->next)
                end = end->next;

            if (dis != -1)
                end->next = new Node(data, dis, true);
            return;
        }
        ptr = ptr->next;
    }
    ptr = Table[index];
    while (ptr->next)
        ptr = ptr->next;

    ptr->next = new Node(key, -1, true); 
    
    if (dis != -1)
        ptr->next->next = new Node(data, dis, true); 
}

int Hash::hashfunction(string key)
{
    int k = 0, i = 0;
    for (char x : key)
        k += ++i * int(x);

    return k % size;
}

Node* Hash::search(string key)
{
    int index = hashfunction(key);
    Node* ptr = Table[index];

    while (ptr)
    {
        if (ptr->dest == key && ptr->dist == -1)
            return ptr;
        ptr = ptr->next;
    }
    return nullptr;
}

Hash::~Hash()
{
    for (int i = 0; i < size; i++)
    {
        Node* ptr = Table[i];
        while (ptr)
        {
            Node* temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
        Table[i] = nullptr;
    }
}

/* ================= ROUTING ================= */

routing::routing()
{
    cityCount = 0;
    h = new Hash();
}

// void routing::AddLocation(string city)
// {
//     if (getCityIndex(city) == -1)
//         cities[cityCount++] = city;

//     h->insert(city, city, -1);
// }

void routing::AddRoads(string source, string dest, int dis)
{
    // Fix: Register cities in the array so getCityIndex works
    if (getCityIndex(source) == -1) AddLocation(source);
    if (getCityIndex(dest) == -1) AddLocation(dest);

    h->insert(source, dest, dis);
    h->insert(dest, source, dis);
}

void routing::blockRoad(string src, string des)
{
    Node* head = h->search(src);
    if (!head) return;

    Node* n = head->next;
    while (n)
    {
        if (n->dest == des)
        {
            n->status = false;
            return;
        }
        n = n->next;
    }
}

int routing::getCityIndex(string cityName) {
    string searchName = Validation::toLowerCase(cityName); // Convert input to lowercase
    
    for (int i = 0; i < cityCount; i++) {
        // Convert the stored city to lowercase to compare
        if (Validation::toLowerCase(cities[i]) == searchName) {
            return i; 
        }
    }
    return -1; // City not found
}

bool routing::isValidCity(string cityName) {
    return getCityIndex(cityName) != -1; // Uses the case-insensitive search
}

int routing::getMinIndex(int dist[], bool visited[])
{
    int minVal = 2147483647, idx = -1;

    for (int i = 0; i < cityCount; i++)
        if (!visited[i] && dist[i] < minVal)
        {
            minVal = dist[i];
            idx = i;
        }
    return idx;
}

int routing::shortPath(string src, string dest)
{
    int srcIndex = getCityIndex(src);
    int destIndex = getCityIndex(dest);
    if (srcIndex == -1 || destIndex == -1)
        return -1;

    int dist[MAX];
    bool visited[MAX];

    for (int i = 0; i < cityCount; i++)
    {
        dist[i] = 2147483647;
        visited[i] = false;
    }

    dist[srcIndex] = 0;

    for (int count = 0; count < cityCount; count++)
    {
        int u = getMinIndex(dist, visited);
        if (u == -1) break;

        visited[u] = true;
        Node* head = h->search(cities[u]);
        if (!head) continue;

        Node* n = head->next;
        while (n)
        {
            if (n->dist != -1 && n->status)
            {
                int v = getCityIndex(n->dest);
                if (v != -1 && !visited[v] &&
                    dist[u] + n->dist < dist[v])
                {
                    dist[v] = dist[u] + n->dist;
                }
            }
            n = n->next;
        }
    }

    return (dist[destIndex] == 2147483647) ? -1 : dist[destIndex];
}

void routing::displayShortRoute(string src, string dest)
{
    int d = shortPath(src, dest);
    if (d == -1)
        cout << "No path exist between " << src << " and " << dest << endl;
    else
        cout << "Shortest distance from " << src << " to " << dest << " is: " << d << endl;
}

void routing::updatedistance(string src, string dest, int dis)
{
    Node* n = h->search(src);
    while (n)
    {
        if (n->dest == dest)
            n->dist = dis;
        n = n->next;
    }
}

// void routing::deleteCity(string city)
// {
//     Node* head = h->search(city);
//     if (!head) return;

//     string neighbors[50];
//     int count = 0;

//     // 1️⃣ Store neighbors
//     Node* curr = head->next;
//     while (curr)
//     {
//         if (curr->dist != -1)
//             neighbors[count++] = curr->dest;
//         curr = curr->next;
//     }

//     // 2️⃣ Delete key node + neighbors safely
//     curr = head;
//     while (curr)
//     {
//         Node* temp = curr;
//         curr = curr->next;
//         delete temp;
//     }

//     // 3️⃣ Remove city from other adjacency lists
//     for (int i = 0; i < count; i++)
//     {
//         Node* other = h->search(neighbors[i]);
//         if (!other) continue;

//         Node* prev = other;
//         Node* now = other->next;

//         while (now)
//         {
//             if (now->dest == city)
//             {
//                 prev->next = now->next;
//                 delete now;
//                 break;
//             }
//             prev = now;
//             now = now->next;
//         }
//     }

//     // 4️⃣ Remove city from city list
//     for (int i = 0; i < cityCount; i++)
//     {
//         if (cities[i] == city)
//         {
//             for (int j = i; j < cityCount - 1; j++)
//                 cities[j] = cities[j + 1];

//             cityCount--;
//             break;
//         }
//     }
// }

void routing::deleteCity(string city)
{
    // 1. Check if map is empty before doing ANYTHING
    if (cityCount == 0) {
        cout << YELLOW << BOLD << "\n[!] DATABASE EMPTY: No cities available to delete." << RESET << endl;
        return;
    }

    // 2. Validate if the city actually exists BEFORE asking for confirmation
    Node* head = h->search(city);
    if (!head) {
        cout << RED << BOLD << "\n[ERROR] '" << city << "' does not exist." << RESET << endl;
        displayAllCities(); // Show them what IS available so they can correct spelling
        return;
    }

    // 3. Professional Confirmation UI
    cout << "\n" << MAGENTA << "------------------------------------------" << RESET << endl;
    cout << RED << BOLD << "  SYSTEM ALERT: DELETION PROTOCOL" << RESET << endl;
    cout << "  TARGET CITY: " << WHITE << city << RESET << endl;
    cout << "  IMPACT: All connected roads will be severed." << endl;
    cout << MAGENTA << "------------------------------------------" << RESET << endl;
    
    string confirm = Validation::inputString(1, "Confirm permanent deletion? (y/n): ");
    if (confirm != "y" && confirm != "Y") {
        cout << CYAN << "\n[!] Deletion aborted. '" << city << "' remains safe." << RESET << endl;
        return;
    }

    // 4. Execution UI (Modern feel)
    cout << "\n" << YELLOW << "Wiping City Registry... " << RESET;

    string neighbors[50];
    int count = 0;

    // Step 1: Map neighbors
    Node* curr = head->next;
    while (curr) {
        if (curr->dist != -1)
            neighbors[count++] = curr->dest;
        curr = curr->next;
    }

    // Step 2: Clear memory 
    
    curr = head;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
    
    h->remove(city); // Remove from Hash Table

    // Step 3: Remove from other cities' lists
    for (int i = 0; i < count; i++) {
        Node* other = h->search(neighbors[i]);
        if (!other) continue;

        Node* prev = other;
        Node* now = other->next;

        while (now) {
            if (now->dest == city) {
                prev->next = now->next;
                delete now;
                break;
            }
            prev = now;
            now = now->next;
        }
    }

    // Step 4: Shift City Array
    for (int i = 0; i < cityCount; i++) {
        if (cities[i] == city) {
            for (int j = i; j < cityCount - 1; j++)
                cities[j] = cities[j + 1];
            cityCount--;
            break;
        }
    }

    // 5. Final Success Feedback
    cout << GREEN << "DONE!" << RESET << endl;
    cout << GREEN << BOLD << "[SUCCESS] Logistics Network recalculated. City purged." << RESET << endl;
}


// 1. Function to show all roads so Admin has "Visibility"
void routing::showAllRoadsBetween(string src, string dest) {
    Node* head = h->search(src); // Reuse existing search logic
    if (!head) {
        cout << "\033[31mSource city not found.\033[0m" << endl;
        return;
    }

    cout << "\nExisting roads from " << src << " to " << dest << ":" << endl;
    Node* n = head->next;
    bool found = false;
    while (n) {
        if (n->dest == dest) {
            cout << "- Distance: " << n->dist << " km | Status: " 
                 << (n->status ? "\033[32mAvailable\033[0m" : "\033[31mBlocked\033[0m") << endl;
            found = true;
        }
        n = n->next;
    }
    if (!found) cout << "No roads found between these cities." << endl;
}

// 2. Function to block a specific road by its distance
// void routing::blockSpecificRoad(string src, string des, int dist) {
//     Node* head = h->search(src); // Reuse existing search logic
//     if (!head) return;

//     Node* n = head->next;
//     while (n) {
//         // Find the specific road that matches both destination AND distance
//         if (n->dest == des && n->dist == dist) {
//             n->status = false; // Block it
//             cout << "\033[33mRoad (" << dist << "km) is now blocked.\033[0m" << endl;
//             return;
//         }
//         n = n->next;
//     }
//     cout << "Specific road with that distance not found." << endl;
// }

void routing::blockSpecificRoad(string src, string des, int dist) {
    // 1. Block the road from SRC to DES
    bool blockedForward = false;
    Node* headSrc = h->search(src);
    if (headSrc) {
        Node* n = headSrc->next;
        while (n) {
            if (n->dest == des && n->dist == dist) {
                n->status = false;
                blockedForward = true;
                break; 
            }
            n = n->next;
        }
    }

    // 2. Block the road from DES to SRC (The Reverse Direction)
    bool blockedBackward = false;
    Node* headDes = h->search(des);
    if (headDes) {
        Node* n = headDes->next;
        while (n) {
            if (n->dest == src && n->dist == dist) { // Note the swap: dest == src
                n->status = false;
                blockedBackward = true;
                break;
            }
            n = n->next;
        }
    }

    if (blockedForward && blockedBackward) {
        cout << "\033[33mRoad between " << src << " and " << des << " (" << dist << "km) is now blocked in both directions.\033[0m" << endl;
    } else {
        cout << RED << "Error: Could not find the full bidirectional road to block." << RESET << endl;
    }
}



// void routing::AddLocation(string city) {
//     // 1. Duplicate Check: Scan the existing array
//     for (int i = 0; i < cityCount; i++) {
//         if (cities[i] == city) {
//             cout << "\033[33m[!] Error: '" << city << "' is already present on the map.\033[0m" << endl;
//             return; // Exit function so duplicate is not added
//         }
//     }

//     // 2. Add if not a duplicate
//     if (cityCount < MAX) {
//         cities[cityCount++] = city;
//         h->insert(city, city, -1);
//         cout << "\033[32m[+] City '" << city << "' added successfully.\033[0m" << endl;
//     } else {
//         cout << "\033[31m[!] Map capacity reached.\033[0m" << endl;
//     }
// }
void routing::AddLocation(string city) {
    // 1. Duplicate Check: Scan using case-insensitive validation
    // This calls getCityIndex which uses toLowerCase to find 'lahore' even if 'Lahore' exists.
    if (isValidCity(city)) {
        cout << YELLOW << "[!] Error: '" << city << "' is already present on the map." << RESET << endl;
        return; // Exit function so duplicate is not added
    }

    // 2. Add if not a duplicate
    if (cityCount < MAX) {
        // We save the 'city' string exactly as the user typed it since we aren't using capitalize.
        cities[cityCount++] = city; 
        
        // Ensure the hash table node is created for this city
        h->insert(city, city, -1); 
        
        cout << GREEN << "[+] City '" << city << "' added successfully." << RESET << endl;
    } else {
        cout << RED << "[!] Map capacity reached." << RESET << endl;
    }
}

// int routing::getShortestDistance(string src, string dest) {
//     Node* head = h->search(src);
//     if (!head) return -1;

//     int minDis = 2147483647;
//     Node* n = head->next;
//     while (n) {
//         if (n->dest == dest && n->status && n->dist < minDis) {
//             minDis = n->dist;
//         }
//         n = n->next;
//     }
//     return (minDis == 2147483647) ? -1 : minDis;
// }

int routing::getShortestDistance(string src, string dest) {
    // Standardize case to handle "Lahore" vs "lahore"
    string s = Validation::toLowerCase(src);
    string d = Validation::toLowerCase(dest);

    Node* head = h->search(s); 
    if (!head) return -1;

    Node* n = head->next;
    while (n) {
        // Compare target against direct neighbors only
        if (Validation::toLowerCase(n->dest) == d && n->status) {
            return n->dist; 
        }
        n = n->next;
    }
    return -1; // No direct link exists
}



// // Logic to unblock a road
// void routing::unblockRecentRoad(string src, string dest) {
//     Node* head = h->search(src);
//     if (!head) return;
//     Node* n = head->next;
//     while (n) {
//         if (n->dest == dest && !n->status) {
//             n->status = true;
//             cout << "\033[32mRoad (" << n->dist << "km) is now Available.\033[0m" << endl;
//             return;
//         }
//         n = n->next;
//     }
//     cout << "No blocked roads found to unblock." << endl;
// }

void routing::unblockRecentRoad(string src, string des) {
    bool forwardFound = false;
    bool backwardFound = false;

    // 1. Unblock the edge in the Source City's list
    Node* headSrc = h->search(src);
    if (headSrc) {
        Node* n = headSrc->next;
        while (n) {
            if (n->dest == des && !n->status) {
                n->status = true; 
                forwardFound = true;
                break; 
            }
            n = n->next;
        }
    }

    // 2. Unblock the edge in the Destination City's list
    Node* headDes = h->search(des);
    if (headDes) {
        Node* n = headDes->next;
        while (n) {
            if (n->dest == src && !n->status) {
                n->status = true;
                backwardFound = true;
                break;
            }
            n = n->next;
        }
    }

    if (forwardFound && backwardFound) {
        cout << GREEN << BOLD << "[✓] Connection Restored: " << RESET 
             << WHITE << src << " <---> " << des << RESET << endl;
    } else {
        cout << RED << "[!] Error: No blocked segments found between these cities." << RESET << endl;
    }
}



bool routing::hasBlockedRoads(string src, string dest) {
    Node* head = h->search(src);
    if (!head) return false;

    Node* n = head->next;
    while (n) {
        if (n->dest == dest && !n->status) {
            return true; // Found at least one blocked road
        }
        n = n->next;
    }
    return false;
}


// bool routing::isValidCity(string city) {
//     // If getCityIndex returns -1, it means the city wasn't found
//     return getCityIndex(city) != -1;
// }


void routing::displayAllPaths() {
    cout << "\n\033[1;36m--- CITY REGISTRY ---\033[0m" << endl;
    displayAllCities(); 

    cout << "\033[1;36m\n--- DIRECT ROAD CONNECTIONS ---\033[0m" << endl;
    
    bool foundConnection = false;
    for (int i = 0; i < cityCount; i++) {
        // Search the hash table for the city's head node
        Node* head = h->search(cities[i]);
        if (!head) continue;

        Node* neighbor = head->next;
        while (neighbor) {
            // Logic: Only show the road if it's a direct connection (dist != -1)
            // and use (cities[i] < neighbor->dest) to avoid showing A-B and B-A
            if (neighbor->dist != -1 && cities[i] < neighbor->dest) {
                cout << WHITE << left << setw(15) << cities[i] 
                     << " <-> " << left << setw(15) << neighbor->dest 
                     << (neighbor->status ? GREEN : RED) 
                     << " [" << neighbor->dist << " km]" << RESET << endl;
                foundConnection = true;
            }
            neighbor = neighbor->next;
        }
    }

    if (!foundConnection) {
        cout << YELLOW << "[!] No direct roads have been added yet." << RESET << endl;
    }
}

void routing::displayAllCities() {
    if (cityCount == 0) return; // Handled by AdminMenu, but safe to keep

    cout << CYAN << BOLD << "REGISTERED CITIES IN NETWORK:" << RESET << endl;
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < cityCount; i++) {
        cout << WHITE << "  > " << left << setw(15) << cities[i];
        if ((i + 1) % 3 == 0) cout << endl; // Show 3 cities per line
    }
    cout << "\n----------------------------------------" << endl;
}


void routing::displayConnectedCitiesShortest() {
    bool anyRoad = false;
    cout << CYAN << BOLD << "ACTIVE DIRECT ROAD SEGMENTS:" << RESET << endl;
    cout << "--------------------------------------------" << endl;

    for (int i = 0; i < cityCount; i++) {
        // Get the head node for city A
        Node* head = h->search(cities[i]); 
        if (!head) continue;

        Node* n = head->next;
        while (n) {
            // Logic: 
            // 1. n->status ensures the road is active
            // 2. n->dist != -1 is a valid distance
            // 3. (cities[i] < n->dest) prevents showing the same road twice (A-B and B-A)
            if (n->status && n->dist != -1 && cities[i] < n->dest) {
                cout << WHITE << "  " << left << setw(12) << cities[i] 
                     << YELLOW << " <--- " << GREEN << n->dist << "km" << YELLOW << " ---> " << RESET 
                     << WHITE << left << setw(12) << n->dest << RESET << endl;
                anyRoad = true;
            }
            n = n->next;
        }
    }

    if (!anyRoad) {
        cout << RED << BOLD << " [!] NO DIRECT ROADS DETECTED IN THE NETWORK." << RESET << endl;
    }
    cout << "--------------------------------------------" << endl;
}



void routing::displayBlockedRoads() {
    cout << RED << BOLD << "CURRENTLY OFFLINE ROUTES:" << RESET << endl;
    cout << "--------------------------------------------" << endl;

    for (int i = 0; i < cityCount; i++) {
        Node* head = h->search(cities[i]); //
        if (!head) continue;

        Node* n = head->next;
        while (n) {
            // Show unique pairs only
            if (n->dist != -1 && !n->status && cities[i] < n->dest) {
                cout << WHITE << "  " << left << setw(12) << cities[i] 
                     << RED << "  [ OFFLINE ]  " << RESET 
                     << WHITE << left << setw(12) << n->dest 
                     << YELLOW << " (" << n->dist << " km)" << RESET << endl;
            }
            n = n->next;
        }
    }
    cout << "--------------------------------------------" << endl;
}