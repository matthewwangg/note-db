#include "cli.h"

#include <iostream>
#include <vector>
#include <string>

namespace cli {

void HandleNewCommand(const std::vector<std::string>& args) {
    std::cout << "New" << std::endl;
}

void HandleEditCommand(const std::vector<std::string>& args) {
    std::cout << "Edit" << std::endl;
}

void HandleDeleteCommand(const std::vector<std::string>& args) {
    std::cout << "Delete" << std::endl;
}

void HandleListCommand() {
    std::cout << "List" << std::endl;
}

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
        HandleNewCommand(args);
    } else if (command == "edit") {
        HandleEditCommand(args);
    } else if (command == "delete") {
        HandleDeleteCommand(args);
    } else if (command == "list") {
        HandleListCommand();
    } else if (command == "help") {
        HandleHelpCommand();
    } else {
        std::cout << "Unsupported command: " << command << "\n";
        HandleHelpCommand();
    }
}

}