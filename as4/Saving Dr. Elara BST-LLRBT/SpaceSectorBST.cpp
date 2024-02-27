#include "SpaceSectorBST.h"
#include "Sector.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {

}

SpaceSectorBST::~SpaceSectorBST() {
    clearTree(root);  // Call a helper function to delete all nodes and release memory
    sector_list.clear();
    leaf_list.clear();
}

// Helper function to perform a post-order traversal and delete nodes
void SpaceSectorBST::clearTree(Sector* currentNode) {
    if (currentNode != nullptr) {
        // Recursively delete the left and right subtrees
        clearTree(currentNode->left);
        clearTree(currentNode->right);

        // Delete the current node
        delete currentNode;
    }
}

vector<string> SpaceSectorBST::customSplit(string &str, char separator) {
    vector<string> result;
    istringstream ss(str);
    string token;

    while (getline(ss, token, separator)) {
        result.push_back(token);
    }

    return result;
}

void SpaceSectorBST::readSectorsFromFile(const std::string &filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    fstream sectors_file(filename);
    string coordinate_line;
    while (getline(sectors_file, coordinate_line)) {
        vector<string> coordinate;
        coordinate = customSplit(coordinate_line, ',');
        if (coordinate[0] == "X") continue;
        else {
            Sector *sector = new Sector(stoi(coordinate[0]), stoi(coordinate[1]), stoi(coordinate[2]));
            sector_list.push_back(sector);
        }
    }
    for (Sector *sector: sector_list) {
        insertSectorByCoordinates(sector->x, sector->y, sector->z);
    }
    // according to the given comparison critera based on the sector coordinates.
    sectors_file.close();
}

Sector *SpaceSectorBST::createSectorLeaf(int x, int y, int z) {
    Sector *sector = new Sector(x, y, z);
    leaf_list.push_back(sector);
    sector->left = NULL;
    sector->right = NULL;
    return sector;
}

void SpaceSectorBST::addSectorPrivate(int x, int y, int z, Sector *&current) {
    if (current == nullptr) {
        current = createSectorLeaf(x, y, z);
    } else if (x < current->x || (x == current->x && y < current->y) ||
               (x == current->x && y == current->y && z < current->z)) {
        addSectorPrivate(x, y, z, current->left);
        current->left->parent = current; // Assign the parent pointer
    } else if (x > current->x || (x == current->x && y > current->y) ||
               (x == current->x && y == current->y && z > current->z)) {
        addSectorPrivate(x, y, z, current->right);
        current->right->parent = current; // Assign the parent pointer
    }
    // Note: If (x, y, z) is equal to the current node, you might want to decide what to do (ignore, update, etc.).
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    addSectorPrivate(x, y, z, root);
}

Sector *SpaceSectorBST::getSector(string sector_code) {
    for (Sector *sector: sector_list) {
        if (sector->sector_code == sector_code)
            return sector;
    }
    return nullptr;
}

void SpaceSectorBST::deleteSector(const string &sector_code) {
    Sector *sectorToDelete = getSector(sector_code);
    if(sectorToDelete != nullptr){
        deleteItem(root, sectorToDelete->x, sectorToDelete->y, sectorToDelete->z);
    }
}

void SpaceSectorBST::deleteItem(Sector*&treePtr, int x, int y, int z) {
    if (treePtr == nullptr) //empty tree
        cout << "Deletion unsuccessful. Root is empty." << endl;

    else if (x == treePtr->x && y == treePtr->y && z == treePtr->z) //root=nodetodel
        deleteNodeItem(treePtr);

    else if ((x < treePtr->x) || (x == treePtr->x && y < treePtr->y) ||
             (x == treePtr->x && y == treePtr->y && z < treePtr->z))
        deleteItem(treePtr->left, x, y, z);

    else
        deleteItem(treePtr->right, x, y, z);
}

/*void SpaceSectorBST::deleteNode(Sector *&node) {
    Sector *sectorToDelete;
    string new_sector_code;

    if (node->left == nullptr && node->right == nullptr) {
        delete node;
        node = nullptr;
    } else if (node->left == nullptr) {
        sectorToDelete = node;
        node = node->right;
        sectorToDelete->right = nullptr;
        delete sectorToDelete;
    }
}*/

void SpaceSectorBST::deleteNodeItem(Sector *&nodeptr) {
    Sector *ptrToDelete;
    Sector *replaceWith;
    replacement_list.push_back(replaceWith);

    std::cout << " asd " << std::endl;

    if ((nodeptr->left == nullptr) && (nodeptr->right == nullptr)) { // leaf
        delete nodeptr;
        nodeptr = nullptr;

    } else if (nodeptr->left == nullptr) {
        ptrToDelete = nodeptr;
        nodeptr = nodeptr->right;
        ptrToDelete->right = nullptr;
        delete ptrToDelete;

    } else if (nodeptr->right == nullptr) {
        ptrToDelete = nodeptr;
        nodeptr = nodeptr->left;
        ptrToDelete->left = nullptr;
        delete ptrToDelete;

    } else {
        //int indexOfNode = findInList(inorder_list, nodeptr, 0);
        //replaceWith = *inorder_list[indexOfNode + 1];
        processLeftmost(nodeptr->right, nodeptr);
    }
}


int SpaceSectorBST::findInList(vector<Sector *> list, Sector *sector, int a) {
    if (list[a] == sector)
        return a;
    else {
        findInList(list, sector, a + 1);
        return 0;
    }
}

void SpaceSectorBST::processLeftmost(Sector *&nodeptr, Sector *&treeNode) {
    if (nodeptr->left == nullptr) {
        treeNode->sector_code = nodeptr->sector_code; //caution
        treeNode->x = nodeptr->x;
        treeNode->y = nodeptr->y;
        treeNode->z = nodeptr->z;
        treeNode->distance_from_earth = nodeptr->distance_from_earth;
        treeNode->parent = nodeptr->parent;

        Sector *delPtr = nodeptr;
        nodeptr = nodeptr->right;
        delPtr->right = nullptr;
        delete delPtr;
    } else
        processLeftmost(nodeptr->left, treeNode);
}

void SpaceSectorBST::displaySectorsInOrder() {
    cout << "Space sectors inorder traversal:" << endl;
    inorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:" << endl;
    preorder(root);
    cout << endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    cout << "Space sectors postorder traversal:" << endl;
    postorder(root);
    cout << endl;
}

void SpaceSectorBST::preorder(Sector *treePtr) {
    if (treePtr != nullptr) {
        cout << treePtr->sector_code << endl;
        preorder(treePtr->left);
        preorder(treePtr->right);
    }
}

void SpaceSectorBST::inorder(Sector *treePtr) {
    if (treePtr != nullptr) {
        inorder(treePtr->left);
        cout << treePtr->sector_code << endl;
        inorder_list.push_back(treePtr);
        inorder(treePtr->right);
    }
}

void SpaceSectorBST::postorder(Sector *treePtr) {
    if (treePtr != nullptr) {
        postorder(treePtr->left);
        postorder(treePtr->right);
        cout << treePtr->sector_code << endl;
    }
}

vector<Sector *> SpaceSectorBST::getStellarPath(const string &sector_code) {
    vector<Sector *> path;

    // Check if the tree is empty or if target_sector is not found
    if (root == nullptr) {
        return path;
    }

    Sector *target_sector = getSector(sector_code);

    if (target_sector == nullptr) {
        return path;
    }

    Sector *current = root;

    while (current != nullptr && (current->x != target_sector->x || current->sector_code != sector_code)) {
        path.push_back(current);

        if (target_sector->x < current->x || (target_sector->x == current->x && sector_code < current->sector_code)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Check if the target sector was found
    if (current == nullptr) {
        return path;
    }

    path.push_back(current);

    return path;
}

void SpaceSectorBST::printStellarPath(const vector<Sector *> &path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    if (path.empty()) {
        cout << "A path to Dr. Elara could not be found.";
    } else {
        cout << "The stellar path to Dr. Elara: ";
        for (const Sector *sector: path) {
            cout << sector->sector_code;
            if (sector != path.back()) {
                cout << "->";
            }
        }
    }
    cout<<endl;
}