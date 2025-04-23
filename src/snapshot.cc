#include "snapshot.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include "utils/snapshot_utils.h"


Snapshot::Snapshot(const std::filesystem::path& root)
    : root_(root)
{

}

void Snapshot::Diff(const Snapshot& previous_snapshot) const {
    std::unordered_map<std::string, std::string> current_map;
    std::unordered_map<std::string, std::string> prev_map;

    for (const auto& entry : entries_) {
        current_map[entry.relative_path] = entry.hash;
    }

    for (const auto& entry : previous_snapshot.entries_) {
        prev_map[entry.relative_path] = entry.hash;
    }

    if (current_map == prev_map) {
        std::cout << "No difference found between this snapshot and your current notes!" << std::endl;
    }

    for (const auto& [path, hash] : current_map) {
        auto it = prev_map.find(path);
        if (it == prev_map.end()) {
            std::cout << "[ADDED]    " << path << '\n';
        } else if (it->second != hash) {
            std::cout << "[MODIFIED] " << path << '\n';
        }
    }

    for (const auto& [path, _] : prev_map) {
        if (current_map.find(path) == current_map.end()) {
            std::cout << "[REMOVED]  " << path << '\n';
        }
    }
}

void Snapshot::Generate(const std::filesystem::path& notes_directory) {
    entries_.clear();

    for (const auto& path: std::filesystem::recursive_directory_iterator(notes_directory)) {
        if (path.is_regular_file() && path.path().extension().string() == ".md") {
            std::filesystem::path relative_path = std::filesystem::relative(path.path(), notes_directory);
            Note note = Note::LoadFromFile(path.path());

            SnapshotEntry entry = SnapshotEntry::SnapshotFromNote(note, relative_path);

            entries_.push_back(std::move(entry));
        }
    }
}

void Snapshot::LoadFromFile(const std::filesystem::path& path) {
    entries_.clear();
    std::ifstream in(path);

    std::string json((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    size_t pos = 0;

    while ((pos = json.find('{', pos)) != std::string::npos) {
        size_t obj_end = json.find('}', pos);

        if (obj_end == std::string::npos) {
            break;
        }

        std::string obj = json.substr(pos, obj_end - pos + 1);

        SnapshotEntry entry;
        entry.relative_path = snapshot_utils::ExtractValue(obj, "relative_path");
        entry.hash = snapshot_utils::ExtractValue(obj, "hash");
        entry.modified = std::chrono::system_clock::from_time_t(std::stoll(snapshot_utils::ExtractValue(obj, "modified")));

        entries_.push_back(std::move(entry));
        pos = obj_end + 1;
    }
}

void Snapshot::SaveToFile() const {
    time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = std::to_string(current_time) + ".json";

    std::ofstream out(root_ / filename);

    out << "[\n";

    for (size_t i = 0; i < entries_.size(); ++i) {
        const auto& entry = entries_[i];

        out << "  {\n";
        out << "    \"relative_path\": \"" << entry.relative_path << "\",\n";
        out << "    \"hash\": \"" << entry.hash << "\",\n";
        out << "    \"modified\": " << std::chrono::system_clock::to_time_t(entry.modified) << '\n';
        out << "  }";

        if (i + 1 < entries_.size()) {
            out << ",";
        }
        out << "\n";
    }

    out << "]\n";
}


