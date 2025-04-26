#include "utils/backup_utils.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <vector>

#include "note.h"

namespace backup_utils {

void ClearNotesDirectory(const std::filesystem::path& notes_dir) {
    const std::unordered_set<std::string> keep_dirs = { ".templates", ".snapshots", ".backups" };

    for (const auto& entry : std::filesystem::directory_iterator(notes_dir)) {
        std::string name = entry.path().filename().string();

        bool should_keep = false;
        for (const std::string& keep : keep_dirs) {
            if (name == keep) {
                should_keep = true;
                break;
            }
        }

        if (should_keep) {
            continue;
        }

        if (entry.is_directory()) {
            std::filesystem::remove_all(entry.path());
        } else {
            std::filesystem::remove(entry.path());
        }
    }
}

std::string Escape(const std::string& input) {
    std::string out;
    for (char c : input) {
        if (c == '\\') {
            out += "\\\\";
        } else if (c == '\"') {
            out += "\\\"";
        } else if (c == '\n') {
            out += "\\n";
        } else {
            out += c;
        }
    }
    return out;
}

void RestoreBackup(const std::filesystem::path& notes_dir, const std::filesystem::path& backup_file) {
    ClearNotesDirectory(notes_dir);

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

            std::string unescaped = Unescape(content_str);
            Note note = Note::FromString(unescaped, full_path.filename().string());
            std::ofstream out(full_path);
            if (out.is_open()) {
                out << note.ToString();
            }
        }
    }
}

time_t SaveBackup(const std::filesystem::path& notes_dir) {
    auto now = std::time(nullptr);
    std::ofstream out(notes_dir / ".backups" / (std::to_string(now) + ".json"));

    out << "[\n";
    bool first = true;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(notes_dir)) {
        if (entry.path().extension() != ".md" || entry.path().string().find(".templates") != std::string::npos) {
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
            << "    \"content\": " << std::quoted(Escape(note.ToString())) << "\n"
            << "  }";
    }
    out << "\n]\n";

    return now;
}

void SetupBackupDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path backup_dir = notes_dir / ".backups";
    std::filesystem::create_directories(backup_dir);
}

std::string Unescape(const std::string& input) {
    std::string out;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            char next = input[i + 1];
            if (next == 'n') {
                out += '\n';
                ++i;
            } else if (next == '\"') {
                out += '\"';
                ++i;
            } else if (next == '\\') {
                out += '\\';
                ++i;
            } else {
                out += input[i];
            }
        } else {
            out += input[i];
        }
    }
    return out;
}

} // namespace backup_utils
