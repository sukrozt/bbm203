#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
        "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
        "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir",
        "Edirne",
        "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay",
        "Isparta",
        "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya",
        "Kutahya",
        "Malatya", "Manisa", "Kahramanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
        "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
        "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
        "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string &distance_data, const std::string &priority_data,
                           const std::string &restricted_data, int maxDistance)
        : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    map.loadDistanceData(distance_data);
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
    map.resetVisited();
    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially

    // Load priority provinces
    // Load restricted provinces
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string &filename) {
    std::ifstream file(filename);
    numPriorityProvinces = 0;
    std::string line;

    while (std::getline(file, line) && numPriorityProvinces < MAX_PRIORITY_PROVINCES) {
        size_t start = line.find('(');
        size_t end = line.find(')');

        if (start != std::string::npos && end != std::string::npos) {
            int provinceIndex = std::stoi(line.substr(start + 1, end - start - 1));
            priorityProvinces[numPriorityProvinces++] = provinceIndex;
        }
    }

    file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;
    numWeatherRestrictedProvinces = 0;

    while (std::getline(file, line) && numWeatherRestrictedProvinces < MAX_WEATHER_RESTRICTED_PROVINCES) {
        size_t start = line.find('(');
        size_t end = line.find(')');

        if (start != std::string::npos && end != std::string::npos) {
            int provinceIndex = std::stoi(line.substr(start + 1, end - start - 1));
            weatherRestrictedProvinces[numWeatherRestrictedProvinces++] = provinceIndex;
        }
    }

    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) return true;
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) return true;
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    map.markAsVisited(startingCity);
    stack.push(startingCity);
    route.push_back(startingCity);
    currentDistance = 0;
    totalDistanceCovered = 0;

    while (!isExplorationComplete()) {
        exploreFromProvince(stack.peek());
    }

    displayResults();
}


// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);

    while (!queue.isEmpty()) {
        int nextProvince = queue.dequeue();

        if (map.isVisited(nextProvince) ||
            isWeatherRestricted(nextProvince)) {
            if (isWeatherRestricted(nextProvince)) {
                std::cout << cities[nextProvince] << " is weather-restricted. Skipping.\n";
            }
            continue;
        }

        // Update distances and add to the route
        totalDistanceCovered += map.getDistance(province, nextProvince);
        currentDistance += map.getDistance(province, nextProvince);
        route.push_back(nextProvince);
        stack.push(nextProvince);

        // Mark as visited and explore further
        map.markAsVisited(nextProvince);

        // Handle refueling logic
        if (currentDistance >= maxDistance) {
            currentDistance = 0; // Simulate refueling
        }
    }

    // Backtrack if needed
    if (!isExplorationComplete()) {
        backtrack();
    }
}



void RoutePlanner::enqueueNeighbors(int province) {
    for (int nextProvince = 0; nextProvince < MAX_SIZE; ++nextProvince) {
        if (!map.isVisited(nextProvince)&& map.isWithinRange(province, nextProvince, maxDistance)) {
            if (isPriorityProvince(nextProvince)) {
                queue.enqueuePriority(nextProvince); // Add to the front
            } else {
                queue.enqueue(nextProvince); // Add to the back
            }
        }
    }
}



void RoutePlanner::backtrack() {
    if (isExplorationComplete()) {
        dead_end = true;
        return;
    }

    stack.pop();

    if (!stack.isEmpty() && queue.isEmpty()) {
        int previousProvince = stack.peek();
    }
}



void RoutePlanner::displayResults() const {
    std::cout << "----------------------------\n";
    std::cout << "Journey Completed!\n";
    std::cout << "----------------------------\n";
    std::cout << "Total Number of Provinces Visited: " << map.countVisitedProvinces() << "\n";
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km\n";
    std::cout << "Route Taken: \n";
    for (size_t i = 0; i < route.size(); ++i) {
        std::cout << cities[route[i]];
        std::cout << " -> ";  // Print the arrow for non-last provinces
        if (i == route.size() - 1)
            std::cout << " End\n";  // Print "End" for the last province
    }
    std::cout << "\n";
    displayPriorityProvincesSummary();
}

bool RoutePlanner::isExplorationComplete() const{
    if(stack.isEmpty() && queue.isEmpty())
        return true;
    return false;
}

void RoutePlanner::displayPriorityProvincesSummary() const {
    int visitedPriorityCount = 0;

    // Count how many priority provinces were visited
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (map.isVisited(priorityProvinces[i])) {
            visitedPriorityCount++;
        }
    }

    // Display priority provinces visit status
    std::cout << "Priority Provinces Status:\n";
    for (int i = 0; i < numPriorityProvinces; ++i) {
        std::cout << cities[priorityProvinces[i]] << " (";
        if (map.isVisited(priorityProvinces[i])) {
            std::cout << "Visited)";
        } else {
            std::cout << "Not Visited)";
        }
        std::cout << "\n";
    }

    // Display the total number of priority provinces visited and the corresponding message
    std::cout << "\nTotal Priority Provinces Visited: " << visitedPriorityCount
              << " out of " << numPriorityProvinces << "\n";

    if (visitedPriorityCount == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited.\n";
    } else {
        std::cout << "Warning: Not all priority provinces were visited.\n";
    }
}
