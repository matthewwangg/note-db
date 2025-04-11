#include "cli.h"
#include "utils/display_utils.h"

#include <iostream>
#include <vector>
#include <string>

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command new!" << std::endl;
        std::cout << "Proper Usage: new <filename>" << std::endl;
        return;
    }
    manager.CreateNote(args);
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command edit!" << std::endl;
        std::cout << "Proper Usage: edit <filename>" << std::endl;
        return;
    }
    manager.EditNote(args);
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command delete!" << std::endl;
        std::cout << "Proper Usage: delete <filename>" << std::endl;
        return;
    }
    manager.DeleteNote(args);
}

void HandleListCommand(NoteManager& manager) {
    manager.ListNotes();
}

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command search!" << std::endl;
        std::cout << "Proper Usage: search <query>" << std::endl;
        return;
    }
    manager.SearchNote(args);
}

void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 2) {
        std::cout << "Invalid usage of command tag!" << std::endl;
        std::cout << "Proper Usage: tag <filename> <tag>" << std::endl;
        return;
    }
    manager.TagNote(args);
}

void HandleHelpCommand() {
    display_utils::PrintCommands();
}


void DispatchCommand(const std::vector<std::string>& command_args) {
    if (command_args.empty()) {
        HandleHelpCommand();
        return;
    }

    NoteManager manager("notes");

    const std::string& command = command_args[0];
    std::vector<std::string> args(command_args.begin() + 1, command_args.end());

    if (!input_validation_utils::ValidateArgs(args)) {
        std::cout << "Invalid argument(s)." << std::endl;
        return;
    }

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