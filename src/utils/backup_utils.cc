#include "utils/backup_utils.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

#include "note.h"

namespace backup_utils {

void SaveBackup(const std::filesystem::path& notes_dir) {
    auto now = std::time(nullptr);
    std::ofstream out(notes_dir / ".backups" / (std::to_string(now) + ".json"));

    out << "[\n";
    bool first = true;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(notes_dir)) {
        if (entry.path().extension() != ".md") {
            continue;
        }

        if (!first) {
            out << ",\n";
        }
        first = false;

        Note note = Note::LoadFromFile(entry.path());
        std::string rel = std::filesystem::relative(entry.path(), notes_dir).string();

        out << "  {\n"
            << "    \"path\": " << std::quoted(rel) << ",\n"
            << "    \"content\": " << std::quoted(note.ToString()) << "\n"
            << "  }";
    }
    out << "\n]\n";
}

void SetupBackupDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path backup_dir = notes_dir / ".backups";
    std::filesystem::create_directories(backup_dir);
}

void RestoreBackup(const std::filesystem::path& notes_dir, const std::filesystem::path& backup_file) {
    std::ifstream in(backup_file);
    if (!in.is_open()) {
        return;
    }

    std::string token;
    while (in >> token) {
        if (token == "{") {
            std::string path_key;
            std::string content_key;
            std::string rel_path;
            std::string content_str;

            in >> path_key >> std::quoted(rel_path) >> token;
            in >> content_key >> std::quoted(content_str);

            std::filesystem::path full_path = notes_dir / rel_path;
            std::filesystem::create_directories(full_path.parent_path());

            Note note = Note::FromString(content_str, full_path.filename().string());
            std::ofstream out(full_path);
            if (out.is_open()) {
                out << note.ToString();
            }
        }
    }
}

} // namespace backup_utils
