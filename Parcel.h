#ifndef PARCEL_H
#define PARCEL_H

#include <string>

class Parcel {
private:
    int id;
    float weight;
    std::string source;
    std::string destination;
    int priority; // 1 = overnight, 2 = 2-Day, 3 = Normal

public:
    Parcel();
    Parcel(int id, float weight, std::string source, std::string destination, int priority);

    // Getters (Accessors)
    int getId() const;
    float getWeight() const;
    std::string getDestination() const;
    int getPriority() const;
    std::string getSource() ;

    // Setters (Mutators) with basic validation
    void setId(int id);
    void setWeight(float weight);
    void setDestination(std::string destination);
    void setPriority(int priority);
    // string getWeightCategory() const;
    // string getZone() const;
    std::string showPriority() const;
    void displayParcel() const;
};

#endif