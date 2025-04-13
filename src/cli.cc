#include "cli.h"
#include "index.h"
#include "utils/display_utils.h"
#include "utils/input_validation_utils.h"
#include "utils/config_utils.h"

#include <iostream>
#include <memory>
#include <vector>
#include <string>

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command new!" << std::endl;
        std::cout << "Proper Usage: new <filename>" << std::endl;
        return;
    }

    std::vector<std::string> normalized_args = args;
    normalized_args[0] = input_validation_utils::NormalizeFilename(normalized_args[0]);

    if (std::filesystem::exists(manager.get_notes_directory() / normalized_args[0])) {
        std::cout << "File " << normalized_args[0] << " already exists!" << std::endl;
        return;
    }

    manager.CreateNote(normalized_args);
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command edit!" << std::endl;
        std::cout << "Proper Usage: edit <filename>" << std::endl;
        return;
    }

    std::vector<std::string> normalized_args = args;
    normalized_args[0] = input_validation_utils::NormalizeFilename(normalized_args[0]);

    if (!std::filesystem::exists(manager.get_notes_directory() / normalized_args[0])) {
        std::cout << "File " << normalized_args[0] << " doesn't exist yet!" << std::endl;
        return;
    }

    manager.EditNote(normalized_args);
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command delete!" << std::endl;
        std::cout << "Proper Usage: delete <filename>" << std::endl;
        return;
    }

    std::vector<std::string> normalized_args = args;
    normalized_args[0] = input_validation_utils::NormalizeFilename(normalized_args[0]);

    if (!std::filesystem::exists(manager.get_notes_directory() / normalized_args[0])) {
        std::cout << "File " << normalized_args[0] << " doesn't exist!" << std::endl;
        return;
    }

    manager.DeleteNote(normalized_args);
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

    std::vector<std::string> normalized_args = args;
    normalized_args[0] = input_validation_utils::NormalizeFilename(normalized_args[0]);

    if (!std::filesystem::exists(manager.get_notes_directory() / normalized_args[0])) {
        std::cout << "File " << normalized_args[0] << " doesn't exist yet!" << std::endl;
        return;
    }

    manager.TagNote(normalized_args);
}

void HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command import!" << std::endl;
        std::cout << "Proper Usage: import <filepath>" << std::endl;
        return;
    }
    manager.ImportCommand(args);
}

void HandleInitCommand(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Invalid usage of command init!" << std::endl;
        std::cout << "Proper Usage: init <directory>" << std::endl;
        return;
    }

    config_utils::SetupConfigFile(args);
}

void HandleHelpCommand() {
    display_utils::PrintCommands();
}

void DispatchCommand(const std::vector<std::string>& command_args) {
    if (command_args.empty()) {
        HandleHelpCommand();
        return;
    }

    const std::string& command = command_args[0];
    std::vector<std::string> args(command_args.begin() + 1, command_args.end());

    if (command == "init") {
        HandleInitCommand(args);
        return;
    }

    std::string notes_directory = config_utils::LoadNotesDirectory(std::filesystem::current_path());
    if (notes_directory.empty()) {
        std::cout << "Missing or invalid .notedb/config.json in current directory!" << std::endl;
        std::cout << "Try running init <directory> to target where your note files are" << std::endl;
        return;
    }

    std::shared_ptr<SearchIndex> index = std::make_shared<SearchIndex>();
    index->BuildIndex(notes_directory);

    NoteManager manager(notes_directory, index);

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
    } else if (command == "import") {
        HandleImportCommand(manager, args);
    } else if (command == "help") {
            HandleHelpCommand();
    } else {
        std::cout << "Unsupported command: " << command << "\n";
        HandleHelpCommand();
    }
}

}