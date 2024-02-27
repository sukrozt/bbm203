#include "SpaceSectorLLRBT.h"
#include <algorithm>

using namespace std;


SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {

}

void SpaceSectorLLRBT::freeTree(Sector* node) {
    if (node != nullptr) {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}

vector<string> SpaceSectorLLRBT::customSplit(string &str, char separator) {
    vector<string> result;
    istringstream ss(str);
    string token;

    while (getline(ss, token, separator)) {
        result.push_back(token);
    }

    return result;
}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string &filename) {
    ifstream sectors_file(filename);
    string coordinate_line;

    while (getline(sectors_file, coordinate_line)) {
        vector<string> coordinate = customSplit(coordinate_line, ',');
        if (coordinate[0] == "X") continue;
        else {
            int x = stoi(coordinate[0]);
            int y = stoi(coordinate[1]);
            int z = stoi(coordinate[2]);
            insertSectorByCoordinates(x, y, z);
        }
    }

    sectors_file.close();
}

SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    /*for (int i = 0; i < sector_list.size(); i++) {
        Sector *temp = sector_list.at(i);
        delete temp;
    }
    sector_list.clear();*/
    freeTree(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    root = insertSectorRecursive(root, x, y, z);
    root->color = BLACK; // Ensure the root is always black after insertion
}

Sector* SpaceSectorLLRBT::insertSectorRecursive(Sector* node, int x, int y, int z) {
    if (node == nullptr) {
        Sector *sector = new Sector(x, y, z); // Create a new red node
        sector_list.push_back(sector);
        sector->color = RED;
        return sector;
    }

    // Perform standard BST insertion
    if (x < node->x || (x == node->x && y < node->y) ||
        (x == node->x && y == node->y && z < node->z)) {
        node->left = insertSectorRecursive(node->left, x, y, z);
        node->left->parent = node;
    } else if (x > node->x || (x == node->x && y > node->y) ||
               (x == node->x && y == node->y && z > node->z)) {
        node->right = insertSectorRecursive(node->right, x, y, z);
        node->right->parent = node;
    } else {
        // Ignore duplicates
        return node;
    }

    // Fix any LLRBT violations
    if (isRed(node->right) && !isRed(node->left)) {
        node = leftRotate(node);
    }
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rightRotate(node);
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }

    return node;
}

// Helper function to perform left rotation
Sector* SpaceSectorLLRBT::leftRotate(Sector* node) {
    Sector* newRoot = node->right;
    newRoot->parent = node->parent;
    node->parent = newRoot;
    node->right = newRoot->left;
    newRoot->left = node;
    newRoot->color = node->color;
    node->color = RED;
    if(node->right == nullptr){}
    else{
        node->right->parent = node;
    }
    return newRoot;
}

// Helper function to perform right rotation
Sector* SpaceSectorLLRBT::rightRotate(Sector* node) {
    Sector* newRoot = node->left;
    newRoot->parent = node->parent;
    node->parent = newRoot;
    node->left = newRoot->right;
    newRoot->right = node;
    newRoot->color = node->color;
    node->color = RED;
    if(node->left == nullptr){}
    else{
        node->left->parent = node;
    }
    return newRoot;
}

// Helper function to flip colors
void SpaceSectorLLRBT::flipColors(Sector* node) {
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;
}

// Helper function to check if a node is red (handles nullptr case as well)
bool SpaceSectorLLRBT::isRed(Sector* node) {
    return (node != nullptr) && (node->color == RED);
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    cout << "Space sectors inorder traversal:" << endl;
    displaySectorsInOrderRecursive(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:" << endl;
    displaySectorsPreOrderRecursive(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    cout << "Space sectors postorder traversal:" << endl;
    displaySectorsPostOrderRecursive(root);
    cout << endl;
}

void SpaceSectorLLRBT::displaySectorsInOrderRecursive(Sector* node) {
    if (node != nullptr) {
        displaySectorsInOrderRecursive(node->left);
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
        displaySectorsInOrderRecursive(node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrderRecursive(Sector* node) {
    if (node != nullptr) {
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
        displaySectorsPreOrderRecursive(node->left);
        displaySectorsPreOrderRecursive(node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrderRecursive(Sector* node) {
    if (node != nullptr) {
        displaySectorsPostOrderRecursive(node->left);
        displaySectorsPostOrderRecursive(node->right);
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
    }
}

void SpaceSectorLLRBT::balanceTree() {
    vector<Sector*> nodes;
    storeInOrder(root, nodes);

    root = nullptr;

    root = sortedArrayToBST(nodes, 0, nodes.size() - 1);

    if (root != nullptr) {
        root->color = BLACK;
    }
}

void SpaceSectorLLRBT::storeInOrder(Sector* node, vector<Sector*>& nodes) {
    if (node != nullptr) {
        storeInOrder(node->left, nodes);
        nodes.push_back(node);
        storeInOrder(node->right, nodes);
    }
}
Sector *SpaceSectorLLRBT::getSector(const string& sector_code) {
    for (Sector *sector: sector_list) {
        if (sector->sector_code == sector_code)
            return sector;
    }
    return nullptr;
}

Sector* SpaceSectorLLRBT::sortedArrayToBST(const vector<Sector*>& nodes, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    int mid = (start + end) / 2;
    Sector* root = nodes[mid];

    root->left = sortedArrayToBST(nodes, start, mid - 1);
    root->right = sortedArrayToBST(nodes, mid + 1, end);

    return root;
}

vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    vector<Sector*> path;
    Sector* end = getSector(sector_code);
    Sector* start = getSector("0SSS");
    findPath(start,end,path);

    return path;
}
void SpaceSectorLLRBT::findPath(Sector* start, Sector* end, std::vector<Sector*>& path) {
    std::vector<Sector*> pathToRootStart;
    std::vector<Sector*> pathToRootEnd;

    while (start != nullptr) {
        pathToRootStart.push_back(start);
        start = start->parent;
    }

    while (end != nullptr) {
        pathToRootEnd.push_back(end);
        end = end->parent;
    }

    std::reverse(pathToRootStart.begin(), pathToRootStart.end());
    std::reverse(pathToRootEnd.begin(), pathToRootEnd.end());

    size_t commonAncestorIndex = 0;
    while (commonAncestorIndex < pathToRootStart.size() &&
           commonAncestorIndex < pathToRootEnd.size() &&
           pathToRootStart[commonAncestorIndex] == pathToRootEnd[commonAncestorIndex]) {
        commonAncestorIndex++;
    }
    for (size_t i = 0; i < commonAncestorIndex; ++i) {
        path.push_back(pathToRootStart[i]);
    }
    for (size_t i = commonAncestorIndex; i < pathToRootEnd.size(); ++i) {
        path.push_back(pathToRootEnd[i]);
    }
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()) {
        cout << "A path to Dr. Elara could not be found." << endl;
    } else {
        cout << "The stellar path to Dr. Elara: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i]->sector_code;
            if (i < path.size() - 1) {
                cout << "->";
            }
        }
        cout <<endl;
        }
}