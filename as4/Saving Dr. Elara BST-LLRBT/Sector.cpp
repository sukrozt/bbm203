#include "Sector.h"
#include <cmath>

// Constructor implementation
Sector::Sector() {}
Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code

    double distance = pow(x,2) + pow(y , 2) + pow(z,2);
    distance_from_earth = sqrt(distance);
    sector_code = "";
    //sector code assignment
    int distanceAsInteger = static_cast<int>(distance_from_earth);
    sector_code += to_string(distanceAsInteger);
    if (x == 0)
        sector_code += "S";
    else if (x < 0)
        sector_code += "L";
    else
        sector_code += "R";
    if (y == 0)
        sector_code += "S";
    else if (y < 0)
        sector_code += "D";
    else
        sector_code += "U";
    if (z == 0)
        sector_code += "S";
    else if (z < 0)
        sector_code += "B";
    else
        sector_code += "F";
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary

}

Sector& Sector::operator=(const Sector& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}