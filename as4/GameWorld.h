#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Map.h"
#include "RealmShapers.h"
#include <vector>
#include <string>

class GameWorld
{
private:
    Map mapTree; // Represents the game map (AVL tree of Isles)
    ShaperTree shaperTree; // Represents the shaper hierarchy (complete binary tree)

public:
    // Constructor
    GameWorld();

    // Accessors
    Map& getMapTree();
    ShaperTree& getShaperTree();

    // Initialize the game with a set of Isles and RealmShapers
    void initializeGame(std::vector<Isle*> isles, std::vector<RealmShaper*> realmShapers);

    // Check if a RealmShaper has access to a particular Isle
    bool hasAccess(RealmShaper* realmShaper, Isle* isle);

    // Explore an area with a given RealmShaper
    void exploreArea(RealmShaper* realmShaper, Isle* isle);

    // Craft a new Isle using the RealmShaper's energy
    void craft(RealmShaper* shaper, const std::string& isleName);

    // Display the current game state in the terminal
    void displayGameState();

    // Process game events using access and duel logs
    void processGameEvents(const std::string& accessLogs, const std::string& duelLogs);

    // Save the current game state to files
    void saveGameState(const std::string& currentIsles, const std::string& currentWorld,
                       const std::string& currentShapers, const std::string& currentPlayerTree);
};

#endif
