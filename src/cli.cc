#include "cli.h"

#include <iostream>
#include <vector>
#include <string>

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    manager.CreateNote(args);
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    manager.EditNote(args);
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    manager.DeleteNote(args);
}

void HandleListCommand(NoteManager& manager) {
    manager.ListNotes();
}

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    manager.SearchNote(args);
}

void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    manager.TagNote(args);
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


void DispatchCommand(const std::vector<std::string>& command_args) {
    if (command_args.empty()) {
        HandleHelpCommand();
        return;
    }

    NoteManager manager("notes");

    const std::string& command = command_args[0];
    std::vector<std::string> args(command_args.begin() + 1, command_args.end());

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