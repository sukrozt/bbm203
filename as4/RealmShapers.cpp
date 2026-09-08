#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    for (auto *shaper : realmShapers)
    {
        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    for (auto *shaper : shapers)
    {
        insert(shaper);
    }
}

int ShaperTree::getSize()
{
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    return index >= 0 && index < static_cast<int>(realmShapers.size());
}

void ShaperTree::insert(RealmShaper *shaper)
{
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    auto it = std::find(realmShapers.begin(), realmShapers.end(), shaper);
    if (it != realmShapers.end())
    {
        int index = std::distance(realmShapers.begin(), it);
        delete *it;
        realmShapers.erase(it);
        return index;
    }
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    auto it = std::find(realmShapers.begin(), realmShapers.end(), shaper);
    return (it != realmShapers.end()) ? std::distance(realmShapers.begin(), it) : -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index == -1)
        return -1;

    return static_cast<int>(std::log2(index + 1));
}

int ShaperTree::getDepth()
{
    return static_cast<int>(std::log2(realmShapers.size()));
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    if (realmShapers.empty() || !challenger)
        throw std::invalid_argument("Duel cannot proceed without valid participants.");

    // Get index of challenger
    int challengerIndex = findIndex(challenger);
    if (challengerIndex == -1)
        throw std::invalid_argument("Challenger does not exist in the tree.");

    // Ensure challenger has a valid parent
    int parentIndex = (challengerIndex - 1) / 2; // Parent of node at i is (i - 1) / 2
    if (!isValidIndex(parentIndex))
        throw std::invalid_argument("Challenger has no parent and cannot initiate a duel.");

    RealmShaper *opponent = realmShapers[parentIndex]; // Parent node as opponent

    // Update Honour Points based on duel result
    if (result) // Challenger wins
    {
        challenger->gainHonour();
        opponent->loseHonour();

        // Swap challenger and opponent in the tree
        std::swap(realmShapers[challengerIndex], realmShapers[parentIndex]);
    }
    else // Opponent wins
    {
        challenger->loseHonour();
        opponent->gainHonour();
    }

    // Handle player elimination if Honour Points reach zero
    if (opponent->getHonour() <= 0)
    {
        std::cout << "[Duel] " << opponent->getName() << " lost all honour, removed from tree." << std::endl;
        remove(opponent);
    }

    if (challenger->getHonour() <= 0)
    {
        std::cout << "[Duel] " << challenger->getName() << " lost all honour, removed from tree." << std::endl;
        remove(challenger);
    }

    return *challenger; // Return updated challenger
}



RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    int index = findIndex(shaper);
    if (index <= 0)
        return nullptr;

    int parentIndex = (index - 1) / 2;
    return isValidIndex(parentIndex) ? realmShapers[parentIndex] : nullptr;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    if (!player_low || !player_high)
    {
        std::cerr << "[Error] Cannot replace null players." << std::endl;
        return;
    }

    int index_low = findIndex(player_low);
    int index_high = findIndex(player_high);

    if (index_low == -1 || index_high == -1)
    {
        std::cerr << "[Error] One or both players not found in the tree." << std::endl;
        return;
    }

    // Swap the two players in the realmShapers vector
    std::swap(realmShapers[index_low], realmShapers[index_high]);
    std::cout << "[Info] Players swapped: " << player_low->getName() << " and " << player_high->getName() << std::endl;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    for (RealmShaper *currentShaper : realmShapers)
    {
        if (*currentShaper == shaper)
        {
            return currentShaper;
        }
    }

    return nullptr; // Return nullptr if not found
}


// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    for (auto *shaper : realmShapers)
    {
        if (shaper->getName() == name)
        {
            return shaper;
        }
    }
    return nullptr;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    // Recursive in-order traversal: Left, Root, Right
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Visit left subtree
    auto leftResult = inOrderTraversal(leftChild);
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    // Visit current node
    result.push_back(realmShapers[index]->getName());

    // Visit right subtree
    auto rightResult = inOrderTraversal(rightChild);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    // Recursive pre-order traversal: Root, Left, Right
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Visit current node
    result.push_back(realmShapers[index]->getName());

    // Visit left subtree
    auto leftResult = preOrderTraversal(leftChild);
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    // Visit right subtree
    auto rightResult = preOrderTraversal(rightChild);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result;
    if (!isValidIndex(index))
        return result;

    // Recursive post-order traversal: Left, Right, Root
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Visit left subtree
    auto leftResult = postOrderTraversal(leftChild);
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    // Visit right subtree
    auto rightResult = postOrderTraversal(rightChild);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    // Visit current node
    result.push_back(realmShapers[index]->getName());

    return result;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index))
        return;

    // Recursive pre-order traversal with file output: Root, Left, Right
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    // Write current node to file
    outFile << realmShapers[index]->getName() << std::endl;

    // Visit left subtree
    preOrderTraversal(leftChild, outFile);

    // Visit right subtree
    preOrderTraversal(rightChild, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    if (realmShapers.empty())
        return;

    // Level-order traversal using a queue
    std::queue<int> queue;
    queue.push(0);

    while (!queue.empty())
    {
        int currentIndex = queue.front();
        queue.pop();

        // Write current node to file
        outFile << realmShapers[currentIndex]->getName() << std::endl;

        // Add left child to queue
        int leftChild = 2 * currentIndex + 1;
        if (isValidIndex(leftChild))
            queue.push(leftChild);

        // Add right child to queue
        int rightChild = 2 * currentIndex + 2;
        if (isValidIndex(rightChild))
            queue.push(rightChild);
    }
}


void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    // Print the current node
    std::cout << prefix << (level > 0 ? "   └---- " : "") << realmShapers[index]->getName() << std::endl;

    // Calculate left and right child indices
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Recursively print the left and right subtrees
    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : ""));
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    if (realmShapers.empty())
    {
        std::cerr << "[Error] Tree is empty. No shapers to write to file." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "[Error] Could not open file: " << filename << std::endl;
        return;
    }

    // Use level-order traversal to write shapers level by level
    std::queue<int> queue;
    queue.push(0); // Start with root

    while (!queue.empty())
    {
        int currentIndex = queue.front();
        queue.pop();

        outFile << realmShapers[currentIndex]->getName() << " " << realmShapers[currentIndex]->getHonour() << std::endl;

        int left = 2 * currentIndex + 1;
        int right = 2 * currentIndex + 2;

        if (isValidIndex(left))
            queue.push(left);
        if (isValidIndex(right))
            queue.push(right);
    }

    outFile.close();
}

void ShaperTree::writeToFile(const std::string &filename)
{
    if (realmShapers.empty())
    {
        std::cerr << "[Error] Tree is empty. No tree to write to file." << std::endl;
        return;
    }

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "[Error] Could not open file: " << filename << std::endl;
        return;
    }

    // Use pre-order traversal to write the tree
    preOrderTraversal(0, outFile);

    outFile.close();
}

