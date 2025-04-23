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

void DispatchCommand(const std::vector<std::string>& command_args) {
    if (command_args.empty()) {
        HandleHelpCommand();
        return;
    }

    const std::string& command = command_args[0];
    std::vector<std::string> args(command_args.begin() + 1, command_args.end());

    if (command == "--version") {
        std::cout << "note-db v0.1" << std::endl;
        return;
    }

    if (command == "help" || command == "--help") {
        HandleHelpCommand();
        return;
    }

    if (command == "init") {
        HandleInitCommand(args);
        return;
    }

    std::string notes_directory = config_utils::LoadNotesDirectory();
    if (notes_directory.empty()) {
        display_utils::PrintError("Missing or invalid .notedb/config.json in home directory!");
        display_utils::PrintInfo("Try running note-db init <directory> to indicate where your note files are");
        return;
    }

    std::shared_ptr<SearchIndex> index = std::make_shared<SearchIndex>();

    NoteManager manager(notes_directory, index);

    if (command == "import") {
        HandleImportCommand(manager, args);
        return;
    }

    if (!input_validation_utils::ValidateArgs(args)) {
        display_utils::PrintError("Invalid argument(s).");
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
    } else if (command == "template") {
        HandleTemplateCommand(manager, args);
    } else {
        display_utils::PrintError("Unsupported command: " + command);
        HandleHelpCommand();
    }
}

void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command delete!");
        display_utils::PrintInfo("Proper Usage: note-db delete <filename> [--directory <folder>]");
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist!");
        return;
    }

    manager.DeleteNote(normalized_args, flag_map);
    display_utils::PrintSuccess("Note " + normalized_filename + " successfully deleted!");
}

void HandleDiffCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command diff!");
        display_utils::PrintInfo("Proper Usage: note-db diff <snapshot>");
        return;
    }

    Snapshot current_snapshot(manager.GetNotesDirectory() / "snapshots");
    current_snapshot.Generate(manager.GetNotesDirectory());

    Snapshot previous_snapshot(manager.GetNotesDirectory() / "snapshots");
    previous_snapshot.LoadFromFile(manager.GetNotesDirectory() / "snapshots" / args[0]);

    current_snapshot.Diff(previous_snapshot);
}

void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command edit!");
        display_utils::PrintInfo("Proper Usage: note-db edit <filename> [--editor <editor>] [--directory <folder>]");
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist yet!");
        return;
    }

    manager.EditNote(normalized_args, flag_map);
    display_utils::PrintSuccess("Note " + normalized_filename + " successfully edited!");
}

void HandleHelpCommand() {
    display_utils::PrintCommands();
}

void HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command import!");
        display_utils::PrintInfo("Proper Usage: note-db import <filepath> [--overwrite <true/false>] [--directory <folder>]");
        return;
    }

    std::filesystem::path file_path = std::filesystem::absolute(args[0]);
    if (!std::filesystem::exists(file_path)) {
        display_utils::PrintError("File doesn't exist: " + file_path.string());
        return;
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    bool overwrite = flag_map.find("--overwrite") != flag_map.end() && flag_map.at("--overwrite") == "true";
    std::filesystem::path destination = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map) / file_path.filename();

    if (std::filesystem::exists(destination) && !overwrite) {
        display_utils::PrintError("File " + file_path.filename().string() + " exists already in notes directory!");
        return;
    }

    manager.ImportNote(args, flag_map);
    display_utils::PrintSuccess("Note " + args[0] + " successfully imported!");
}

void HandleInitCommand(const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command init!");
        display_utils::PrintInfo("Proper Usage: note-db init <directory>");
        return;
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    config_utils::SetupConfigFile(args, flag_map);
    template_utils::SetupTemplateDirectory(args, flag_map);
    snapshot_utils::SetupSnapshotDirectory(args, flag_map);
    display_utils::PrintSuccess("Directory " + args[0] + " successfully initialized!");
}

void HandleListCommand(NoteManager& manager) {
    manager.ListNotes();
}

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command new!");
        display_utils::PrintInfo("Proper Usage: note-db new <filename> [--template <name>] [--editor <editor>] [--overwrite <true/false>] [--directory <folder>]");
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    bool overwrite = flag_map.find("--overwrite") != flag_map.end() && flag_map.at("--overwrite") == "true";
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (std::filesystem::exists(path / normalized_filename) && !overwrite) {
        display_utils::PrintError("File " + normalized_filename + " already exists!");
        return;
    }

    manager.CreateNote(normalized_args, flag_map);
    display_utils::PrintSuccess("Note " + normalized_filename + " successfully created!");
}

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command search!");
        display_utils::PrintInfo("Proper Usage: note-db search <query> [--tag <tag>] [--limit <limit>] [--sort-by <field>]");
        return;
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    manager.SearchNote(args, flag_map);
}

void HandleSnapshotCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (!args.empty()) {
        display_utils::PrintError("Invalid usage of command snapshot!");
        display_utils::PrintInfo("Proper Usage: note-db snapshot");
        return;
    }

    Snapshot snapshot(manager.GetNotesDirectory() / "snapshots");
    snapshot.Generate(manager.GetNotesDirectory());
    snapshot.SaveToFile();
    display_utils::PrintSuccess("Snapshot successfully saved!");
}

void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() < 2 || !command_utils::ValidateArgs(args, 2)) {
        display_utils::PrintError("Invalid usage of command tag!");
        display_utils::PrintInfo("Proper Usage: note-db tag <filename> <tag> [--directory <folder>]");
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename, args[1]};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 2);
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist yet!");
        return;
    }

    manager.TagNote(normalized_args, flag_map);
    display_utils::PrintSuccess("Note " + normalized_filename + " successfully tagged!");
}

void HandleTemplateCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        display_utils::PrintError("Invalid usage of command template!");
        display_utils::PrintInfo("Proper Usage: template <filename>");
        return;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::filesystem::path template_path = manager.GetNotesDirectory() / "templates" / normalized_filename;

    if (std::filesystem::exists(template_path)) {
        display_utils::PrintError("Template " + normalized_filename + " already exists!");
        return;
    }

    template_utils::CreateUniqueTemplate(manager.GetEditor(), template_path);
}

} // namespace cli
