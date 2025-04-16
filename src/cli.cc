#include "cli.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "index.h"
#include "snapshot.h"
#include "utils/command_utils.h"
#include "utils/config_utils.h"
#include "utils/display_utils.h"
#include "utils/input_validation_utils.h"
#include "utils/snapshot_utils.h"
#include "utils/template_utils.h"

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command new!" << std::endl;
        std::cout << "Proper Usage: new <filename> [--template <name>]" << std::endl;
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    if (std::filesystem::exists(manager.GetNotesDirectory() / normalized_filename)) {
        std::cout << "File " << normalized_filename << " already exists!" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.CreateNote(normalized_args, flag_map);
    std::cout << "Note " << normalized_filename << " successfully created!" << std::endl;
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command edit!" << std::endl;
        std::cout << "Proper Usage: edit <filename>" << std::endl;
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    if (!std::filesystem::exists(manager.GetNotesDirectory() / normalized_filename)) {
        std::cout << "File " << normalized_filename << " doesn't exist yet!" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.EditNote(normalized_args, flag_map);
    std::cout << "Note " << normalized_filename << " successfully edited!" << std::endl;
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command delete!" << std::endl;
        std::cout << "Proper Usage: delete <filename>" << std::endl;
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    if (!std::filesystem::exists(manager.GetNotesDirectory() / normalized_filename)) {
        std::cout << "File " << normalized_filename << " doesn't exist!" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.DeleteNote(normalized_args, flag_map);
    std::cout << "Note " << normalized_filename << " successfully deleted!" << std::endl;
}

void HandleListCommand(NoteManager& manager) {
    manager.ListNotes();
}

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command search!" << std::endl;
        std::cout << "Proper Usage: search <query> [--tag <tag>] [--limit <limit>] [--sort-by <field>]" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.SearchNote(args, flag_map);
}

void HandleSnapshotCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (!args.empty()) {
        std::cout << "Invalid usage of command snapshot!" << std::endl;
        std::cout << "Proper Usage: snapshot" << std::endl;
        return;
    }

    Snapshot snapshot(manager.GetNotesDirectory() / "snapshots");
    snapshot.Generate(manager.GetNotesDirectory());
    snapshot.SaveToFile();
    std::cout << "Snapshot successfully saved!" << std::endl;
}

void HandleDiffCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command diff!" << std::endl;
        std::cout << "Proper Usage: diff <snapshot>" << std::endl;
        return;
    }

    Snapshot current_snapshot(manager.GetNotesDirectory() / "snapshots");
    current_snapshot.Generate(manager.GetNotesDirectory());

    Snapshot previous_snapshot(manager.GetNotesDirectory() / "snapshots");
    previous_snapshot.LoadFromFile(manager.GetNotesDirectory() / "snapshots" / args[0]);

    current_snapshot.Diff(previous_snapshot);
}

void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Invalid usage of command tag!" << std::endl;
        std::cout << "Proper Usage: tag <filename> <tag>" << std::endl;
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    if (!std::filesystem::exists(manager.GetNotesDirectory() / normalized_filename)) {
        std::cout << "File " << normalized_filename << " doesn't exist yet!" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 2, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.TagNote(normalized_args, flag_map);
    std::cout << "Note " << normalized_filename << " successfully tagged!" << std::endl;
}

void HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command import!" << std::endl;
        std::cout << "Proper Usage: import <filepath>" << std::endl;
        return;
    }

    std::filesystem::path file_path = std::filesystem::absolute(args[0]);
    if (!std::filesystem::exists(file_path)) {
        std::cout << "File doesn't exist: " << file_path << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    manager.ImportNote(args, flag_map);
    std::cout << "Note " << args[0] << " successfully imported!" << std::endl;
}

void HandleInitCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Invalid usage of command init!" << std::endl;
        std::cout << "Proper Usage: init <directory>" << std::endl;
        return;
    }

    std::vector<std::string> flags(args.begin() + 1, args.end());
    std::unordered_map<std::string, std::string> flag_map = command_utils::ParseFlags(flags);

    config_utils::SetupConfigFile(args, flag_map);
    template_utils::SetupTemplateDirectory(args, flag_map);
    snapshot_utils::SetupSnapshotDirectory(args, flag_map);
    std::cout << "Directory " << args[0] << " successfully initialized!" << std::endl;
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

    std::string notes_directory = config_utils::LoadNotesDirectory();
    if (notes_directory.empty()) {
        std::cout << "Missing or invalid .notedb/config.json in home directory!" << std::endl;
        std::cout << "Try running init <directory> to indicate where your note files are" << std::endl;
        return;
    }

    std::shared_ptr<SearchIndex> index = std::make_shared<SearchIndex>();

    NoteManager manager(notes_directory, index);

    if (command == "import") {
        HandleImportCommand(manager, args);
        return;
    }

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
        index->BuildIndex(notes_directory);
        HandleSearchCommand(manager, args);
    } else if (command == "snapshot") {
        HandleSnapshotCommand(manager, args);
    } else if (command == "diff") {
        HandleDiffCommand(manager, args);
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