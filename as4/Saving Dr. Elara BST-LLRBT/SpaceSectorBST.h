#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

using namespace std;

class SpaceSectorBST {

private:
    void addSectorPrivate(int x, int y, int z, Sector *&current);
  
public:
    Sector *root;
    vector<Sector*> sector_list;
    vector<Sector*> leaf_list;
    vector<Sector*> inorder_list;
    vector<Sector*> replacement_list;
    typedef void (*FunctionType) (Sector & anItem);
    SpaceSectorBST();
    ~SpaceSectorBST();
    void clearTree(Sector* currentNode);
    Sector* createSectorLeaf(int x, int y, int z);
    void deleteItem (Sector *&treePtr, int x, int y, int z);
    int findInList(vector<Sector*> list, Sector *sector, int a);
    void deleteNode(Sector* &node);
    void clearLists();
    vector<string> customSplit(string &str, char separator);
    Sector* getSector(string sector_code);
    void processLeftmost(Sector * &nodeptr, Sector*& treeNode);
    void deleteNodeItem(Sector * &nodeptr);
    void preorder(Sector *treePtr);
    void inorder(Sector *treePtr);
    void postorder(Sector *treePtr);
    void readSectorsFromFile(const string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    vector<Sector*> getStellarPath(const string& sector_code);
    void printStellarPath(const vector<Sector*>& path);
    void deleteTree(Sector* &node);
};

#endif // SPACESECTORBST_H
