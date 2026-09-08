#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper* realmShaper, Isle* isle) {
    if (realmShaper == nullptr) {
        return false;
    }
    if (isle == nullptr) {
        return false;
    }
    int shaperDepth = shaperTree.getDepth(realmShaper);
    int minDepth = mapTree.calculateMinMapDepthAccess(shaperDepth, shaperTree.getDepth(),mapTree.getDepth());

    if (shaperDepth >= minDepth) {
        return true;
    }
    return false;
}

void GameWorld::exploreArea(RealmShaper* realmShaper, Isle* isle) {
    if (!hasAccess(realmShaper, isle)) {
        return;
    }

    // Visit the Isle
    if(!mapTree.findIsle(*isle))
        craft(realmShaper, isle->getName());
    isle->increaseShaperCount();
    realmShaper->collectItem(isle->getItem());

    // Handle overcrowding
    if (!isle->increaseShaperCount()) {
       mapTree.remove(isle);
    }
}

void GameWorld::craft(RealmShaper* shaper, const std::string& isleName) {
    if (shaper->hasEnoughEnergy()) {
        shaper->loseEnergy();

        Isle* newIsle = new Isle(isleName);
        mapTree.insert(newIsle);  // Ensure the mapTree properly deletes the Isle when it's removed

    }
}


void GameWorld::displayGameState() {
    mapTree.displayMap();
    shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string& accessLogs, const std::string& duelLogs) {
    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    std::string accessLine, duelLine;
    int accessCount = 0;

    while (std::getline(accessFile, accessLine)) {
        std::istringstream accessStream(accessLine);
        std::string shaperName, isleName;
        accessStream >> shaperName >> isleName;

        RealmShaper* shaper = shaperTree.findPlayer(shaperName);
        Isle* isle = mapTree.findIsle(isleName);

        if(shaper != nullptr && isle != nullptr){
            exploreArea(shaper, isle);
            accessCount++;

            if (accessCount % 5 == 0 && std::getline(duelFile, duelLine)) {
                // Process duel logs
                std::istringstream duelStream(duelLine);
                std::string challengerName, opponentName;
                duelStream >> challengerName >> opponentName;

                RealmShaper *challenger = shaperTree.findPlayer(challengerName);
                RealmShaper *opponent = shaperTree.findPlayer(opponentName);

                if (challenger && opponent) {
                    shaperTree.duel(challenger, opponent);
                }
            }
        }
    }
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}

