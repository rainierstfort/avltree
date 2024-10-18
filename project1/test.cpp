/*#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>


// uncomment and replace the following with your own headers


#include "class.h"


// you must write 5 unique, meaningful tests for credit on the testing portion of this project!

// the provided test from the template is below.

// NEEDED TO LOOK UP LOGIC FOR THIS FUNCTION TO CATCH THE COUT OUTPUTS USED IN MY CODE, FOUND SOURCES TO HELP LIKE GEEKFORGEEKS/CPPREFERENCES
std::string captureOutput(void(*func)(AVL&), AVL& tree) {

    std::ostringstream buffer;
    std::streambuf* prevCoutBuf = std::cout.rdbuf(buffer.rdbuf()); // Redirects std::cout to buffer

    func(tree);  // Calls the provided function with the AVL tree

    std::cout.rdbuf(prevCoutBuf); // Restores original std::cout
    return buffer.str(); // Returns the captured output
}

// Helper func to check balance
bool isBalanced(TreeNode* node) {
    if (node == nullptr) {
        return true;
    }
    int balance = std::abs(node->left ? node->left->height : 0 - (node->right ? node->right->height : 0));
    return balance <= 1 && isBalanced(node->left) && isBalanced(node->right);
}

// Wrapper func to call printInOrder() inside captureOutput func
void callPrintInOrder(AVL& tree) {
    tree.printInOrder();
}

// Helper function to search for an ID in AVL and call searchByID
void searchByIDHelper(AVL& tree) {
    tree.searchByID("00000005"); //Specifically using 00000005 to show the test case
}

// Helper function to perform an inorder traversal and collect the IDs
void collectInOrder(TreeNode* node, std::vector<std::string>& ids) {
    if (node == nullptr) return;
    collectInOrder(node->left, ids);
    ids.push_back(node->student.ID);
    collectInOrder(node->right, ids);
}

// Test case
TEST_CASE("AVL Tree command tests", "[AVL]") {
	AVL tree;

	SECTION("Invalid insert commands") {
		// Invalid name with numbers
		std::string output = captureOutput([](AVL& t) {
			t.insert("45679999","A11y");
		}, tree);
		REQUIRE(output == "unsuccessful\n");



        // Invalid UFID (less than 8 digits)
        output = captureOutput([](AVL& t) {
            t.insert("1234567","Rainier");
        }, tree);
        REQUIRE(output == "unsuccessful\n");

        // Invalid UFID (more than 8 digits)
        output = captureOutput([](AVL& t) {
            t.insert("123456789","Bye");
        }, tree);
        REQUIRE(output == "unsuccessful\n");

        // Name with special characters
        output = captureOutput([](AVL& t) {
            t.insert("45679999","Jessica!");
        }, tree);
        REQUIRE(output == "unsuccessful\n");

        // Empty name
        output = captureOutput([](AVL& t) {
            t.insert("45679999","");
        }, tree);
        REQUIRE(output == "unsuccessful\n");
    }
}

TEST_CASE("Edge cases tests", "[AVL]") {
    AVL tree;

    SECTION("Edge Case 1: Delete a node that doesn't exist") {
        // Insert some nodes into the tree first
        tree.insert("12345678","John Doe");
        tree.insert("23456789","Jane Doe");

        // Attempt to delete a node with a UFID that doesn't exist
        std::string output = captureOutput([](AVL& t) {
            t.deleteNode("98765432");  // This UFID doesn't exist
        }, tree);
        REQUIRE(output == "unsuccessful\n");
    }

    SECTION("Edge Case 2: Insert a duplicate GatorID") {
        // Insert a node
        tree.insert("12345678","John Doe");

        // Attempt to insert a duplicate UFID
        std::string output = captureOutput([](AVL& t) {
            t.insert("12345678","Jane Doe");  // Duplicate UFID
        }, tree);
        REQUIRE(output == "unsuccessful\n");
    }

    SECTION("Edge Case 3: Search for a node in an empty tree", "[AVL]") {
        AVL emptyTree;  // A new empty AVL tree

        // Attempt to search for a UFID in an empty tree
        std::string output = captureOutput([](AVL& t) {
            t.searchByID("12345678");  // Searching in an empty tree
        }, emptyTree);
        REQUIRE(output == "unsuccessful\n");
    }
}

TEST_CASE("AVL Tree Rotations", "[AVL]") {

    AVL tree;

    SECTION("Left-Left Rotation Case") {
        // Insert nodes that will cause a left-left imbalance
        tree.insert("30000000", "C");  // Root
        tree.insert("20000000", "B");  // Left child of root
        tree.insert("10000000", "A");  // Left child of left child

        // The tree should trigger a single right rotation on node 30
        REQUIRE(isBalanced(tree.root));  // Ensure tree is balanced after rotation
        REQUIRE(tree.root->student.ID == "20000000");  // Root should be B (20) after rotation
    }
    SECTION("Left-Right Rotation Case") {
        // Insert nodes that will cause a left-right imbalance
        tree.insert("30000000", "C");  // Root
        tree.insert("10000000", "A");  // Left child of root
        tree.insert("20000000", "B");  // Right child of left child

        // The tree should trigger a left-right rotation (left rotate A, then right rotate C)
        REQUIRE(isBalanced(tree.root));  // Ensure tree is balanced after rotation
        REQUIRE(tree.root->student.ID == "20000000");  // Root should be B (20) after rotation
    }

    SECTION("Right-Right Rotation Case") {
        // Insert nodes that will cause a right-right imbalance
        tree.insert("10000000", "A");  // Root
        tree.insert("20000000", "B");  // Right child of root
        tree.insert("30000000", "C");  // Right child of right child

        // The tree should trigger a single left rotation on node 10
        REQUIRE(isBalanced(tree.root));  // Ensure tree is balanced after rotation
        REQUIRE(tree.root->student.ID == "20000000");  // Root should be B (20) after rotation
    }

    SECTION("Right-Left Rotation Case") {
        // Insert nodes that will cause a right-left imbalance
        tree.insert("10000000", "A");  // Root
        tree.insert("30000000", "C");  // Right child of root
        tree.insert("20000000", "B");  // Left child of right child

        // The tree should trigger a right-left rotation (right rotate C, then left rotate A)
        REQUIRE(isBalanced(tree.root));  // Ensure tree is balanced after rotation
        REQUIRE(tree.root->student.ID == "20000000");  // Root should be B (20) after rotation
    }
}

TEST_CASE("Delete node with no children (leaf node)", "[AVL]") {
    AVL tree;

    // Insert nodes
    tree.insert("00000010", "StudentTEN");
    tree.insert("00000005", "StudentFIVE");
    tree.insert("00000020", "StudentTWENTY");

    // Delete leaf node (Student5)
    tree.deleteNode("00000005");

    // Capture the output when searching for the deleted node
    std::string output = captureOutput(searchByIDHelper, tree);

    // Verify the deletion was successful by checking the output
    REQUIRE(output == "unsuccessful\n");

    // Verify the tree structure
    REQUIRE(tree.root->student.ID == "00000010");
    REQUIRE(tree.root->left == nullptr);
    REQUIRE(tree.root->right->student.ID == "00000020");

    // Ensure the tree is balanced
    REQUIRE(isBalanced(tree.root) == true);
}

// Test case for deletion of a node with one child
TEST_CASE("Delete node with one child", "[AVL]") {
    AVL tree;

    // Insert nodes
    tree.insert("00000010", "StudentTEN");
    tree.insert("00000005", "StudentFIVE");
    tree.insert("00000001", "StudentONE");  // Left child of Student5

    // Delete node with one child (Student5)
    tree.deleteNode("00000005");

    std::string output = captureOutput(searchByIDHelper, tree);

    REQUIRE(output == "unsuccessful\n");

    // Verify the tree structure (Student1 should take place of Student5)
    REQUIRE(tree.root->left->student.ID == "00000001");
    REQUIRE(tree.root->left->left == nullptr);
    REQUIRE(tree.root->left->right == nullptr);

    REQUIRE(isBalanced(tree.root) == true);
}

// Test case for deletion of a node with two children
TEST_CASE("Delete node with two children", "[AVL]") {
    AVL tree;

    // Insert nodes
    tree.insert("00000020", "StudentTWENTY");
    tree.insert("00000010", "StudentTEN");
    tree.insert("00000030", "StudentTHIRTY");
    tree.insert("00000025", "StudentTWENTYFIVE");  // Left child of Student30
    tree.insert("00000035", "StudentTHIRTYFIVE");  // Right child of Student30

    // Delete node with two children (Student30)
    tree.deleteNode("00000030");

    std::string output = captureOutput(searchByIDHelper, tree);

    REQUIRE(output == "unsuccessful\n");

    REQUIRE(tree.root->right->student.ID == "00000035");
    REQUIRE(tree.root->right->left->student.ID == "00000025");
    REQUIRE(tree.root->right->right == nullptr);

    REQUIRE(isBalanced(tree.root) == true);
}*/