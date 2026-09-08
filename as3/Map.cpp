#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <vector>
#include <algorithm>

const int INF = std::numeric_limits<int>::max();


Map::Map() {
    distanceMatrix = new int*[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i) {
        distanceMatrix[i] = new int[MAX_SIZE];
        for (int j = 0; j < MAX_SIZE; ++j) {
            distanceMatrix[i][j] = INF; // Initialize with INF (no direct connection)
        }
        visited[i] = false; // Initially, all provinces are unvisited
    }
}

void Map::loadDistanceData(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << "\n";
        return;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line) && row < MAX_SIZE) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;

        while (std::getline(ss, cell, ',') && col < MAX_SIZE) {
            try {
                int distance = std::stoi(cell); // Convert string to integer
                if (distance < 0) {
                    distanceMatrix[row][col] = INF; // Invalid entries are treated as no connection
                } else {
                    distanceMatrix[row][col] = distance;
                }
            } catch (const std::invalid_argument&) {
                distanceMatrix[row][col] = INF; // Handle invalid entries
            }
            ++col;
        }
        ++row;
    }

    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    if (provinceA < 0 || provinceA >= MAX_SIZE || provinceB < 0 || provinceB >= MAX_SIZE) {
        return false; // Invalid province indices
    }
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    return province >= 0 && province < MAX_SIZE && visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    std::fill(visited, visited + MAX_SIZE, false);
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    return std::count(visited, visited + MAX_SIZE, true);
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    if (provinceA < 0 || provinceA >= MAX_SIZE || provinceB < 0 || provinceB >= MAX_SIZE) {
        return INF; // Invalid indices
    }
    return distanceMatrix[provinceA][provinceB];
}

// Destructor
Map::~Map() {
    if (distanceMatrix) {
        for (int i = 0; i < MAX_SIZE; ++i) {
            delete[] distanceMatrix[i];
        }
        delete[] distanceMatrix;
    }
}