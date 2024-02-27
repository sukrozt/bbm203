#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    vector <Sector*> sector_list;
    SpaceSectorLLRBT();
    void balanceTree();
    ~SpaceSectorLLRBT();
    void freeTree(Sector* node);
    Sector* getSector(const string& sector_code);
    void storeInOrder(Sector* node, vector<Sector*>& nodes);
    Sector* sortedArrayToBST(const vector<Sector*>& nodes, int start, int end);
    void findPath(Sector* start, Sector* end, std::vector<Sector*>& path);
    vector<string> customSplit(string &str, char separator);
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    Sector* leftRotate(Sector* node);
    Sector* rightRotate(Sector* node);
    void flipColors(Sector* node);
    bool isRed(Sector* node);
    Sector* insertSectorRecursive(Sector* node, int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    void displaySectorsPostOrderRecursive(Sector* node);
    void displaySectorsInOrderRecursive(Sector* node);
    void displaySectorsPreOrderRecursive(Sector* node);
    vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
