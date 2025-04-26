#include "cli.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "index.h"
#include "snapshot.h"
#include "utils/automation_utils.h"
#include "utils/backup_utils.h"
#include "utils/command_utils.h"
#include "utils/config_utils.h"
#include "utils/display_utils.h"
#include "utils/input_validation_utils.h"
#include "utils/snapshot_utils.h"
#include "utils/template_utils.h"

namespace cli {

bool DispatchCommand(const std::vector<std::string>& command_args, bool automated) {
    if (command_args.empty()) {
        HandleHelpCommand();
        return false;
    }

    const std::string& command = command_args[0];
    std::vector<std::string> args(command_args.begin() + 1, command_args.end());

    if (command == "--version") {
        std::cout << "note-db v0.1" << std::endl;
        return true;
    }

    if (command == "help" || command == "--help") {
        HandleHelpCommand();
        return true;
    }

    if (command == "init") {
        return HandleInitCommand(args);
    }

    std::string notes_directory = config_utils::LoadNotesDirectory();
    if (notes_directory.empty()) {
        display_utils::PrintError("Missing or invalid .note-db/config.json in home directory!");
        display_utils::PrintInfo("Try running note-db init <directory> to indicate where your note files are");
        return false;
    }

    std::shared_ptr<SearchIndex> index = std::make_shared<SearchIndex>();
    NoteManager manager(notes_directory, index);

    if (command == "import") {
        return HandleImportCommand(manager, args);
    }

    if (!input_validation_utils::ValidateArgs(args)) {
        display_utils::PrintError("Invalid argument(s).");
        return false;
    }

    bool result = false;

    if (command == "new") {
        result = HandleNewCommand(manager, args);
    } else if (command == "edit") {
        result = HandleEditCommand(manager, args);
    } else if (command == "delete") {
        result = HandleDeleteCommand(manager, args);
    } else if (command == "list") {
        result = HandleListCommand(manager);
    } else if (command == "search") {
        index->BuildIndex(notes_directory);
        result = HandleSearchCommand(manager, args);
    } else if (command == "snapshot") {
        result = HandleSnapshotCommand(manager, args);
    } else if (command == "diff") {
        result = HandleDiffCommand(manager, args);
    } else if (command == "tag") {
        result = HandleTagCommand(manager, args);
    } else if (command == "template") {
        result = HandleTemplateCommand(manager, args);
    } else if (command == "backup") {
        result = HandleBackupCommand(manager, args);
    } else if (command == "restore") {
        result = HandleRestoreCommand(manager, args);
    } else if (command == "run") {
        result = HandleRunCommand(manager, args, automated);
    } else {
        display_utils::PrintError("Unsupported command: " + command);
        HandleHelpCommand();
    }

    return result;
}

bool HandleBackupCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (!args.empty()) {
        display_utils::PrintError("Invalid usage of command backup!");
        display_utils::PrintInfo("Proper Usage: note-db backup");
        return false;
    }

    time_t time = backup_utils::SaveBackup(manager.GetNotesDirectory());

    display_utils::PrintSuccess("Backup " + std::to_string(time) + ".json created successfully.");
    return true;
}

bool HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command delete!");
        display_utils::PrintInfo("Proper Usage: note-db delete <filename> [--directory <folder>]");
        return false;
    }

    std::filesystem::path filename = args[0];
    if (filename.has_extension() && filename.extension() != ".md") {
        display_utils::PrintWarning("This file is not a Markdown note!");
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist!");
        return false;
    }

    manager.DeleteNote(normalized_args, flag_map);

    display_utils::PrintSuccess("Note " + normalized_filename + " successfully deleted!");
    return true;
}

bool HandleDiffCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command diff!");
        display_utils::PrintInfo("Proper Usage: note-db diff <snapshot>");
        return false;
    }

    Snapshot current_snapshot(manager.GetNotesDirectory() / ".snapshots");
    current_snapshot.Generate(manager.GetNotesDirectory());

    std::filesystem::path path = manager.GetNotesDirectory() / ".snapshots" / args[0];
    if (path.extension().string() != ".json" || !std::filesystem::exists(path)) {
        display_utils::PrintError("This is not a valid snapshot file! Please try again with a valid snapshot!");
        return false;
    }

    Snapshot previous_snapshot(manager.GetNotesDirectory() / ".snapshots");
    previous_snapshot.LoadFromFile(path);

    current_snapshot.Diff(previous_snapshot);

    return true;
}

bool HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command edit!");
        display_utils::PrintInfo("Proper Usage: note-db edit <filename> [--editor <editor>] [--directory <folder>]");
        return false;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    if (flag_map.find("--editor") == flag_map.end() && !std::getenv("NOTEDB_EDITOR")) {
        display_utils::PrintWarning("No editor specified. Using default editor " + manager.GetEditor() + ".");
    }

    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist yet!");
        return false;
    }

    manager.EditNote(normalized_args, flag_map);

    display_utils::PrintSuccess("Note " + normalized_filename + " successfully edited!");
    return true;
}

void HandleHelpCommand() {
    display_utils::PrintCommands();
}

bool HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command import!");
        display_utils::PrintInfo("Proper Usage: note-db import <filepath> [--overwrite] [--directory <folder>]");
        return false;
    }

    std::filesystem::path file_path = std::filesystem::absolute(args[0]);
    if (!std::filesystem::exists(file_path)) {
        display_utils::PrintError("File doesn't exist: " + file_path.string());
        return false;
    }

    if (file_path.filename().string().length() > 30) {
        display_utils::PrintWarning("Filename is very long, consider something below 30 characters");
    }

    if (file_path.extension() != ".md") {
        display_utils::PrintWarning("Imported file is not a Markdown file!");
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    bool overwrite = flag_map.find("--overwrite") != flag_map.end() && flag_map.at("--overwrite") == "true";
    std::filesystem::path destination = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map) / file_path.filename();

    if (std::filesystem::exists(destination) && !overwrite) {
        display_utils::PrintError("File " + file_path.filename().string() + " exists already in notes directory!");
        return false;
    }

    manager.ImportNote(args, flag_map);

    display_utils::PrintSuccess("Note " + args[0] + " successfully imported!");
    return true;
}

bool HandleInitCommand(const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command init!");
        display_utils::PrintInfo("Proper Usage: note-db init <directory>");
        return false;
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    std::filesystem::path target_directory = args[0];
    if (std::filesystem::exists(target_directory) && !std::filesystem::is_directory(target_directory)) {
        display_utils::PrintError("Path exists but is not a directory: " + target_directory.string());
        return false;
    }

    config_utils::SetupConfigFile(args, flag_map);
    automation_utils::SetupBasicCommandFile();
    template_utils::SetupTemplateDirectory(args, flag_map);
    snapshot_utils::SetupSnapshotDirectory(args, flag_map);
    backup_utils::SetupBackupDirectory(args, flag_map);

    display_utils::PrintSuccess("Directory " + args[0] + " successfully initialized!");
    return true;
}

bool HandleListCommand(NoteManager& manager) {
    manager.ListNotes();
    return true;
}

bool HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command new!");
        display_utils::PrintInfo("Proper Usage: note-db new <filename> [--template <name>] [--editor <editor>] [--overwrite] [--directory <folder>]");
        return false;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    bool overwrite = flag_map.find("--overwrite") != flag_map.end() && flag_map.at("--overwrite") == "true";
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (std::filesystem::exists(path / normalized_filename) && !overwrite) {
        display_utils::PrintError("File " + normalized_filename + " already exists!");
        return false;
    }

    if (normalized_filename.length() > 30) {
        display_utils::PrintWarning("Filename is very long, consider something below 30 characters");
    }

    if (flag_map.find("--editor") == flag_map.end() && !std::getenv("NOTEDB_EDITOR")) {
        display_utils::PrintWarning("No editor specified. Using default editor.");
    }

    manager.CreateNote(normalized_args, flag_map);

    display_utils::PrintSuccess("Note " + normalized_filename + " successfully created!");
    return true;
}

bool HandleRestoreCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command restore!");
        display_utils::PrintInfo("Proper Usage: note-db restore <backup-name>");
        return false;
    }

    std::filesystem::path backup_file = manager.GetNotesDirectory() / ".backups" / args[0];
    if (!std::filesystem::exists(backup_file)) {
        display_utils::PrintError("Backup file " + backup_file.string() + " not found!");
        return false;
    }

    backup_utils::RestoreBackup(manager.GetNotesDirectory(), backup_file);

    display_utils::PrintSuccess("Backup restored successfully.");
    return true;
}

bool HandleRunCommand(NoteManager& manager, const std::vector<std::string>& args, bool automated) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command run!");
        display_utils::PrintInfo("Proper Usage: note-db run <command>");
        return false;
    }

    if (automated) {
        display_utils::PrintError("Calling a run command from a custom command is not allowed!");
        return false;
    }

    std::optional<CommandDefinition> command = automation_utils::LoadCommandByName(args[0]);
    if (!command) {
        display_utils::PrintError("Command " + args[0] + " not found!");
        return false;
    }

    time_t time = backup_utils::SaveBackup(manager.GetNotesDirectory());
    for (const std::string& command_str : command->steps) {
        if (!automation_utils::RunCommand(command_str)) {
            display_utils::PrintError("Command " + command_str + " failed during execution, please fix accordingly.");
            backup_utils::RestoreBackup(manager.GetNotesDirectory(), std::to_string(time) + ".json");
            return false;
        }
    }

    return true;
}

bool HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty() || !command_utils::ValidateArgs(args, 1)) {
        display_utils::PrintError("Invalid usage of command search!");
        display_utils::PrintInfo("Proper Usage: note-db search <query> [--tag <tag>] [--limit <limit>] [--sort-by <field>]");
        return false;
    }

    if (args[0].length() < 3) {
        display_utils::PrintWarning("Search query is very short. Results may be noisy.");
    }

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 1);

    manager.SearchNote(args, flag_map);

    return true;
}

bool HandleSnapshotCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (!args.empty()) {
        display_utils::PrintError("Invalid usage of command snapshot!");
        display_utils::PrintInfo("Proper Usage: note-db snapshot");
        return false;
    }

    Snapshot snapshot(manager.GetNotesDirectory() / ".snapshots");
    snapshot.Generate(manager.GetNotesDirectory());
    time_t timestamp = snapshot.SaveToFile();
    std::string snapshot_filename = std::to_string(timestamp) + ".json";

    display_utils::PrintSuccess("Snapshot " + snapshot_filename + " successfully saved!");

    return true;
}

bool HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.size() < 2 || !command_utils::ValidateArgs(args, 2)) {
        display_utils::PrintError("Invalid usage of command tag!");
        display_utils::PrintInfo("Proper Usage: note-db tag <filename> <tag> [--directory <folder>]");
        return false;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::vector<std::string> normalized_args = {normalized_filename, args[1]};

    std::unordered_map<std::string, std::string> flag_map = command_utils::ExtractFlagMap(args, 2);
    std::filesystem::path path = command_utils::ResolveDirectory(manager.GetNotesDirectory(), flag_map);

    if (!std::filesystem::exists(path / normalized_filename)) {
        display_utils::PrintError("File " + normalized_filename + " doesn't exist yet!");
        return false;
    }

    manager.TagNote(normalized_args, flag_map);

    display_utils::PrintSuccess("Note " + normalized_filename + " successfully tagged!");
    return true;
}

bool HandleTemplateCommand(NoteManager& manager, const std::vector<std::string>& args) {
    if (args.empty()) {
        display_utils::PrintError("Invalid usage of command template!");
        display_utils::PrintInfo("Proper Usage: template <filename>");
        return false;
    }

    std::string normalized_filename = input_validation_utils::NormalizeFilename(args[0]);
    std::filesystem::path template_path = manager.GetNotesDirectory() / ".templates" / normalized_filename;

    if (std::filesystem::exists(template_path)) {
        display_utils::PrintError("Template " + normalized_filename + " already exists!");
        return false;
    }

    template_utils::CreateUniqueTemplate(manager.GetEditor(), template_path);

    display_utils::PrintSuccess("Template " + normalized_filename + " successfully created!");
    return true;
}

} // namespace cli
