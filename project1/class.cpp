//
// Created by Rainier St. Fort on 10/1/24.
//

#include "class.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>

Student::Student() {}

Student::Student(const std::string& studID, const std::string& studName) {
    ID = studID;
    name = studName;
}

bool Student::operator<(const Student& other) const {
    return ID < other.ID;
}

bool Student::operator>(const Student& other) const {
    return ID > other.ID;
}

bool Student::operator==(const Student& other) const {
    return ID == other.ID;
}

// TreeNode constructor
TreeNode::TreeNode(const Student& stud) : student(stud), left(nullptr), right(nullptr), height(1) {}

// AVL class implementations

AVL::AVL() {
    root = nullptr;
}

int AVL::height(TreeNode* node) {
    return node ? node->height : 0;
}

int AVL::getBalance(TreeNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

TreeNode* AVL::rightRotate(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;

    return x;
}

TreeNode* AVL::leftRotate(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;

    return y;
}

TreeNode* AVL::balanceNode(TreeNode* node) {
    node->height = std::max(height(node->left), height(node->right)) + 1;
    int balance = getBalance(node);

    // Left-Left case
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }

    // Left-Right case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right-Right case
    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }

    // Right-Left case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // No rotation needed
}

// ID validation
bool AVL::isValidID(const std::string& UFID) {
    std::regex idRegex("^\\d{8}$");
    return std::regex_match(UFID, idRegex);
}

// Validates name (only alphabets and spaces)
bool AVL::isValidName(const std::string& name) {
    std::regex nameRegex("^[A-Za-z ]+$");
    return std::regex_match(name, nameRegex);
}

// Checks if UFID is unique
bool AVL::isUniqueID(TreeNode* node, const std::string& UFID) {
    if (node == nullptr) {
        return true;
    }
    if (node->student.ID == UFID) {
        return false;
    }
    if (UFID < node->student.ID) {
        return isUniqueID(node->left, UFID);
    } else {
        return isUniqueID(node->right, UFID);
    }
}

// Inserts a Student into the AVL tree
TreeNode* AVL::insert(TreeNode* node, const Student& stud) {
    if (node == nullptr) {
        return new TreeNode(stud);
    }
    if (stud < node->student) {
        node->left = insert(node->left, stud);
    } else if (stud > node->student) {
        node->right = insert(node->right, stud);
    } else {
        std::cout << "unsuccessful" << std::endl;
        return node;
    }
    return balanceNode(node);
}

void AVL::insert(const std::string& UFID, const std::string& name) {
    if (!isValidID(UFID) || !isValidName(name) || !isUniqueID(root, UFID)) {
        std::cout << "unsuccessful" << std::endl;
        return;
    }
    root = insert(root, Student(UFID, name));
    std::cout << "successful" << std::endl;
}

// Finds the inorder successor (smallest node in the right subtree)
TreeNode* AVL::findInorderSuccessor(TreeNode* node) {
    TreeNode* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

// Delete a node with the given UFID
TreeNode* AVL::deleteNode(TreeNode* root, const std::string& UFID) {
    if (root == nullptr) {
        return root;  // Node not found
    }
    if (UFID < root->student.ID) {
        root->left = deleteNode(root->left, UFID);
    } else if (UFID > root->student.ID) {
        root->right = deleteNode(root->right, UFID);
    } else {
        // Node with only one child or no child
        if (root->left == nullptr) {
            TreeNode* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            TreeNode* temp = root->left;
            delete root;
            return temp;
        }
        // Node with two children
        TreeNode* temp = findInorderSuccessor(root->right);
        root->student = temp->student;
        root->right = deleteNode(root->right, temp->student.ID);
    }
    return balanceNode(root);
}

void AVL::deleteNode(const std::string& UFID) {
    if (searchByID(root, UFID) == nullptr) {
        std::cout << "unsuccessful" << std::endl;
    } else {
        root = deleteNode(root, UFID);
        std::cout << "successful" << std::endl;
    }
}

TreeNode* AVL::searchByID(TreeNode* node, const std::string& UFID) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->student.ID == UFID) {
        return node;
    }
    if (UFID < node->student.ID) {
        return searchByID(node->left, UFID);
    } else {
        return searchByID(node->right, UFID);
    }
}

void AVL::searchByID(const std::string& UFID) {
    TreeNode* result = searchByID(root, UFID);
    if (result == nullptr) {
        std::cout << "unsuccessful" << std::endl;
    } else {
        std::cout << result->student.name << std::endl;
    }
}

TreeNode* AVL::searchByName(TreeNode* node, const std::string& name) {
    if (node == nullptr) {
        return nullptr; // Base case: return nullptr if the node is null
    }

    // Check the current node
    if (node->student.name == name) {
        return node;
    }

    // Searchs in the left and right subtrees
    TreeNode* leftResult = searchByName(node->left, name);
    TreeNode* rightResult = searchByName(node->right, name);

    // Returns the first found node in either subtree
    return leftResult != nullptr ? leftResult : rightResult;
}

// Function to search for a student by name and collect all associated UFIDs
void AVL::searchByName(const std::string& name) {
    std::vector<std::string> UFIDs;
    collectUFIDs(root, UFIDs);

    // Prints all UFIDs that match the specified name
    bool found = false; // Flags to check if any UFID was printed
    for (const auto& id : UFIDs) {
        // Searching for the name using the UFID to find the corresponding student
        TreeNode* node = searchByID(root, id);
        if (node != nullptr && node->student.name == name) {
            std::cout << id << std::endl; // Printing each matching UFID
            found = true; // Setting flag to true since a match was found
        }
    }

    if (!found) {
        std::cout << "unsuccessful" << std::endl;
    }
}

// Collect UFIDs
void AVL::collectUFIDs(TreeNode* node, std::vector<std::string>& UFIDs) {
    if (node == nullptr) return;
    collectUFIDs(node->left, UFIDs);
    UFIDs.push_back(node->student.ID);
    collectUFIDs(node->right, UFIDs);
}

// Delete the Nth inorder student
void AVL::deleteNthInorder(int N) {
    std::vector<std::string> UFIDs;
    collectUFIDs(root, UFIDs);

    if (N < 0 || N >= UFIDs.size()) {
        std::cout << "unsuccessful" << std::endl;
        return;
    }
    std::string UFID = UFIDs[N];
    deleteNode(UFID);
    //std::cout << "successful" << std::endl;
}

void AVL::inOrder(TreeNode* node, bool& first) {
    if (node != nullptr) {
        inOrder(node->left, first); // Traverse the left subtree

        // Print the current student's name
        if (first) {
            std::cout << node->student.name; // Print without a comma for the first element
            first = false; // Set first to false after printing the first name
        } else {
            std::cout << ", " << node->student.name; // Print with a comma for subsequent elements
        }

        inOrder(node->right, first); // Traverse the right subtree
    }
}

void AVL::printInOrder() {
    bool first = true;
    inOrder(root, first); // Start the in-order traversal
    std::cout << std::endl;
}

void AVL::preOrder(TreeNode* node, bool& first) {
    if (node != nullptr) {

        if (first) {
            std::cout << node->student.name;
            first = false;
        } else {
            std::cout << ", " << node->student.name;
        }

        preOrder(node->left, first);
        preOrder(node->right, first);
    }
}

void AVL::printPreOrder() {
    bool first = true;
    preOrder(root, first); // Start the pre-order traversal
    std::cout << std::endl;
}

void AVL::postOrder(TreeNode* node, bool& first) {
    if (node != nullptr) {
        postOrder(node->left, first); // Traverse the left subtree
        postOrder(node->right, first); // Traverse the right subtree

        // Print the current student's name
        if (first) {
            std::cout << node->student.name;
            first = false;
        } else {
            std::cout << ", " << node->student.name;
        }
    }
}

void AVL::printPostOrder() {
    bool first = true;
    postOrder(root, first);
    std::cout << std::endl;
}


int AVL::printLevelCount() {
    if (root == nullptr) {
        return 0; // If the tree is empty, return 0 levels
    }
    return root->height; // height == levels
}