#include "cli.h"

#include <iostream>
#include <vector>
#include <string>

namespace cli {

void HandleHelpCommand() {
    std::cout << "Available commands:" << std::endl
              << "  new     Create a new note" << std::endl
              << "  edit    Edit an existing note" << std::endl
              << "  delete  Delete a note" << std::endl
              << "  list    List all notes" << std::endl
              << "  help    Show this help message" << std::endl;
}

void DispatchCommand(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        HandleHelpCommand();
        return;
    }

    const std::string& command = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    if (command == "new") {
        // Handle new command
    } else if (command == "edit") {
        // Handle edit command
    } else if (command == "delete") {
        // Handle delete command
    } else if (command == "list") {
        // Handle list command
    } else if (command == "help") {
        HandleHelpCommand();
    } else {
        std::cout << "Unsupported command: " << command << "\n";
        HandleHelpCommand();
    }
}

}