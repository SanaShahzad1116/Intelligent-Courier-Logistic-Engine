# SwiftEx – Intelligent Courier Logistics Engine

SwiftEx is a C++ based courier logistics management system developed as a **Data Structures & Algorithms** project. It simulates the complete courier workflow, from parcel registration and priority-based sorting to route calculation, tracking, and final delivery.

## Features

- Priority-based parcel sorting
- Dynamic shortest-path routing
- Blocked-road and alternative route handling
- Parcel tracking with delivery history
- Courier/rider assignment
- FIFO-based transit queue management
- Undo and audit log functionality
- Admin and User menu interfaces

## Data Structures & Algorithms

The project demonstrates the practical use of:

- **Heap** – Priority-based parcel management
- **Graph** – City and road network
- **Hash Table** – Fast parcel lookup
- **Queue** – Courier and transit management
- **Stack** – Undo operations
- **Linked Lists** – Dynamic data and history management
- **Dijkstra's Algorithm** – Shortest path calculation
- **Merge Sort / Heap operations** – Parcel organization

## Technologies

- **C++**
- Data Structures & Algorithms
- Object-Oriented Programming
- Console-based CLI
- Git & GitHub

## Project Structure

```text
DSA-3rd/
├── AdminMenu.cpp / .h
├── CourierSystem.cpp / .h
├── Menu.cpp / .h
├── Parcel.cpp / .h
├── ParcelTracking.cpp / .h
├── SortingModule.cpp / .h
├── UserMenu.cpp / .h
├── Validator.cpp / .h
├── UIhelper.cpp / .h
├── routing.cpp / .h
├── main.cpp
└── README.md
How to Run

Make sure g++ is installed and available in your terminal.

Compile all C++ source files:

g++ *.cpp -o main.exe

Run the program:

.\main.exe
Team

Developed by Sana Shahzad, Muhammad Ali Rana, Ayesha Qayyum, and Usman Shahid for CSC-261 Data Structures & Algorithm at UET Lahore – New Campus.
