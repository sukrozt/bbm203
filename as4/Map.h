#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <queue>
#include <cmath>
#include "Isle.h"

struct MapNode
{
    Isle *isle;
    MapNode *left, *right;
    int height;

    MapNode(Isle *isle) : isle(isle), left(nullptr), right(nullptr), height(1) {}
    ~MapNode()
    {
        // TODO: Free any dynamically allocated memory if necessary
    }
};

class Map
{
private:
    MapNode *root; // Root node of the tree

    // Height of a node
    int height(MapNode *node);

    // Calculate balance factor of a node
    int getBalance(MapNode *node);

    // AVL Rotations
    MapNode *rotateRight(MapNode *current);
    MapNode *rotateLeft(MapNode *current);

    // Helper functions for recursive AVL insertion and deletion
    MapNode *insert(MapNode *node, Isle *isle);
    MapNode *remove(MapNode *node, Isle *isle);

    // Item drop helper functions
    void preOrderItemDrop(MapNode *current, int &count);
    void postOrderItemDrop(MapNode *current, int &count);

public:
    Map();
    ~Map();

    // Tree operations
    void insert(Isle *isle);
    void remove(Isle *isle);
    Isle *findIsle(Isle isle);
    Isle *findIsle(std::string name);
    MapNode *findNode(Isle isle);
    MapNode *findNode(std::string name);

    // Initialize tree from a vector
    void initializeMap(std::vector<Isle *> isles);
    MapNode *findFirstEmptyIsle(MapNode *node);

    int getDepth(MapNode *node); // Determines the depth of a node within the tree.
    int getIsleDepth(Isle *isle); // Determines the depth of a specific Isle.
    int getDepth(); // Total depth of the tree

    // Display tree in terminal
    void display(MapNode *current, int depth, int state);
    void displayMap();

    // Item drop API
    void populateWithItems();
    void dropItemBFS();

    int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth);

    // Write the AVL tree to a file with levels
    void writeToFile(const std::string &filename);
    void writeIslesToFile(const std::string &filename);
};

#endif
