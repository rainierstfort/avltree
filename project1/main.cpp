#include <iomanip>
#include <iostream>
#include <string>
#include "class.h"
#include <sstream>

int main() {
    AVL tree;
    int numCommands;
    std::string inputLine;

    // Read number of commands
    std::getline(std::cin, inputLine);
    numCommands = std::stoi(inputLine);

    for (int i = 0; i < numCommands; ++i) {
        std::getline(std::cin, inputLine);
        std::istringstream iss(inputLine);
        std::string command;
        iss >> command;

        if (command == "insert") {
            std::string name;
            std::string UFID;

            // Parse the quoted name
            iss >> std::quoted(name);

            // Get the UFID
            iss >> UFID;

            if (tree.isValidID(UFID) && tree.isValidName(name) && tree.isUniqueID(tree.root, UFID)) {
                tree.insert(UFID, name);
            } else {
                std::cout << "unsuccessful" << std::endl;
            }

        } else if (command == "search") {
            std::string param;
            iss >> param;

            // If search by name (check for quotes)
            if (param.front() == '"') {
                std::string name;
                iss.clear();
                iss.str(inputLine);
                std::getline(iss, name, '"');
                std::getline(iss, name, '"'); // Extracts the actual name between quotes

                tree.searchByName(name);

            } else {
                // Otherwise it's a search by UFID
                tree.searchByID(param);
            }

        } else if (command == "remove") {
            std::string UFID;
            iss >> UFID;
            tree.deleteNode(UFID);

        } else if (command == "removeInorder") {
            int N;
            iss >> N;
            tree.deleteNthInorder(N);

        } else if (command == "printInorder") {
            tree.printInOrder();

        } else if (command == "printPreorder") {
            tree.printPreOrder();

        } else if (command == "printPostorder") {
            tree.printPostOrder();

        } else if (command == "printLevelCount") {
            std::cout << tree.printLevelCount() << std::endl;

        } else {
            std::cout << "Invalid command!" << std::endl;
        }
    }

    return 0;
}

