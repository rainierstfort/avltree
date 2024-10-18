//
// Created by Rainier St. Fort on 9/30/24.
//

#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <vector>
#include <iostream>

// Class for storing student information
class Student {
public:
    std::string ID;
    std::string name;

    // Constructors
    Student();
    Student(const std::string& studID, const std::string& studName);

    // Operator overloads
    bool operator<(const Student& other) const;
    bool operator>(const Student& other) const;
    bool operator==(const Student& other) const;
};

// TreeNode class declaration
class TreeNode {
public:
    Student student;
    TreeNode* left;
    TreeNode* right;
    int height;

    TreeNode(const Student& stud);
};

// AVL class declaration
class AVL {
public:
    TreeNode* root;

    AVL();

    // Tree height and balancing
    int height(TreeNode* node);
    int getBalance(TreeNode* node);
    TreeNode* rightRotate(TreeNode* y);
    TreeNode* leftRotate(TreeNode* x);
    TreeNode* balanceNode(TreeNode* node);

    // Insertion
    void insert(const std::string& UFID, const std::string& name);
    TreeNode* insert(TreeNode* node, const Student& stud);

    // Validation
    bool isValidID(const std::string& UFID);
    bool isValidName(const std::string& name);
    bool isUniqueID(TreeNode* node, const std::string& UFID);

    // Deletion
    TreeNode* findInorderSuccessor(TreeNode* node);
    TreeNode* deleteNode(TreeNode* root, const std::string& UFID);
    void deleteNode(const std::string& UFID);

    // Searching
    TreeNode* searchByID(TreeNode* node, const std::string& UFID);
    void searchByID(const std::string& UFID);
    TreeNode* searchByName(TreeNode* node, const std::string& name);
    void searchByName(const std::string& name);

    // Collection
    void collectUFIDs(TreeNode* node, std::vector<std::string>& UFIDs);
    void deleteNthInorder(int N);

    // Traversal funcs
    void inOrder(TreeNode* node, bool& first);
    void printInOrder();
    void preOrder(TreeNode* node, bool& first);
    void printPreOrder();
    void postOrder(TreeNode* node, bool& first);
    void printPostOrder();

    int printLevelCount();

};

#endif //CLASS_H
