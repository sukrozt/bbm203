#include "SpaceSectorRBT.h"
#include <algorithm>

using namespace std;

SpaceSectorRBT::SpaceSectorRBT() : root(nullptr) {

}

void SpaceSectorRBT::freeTree(Sector *node) {
    if (node != nullptr) {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}

vector <string> SpaceSectorRBT::customSplit(string &str, char separator) {
    vector <string> result;
    istringstream ss(str);
    string token;

    while (getline(ss, token, separator)) {
        result.push_back(token);
    }

    return result;
}

void SpaceSectorRBT::readSectorsFromFile(const string &filename) {
    ifstream sectors_file(filename);
    string coordinate_line;

    while (getline(sectors_file, coordinate_line)) {
        vector <string> coordinate = customSplit(coordinate_line, ',');
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

SpaceSectorRBT::~SpaceSectorRBT() {
    freeTree(root);
}

void SpaceSectorRBT::balance(Sector *sector) {
    if (sector == root)
        sector->color = BLACK;
    else {
        if (sector->parent != nullptr && sector->parent->color == RED) {
            Sector *parent = sector->parent;
            Sector *grandparent = parent->parent;
            if (grandparent != nullptr) {
                Sector *uncle = nullptr;
                if (grandparent->left == parent) {
                    uncle = grandparent->right;
                } else {
                    uncle = grandparent->left;
                }
                if (uncle != nullptr && uncle->color == RED) {
                    flipColors(parent, uncle, grandparent);
                    balance(grandparent);
                } else {
                    if (parent->left == sector && grandparent->right == parent) {
                        rightRotate(parent);
                        leftRotate(grandparent);
                        bool tempColor = sector->color;
                        sector->color = grandparent->color;
                        grandparent->color = tempColor;
                    } else if (parent->left == sector && grandparent->left == parent) {
                        rightRotate(grandparent);
                        bool tempColor = parent->color;
                        parent->color = grandparent->color;
                        grandparent->color = tempColor;
                    } else if (parent->right == sector && grandparent->left == parent) {
                        leftRotate(parent);
                        rightRotate(grandparent);
                        bool tempColor = parent->color;
                        sector->color = grandparent->color;
                        grandparent->color = tempColor;
                    } else if (parent->right == sector && grandparent->right == parent) {
                        leftRotate(grandparent);
                        bool tempColor = parent->color;
                        parent->color = grandparent->color;
                        grandparent->color = tempColor;
                    }
                }
            }
        }
    }
}

void SpaceSectorRBT::insertSectorByCoordinates(int x, int y, int z) {
    Sector *sector = new Sector(x, y, z);
    sector->color = RED;
    sector->left = nullptr;
    sector->right = nullptr;
    sector_list.push_back(sector);
    if (root == nullptr) {
        root = sector;
        balance(root);
    } else {
        Sector *sec = insertSectorRecursive(root, x, y, z, sector); //last added leaf
        balance(sector);
    }
}

Sector *SpaceSectorRBT::insertSectorRecursive(Sector *node, int x, int y, int z, Sector *sector) {
    if (node == nullptr) {
        return sector;
    }
    if (x < node->x || (x == node->x && y < node->y) || (x == node->x && y == node->y && z < node->z)) {
        node->left = insertSectorRecursive(node->left, x, y, z, sector);
        node->left->parent = node;
    } else if (x > node->x || (x == node->x && y > node->y) || (x == node->x && y == node->y && z > node->z)) {
        node->right = insertSectorRecursive(node->right, x, y, z, sector);
        node->right->parent = node;
    }

    return node;
}


void SpaceSectorRBT::leftRotate(Sector *node) {
    Sector *x = node->right;
    Sector *y = x->left;
    x->left = node;
    node->right = y;
    x->parent = node->parent;
    if (x->parent == nullptr)
        root = x;
    else if (node == node->parent->left)
        node->parent->left = x;
    else
        node->parent->right = x;
    node->parent = x;
    if (y != nullptr)
        y->parent = node;
}

void SpaceSectorRBT::rightRotate(Sector *node) {
    Sector *x = node->left;
    Sector *y = x->right;
    x->right = node;
    node->left = y;
    x->parent = node->parent;
    if (x->parent == nullptr)
        root = x;
    else if (node == node->parent->left)
        node->parent->left = x;
    else
        node->parent->right = x;
    node->parent = x;
    if (y != nullptr)
        y->parent = node;
}

void SpaceSectorRBT::flipColors(Sector *parent,
                                Sector *uncle, Sector *grandparent) {
    parent->color = BLACK;
    uncle->color = BLACK;
    grandparent->color = RED;
}

bool SpaceSectorRBT::isRed(Sector *node) {
    return (node != nullptr) && (node->color == RED);
}

void SpaceSectorRBT::displaySectorsInOrder() {
    cout << "Space sectors inorder traversal:" << endl;
    displaySectorsInOrderRecursive(root);
    cout << endl;
}

void SpaceSectorRBT::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:" << endl;
    displaySectorsPreOrderRecursive(root);
    cout << endl;
}

void SpaceSectorRBT::displaySectorsPostOrder() {
    cout << "Space sectors postorder traversal:" << endl;
    displaySectorsPostOrderRecursive(root);
    cout << endl;
}

void SpaceSectorRBT::displaySectorsInOrderRecursive(Sector *node) {
    if (node != nullptr) {
        displaySectorsInOrderRecursive(node->left);
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
        displaySectorsInOrderRecursive(node->right);
    }
}

void SpaceSectorRBT::displaySectorsPreOrderRecursive(Sector *node) {
    if (node != nullptr) {
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
        displaySectorsPreOrderRecursive(node->left);
        displaySectorsPreOrderRecursive(node->right);
    }
}

void SpaceSectorRBT::displaySectorsPostOrderRecursive(Sector *node) {
    if (node != nullptr) {
        displaySectorsPostOrderRecursive(node->left);
        displaySectorsPostOrderRecursive(node->right);
        cout << (node->color ? "RED" : "BLACK") << " sector: " << node->sector_code << endl;
    }
}

void SpaceSectorRBT::balanceTree() {
    vector < Sector * > nodes;
    storeInOrder(root, nodes);

    root = nullptr;

    root = sortedArrayToBST(nodes, 0, nodes.size() - 1);

    if (root != nullptr) {
        root->color = BLACK;
    }
}

void SpaceSectorRBT::storeInOrder(Sector *node, vector<Sector *> &nodes) {
    if (node != nullptr) {
        storeInOrder(node->left, nodes);
        nodes.push_back(node);
        storeInOrder(node->right, nodes);
    }
}

Sector *SpaceSectorRBT::getSector(const string &sector_code) {
    for (Sector *sector: sector_list) {
        if (sector->sector_code == sector_code)
            return sector;
    }
    return nullptr;
}

Sector *SpaceSectorRBT::sortedArrayToBST(const vector<Sector *> &nodes, int start, int end) {
    if (start > end) {
        return nullptr;
    }

    int mid = (start + end) / 2;
    Sector *root = nodes[mid];

    root->left = sortedArrayToBST(nodes, start, mid - 1);
    root->right = sortedArrayToBST(nodes, mid + 1, end);

    return root;
}

vector<Sector *> SpaceSectorRBT::getStellarPath(const std::string &sector_code) {
    vector < Sector * > path;
    Sector *end = getSector(sector_code);
    Sector *start = getSector("0SSS");
    findPath(start, end, path);

    return path;
}

void SpaceSectorRBT::findPath(Sector *start, Sector *end, std::vector<Sector *> &path) {
    std::vector < Sector * > pathToRootStart;
    std::vector < Sector * > pathToRootEnd;

    while (start != nullptr) {
        //cout << "debug " << start->sector_code << endl;
        pathToRootStart.push_back(start);
        start = start->parent;
    }
    while (end != nullptr) {
        //cout << "debug " << end->sector_code << endl;
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

void SpaceSectorRBT::printStellarPath(const std::vector<Sector *> &path) {
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
        cout << endl;
    }
}