#include "cli.h"

#include <iostream>
#include <vector>
#include <string>

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    std::cout << "New" << std::endl;
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    std::cout << "Edit" << std::endl;
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    std::cout << "Delete" << std::endl;
}

void HandleListCommand(NoteManager& manager) {
    std::cout << "List" << std::endl;
}

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    std::cout << "Search" << std::endl;
}

void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    std::cout << "Tag" << std::endl;
}

void HandleHelpCommand() {
    std::cout << "Available commands:" << std::endl
              << "  new <filename>           Create a new note" << std::endl
              << "  edit <filename>          Edit a note" << std::endl
              << "  delete <filename>        Delete a note" << std::endl
              << "  list                     List all notes" << std::endl
              << "  search <query>           Search note contents" << std::endl
              << "  tag <filename> <tag>     Add a tag to a note" << std::endl
              << "  help                     Show this message" << std::endl;
}


void DispatchCommand(NoteManager& manager, const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        HandleHelpCommand();
        return;
    }

    const std::string& command = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    if (command == "new") {
        HandleNewCommand(manager, args);
    } else if (command == "edit") {
        HandleEditCommand(manager, args);
    } else if (command == "delete") {
        HandleDeleteCommand(manager, args);
    } else if (command == "list") {
        HandleListCommand(manager);
    } else if (command == "search") {
        HandleSearchCommand(manager, args);
    } else if (command == "tag") {
        HandleTagCommand(manager, args);
    } else if (command == "help") {
        HandleHelpCommand();
    } else {
        std::cout << "Unsupported command: " << command << "\n";
        HandleHelpCommand();
    }
}

}