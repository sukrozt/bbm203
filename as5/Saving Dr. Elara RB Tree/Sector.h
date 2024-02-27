#ifndef SECTOR_H
#define SECTOR_H

#include <string>

const bool RED = true;
const bool BLACK = false;
using namespace std;

class Sector {
public:

    Sector(int x, int y, int z);
    ~Sector();
    int x, y, z; // Coordinates of the sector
    double distance_from_earth; // Calculated Euclidean distance from the Earth
    string sector_code; // Unique identifier based on coordinates and distance
    Sector *left, *right, *parent; // Pointers to child and parent nodes
    bool color; // Node color for Red-Black Tree


    // Overloaded operators
    Sector& operator=(const Sector& other);
    bool operator==(const Sector& other) const;
    bool operator!=(const Sector& other) const;

};

#endif // SECTOR_H
