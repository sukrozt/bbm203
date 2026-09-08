#include <algorithm>
#include <functional>
#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map() {
    // Assuming you have a recursive delete method for the nodes in the tree
    std::function<void(MapNode*)> deleteNodes = [&](MapNode* node) {
        if (!node) return;
        deleteNodes(node->left);
        deleteNodes(node->right);
        delete node;  // Ensure the node is deleted
    };
    deleteNodes(root);
}


void Map::initializeMap(std::vector<Isle *> isles)
{
    for (auto isle : isles)
        insert(isle);

    populateWithItems();
}

MapNode *Map::rotateRight(MapNode *current)
{
    if (!current || !current->left)
    {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

MapNode *Map::rotateLeft(MapNode *current)
{
    if (!current || !current->right)
    {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    return node ? node->height : 0;
}

MapNode* Map::insert(MapNode* node, Isle* isle)
{
    // Base case: If the node is null, create a new node.
    if (!node)
        return new MapNode(isle);

    // Traverse the tree to find the correct spot for the new isle.
    if (*isle < *node->isle)
        node->left = insert(node->left, isle);
    else if (*isle > *node->isle)
        node->right = insert(node->right, isle);
    else
        return node;  // Isle already exists, return the node.

    // Update the height of the current node after insertion.
    node->height = 1 + std::max(height(node->left), height(node->right));

    // Calculate the balance factor to check whether this node became unbalanced.
    int balance = getBalance(node);

    // Left heavy situation (right rotation).
    if (balance > 1 && *isle < *node->left->isle)
        return rotateRight(node);

    // Right heavy situation (left rotation).
    if (balance < -1 && *isle > *node->right->isle)
        return rotateLeft(node);

    // Left-right case (left rotation on left child, then right rotation).
    if (balance > 1 && *isle > *node->left->isle)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-left case (right rotation on right child, then left rotation).
    if (balance < -1 && *isle < *node->right->isle)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}


void Map::insert(Isle *isle)
{
    //root = insert((root), isle);

    // you might need to insert some checks / functions here depending on your implementation
}
int Map::getBalance(MapNode *node)
{
    if (!node)
        return 0;
    return height(node->left) - height(node->right);
}


MapNode *Map::remove(MapNode *node, Isle *isle){
    if (!node)
    {
        return node;
    }

    if (*isle < *node->isle)
        node->left = remove(node->left, isle);
    else if (*isle > *node->isle)
        node->right = remove(node->right, isle);
    else
    {
        if (!node->left || !node->right)
        {
            MapNode *temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        }

        MapNode *temp = node->right;
        while (temp->left)
            temp = temp->left;

        node->isle = temp->isle;
        node->right = remove(node->right, temp->isle);
    }

    node->height = std::max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    // Left heavy
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right heavy
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}


void Map::remove(Isle *isle)
{
    //root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
    delete isle;
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if (!current)
        return;

    count++;
    if (count % 5 == 0)
    {
        current->isle->setItem(EINSTEINIUM);
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}


// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if (!current)
        return;

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    count++;
    if (count % 3 == 0)
    {
        current->isle->setItem(GOLDIUM);
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (!node)
        return nullptr;

    if (!node->isle->getItem() == EMPTY)
        return node;

    MapNode *leftResult = findFirstEmptyIsle(node->left);
    return leftResult ? leftResult : findFirstEmptyIsle(node->right);
}

void Map::dropItemBFS()
{
    static int rebalanceCount = 0;
    rebalanceCount++;

    if (rebalanceCount % 3 == 0)
    {
        MapNode *targetNode = findFirstEmptyIsle(root);
        if (targetNode)
        {
            targetNode->isle->setItem(AMAZONITE);
        }

    }
}

int Map::getDepth(MapNode *node)
{
    if (!node)
        return -1;

    int depth = 0;
    MapNode *current = root;
    while (current)
    {
        if (*current->isle == *node->isle)
            return depth;
        else if (*node->isle < *current->isle)
            current = current->left;
        else
            current = current->right;
        depth++;
    }
    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    MapNode *current = root;
    int depth = 0;
    while (current)
    {
        if (*current->isle == *isle)
            return depth;
        else if (*isle < *current->isle)
            current = current->left;
        else
            current = current->right;
        depth++;
    }
    return -1;
}

int Map::getDepth()
{
    std::function<int(MapNode *)> calculateDepth = [&](MapNode *node) -> int
    {
        if (!node)
            return 0;
        return 1 + std::max(calculateDepth(node->left), calculateDepth(node->right));
    };
    return calculateDepth(root);
}

void Map::populateWithItems()
{
    int count = 0;
    postOrderItemDrop(root, count);
    count = 0;
    preOrderItemDrop(root, count);
}

Isle *Map::findIsle(Isle isle)
{
    MapNode *current = root;
    while (current)
    {
        if (*current->isle == isle)
            return current->isle;
        else if (isle < *current->isle)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    MapNode *current = root;
    while (current)
    {
        if (current->isle->getName() == name)
            return current->isle;
        else if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;
    while (current)
    {
        if (*current->isle == isle)
            return current;
        else if (isle < *current->isle)
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;
    while (current)
    {
        if (current->isle->getName() == name)
            return current;
        else if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }
    return nullptr;
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

void Map::display(MapNode *current, int depth, int state)
{
    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        std::cout << "     ";

    if (state == 1) // left
        std::cout << "   ┌───";
    else if (state == 2) // right
        std::cout << "   └───";

    std::cout << "[" << current->isle->getName() << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}


void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "[Error] Unable to open file: " << filename << std::endl;
        return;
    }

    if (!root)
    {
        file.close();
        return; // Handle the empty tree case
    }

    std::queue<MapNode*> q;
    q.push(root);
    int level = 0;

    while (!q.empty())
    {
        int levelSize = q.size(); // Number of nodes at the current level
        std::vector<std::string> currentLevelNames; // Store the names for the current level

        // Process all nodes at the current level
        for (int i = 0; i < levelSize; i++)
        {
            MapNode *current = q.front();
            q.pop();

            if (current)
            {
                currentLevelNames.push_back(current->isle->getName());
                q.push(current->left);
                q.push(current->right);
            }
            else
            {
                currentLevelNames.push_back("NULL");
            }
        }

        // Print the current level's node names
        file << level + 1; // Level number starts at 1, not 0
        for (const auto &name : currentLevelNames)
        {
            file << " " << name;
        }
        file << std::endl;

        level++;
    }

    file.close();
}


void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "[Error] Unable to open file: " << filename << std::endl;
        return;
    }

    std::vector<Isle *> isles;
    std::function<void(MapNode *)> collectIsles = [&](MapNode *node)
    {
        if (!node)
            return;
        collectIsles(node->left);
        isles.push_back(node->isle);
        collectIsles(node->right);
    };

    collectIsles(root);

    std::sort(isles.begin(), isles.end(), [](Isle *a, Isle *b) {
        return a->getName() < b->getName();
    });

    for (const auto &isle : isles)
        file << isle->getName() << "\n";

    file.close();
}