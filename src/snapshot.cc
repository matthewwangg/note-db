#include "snapshot.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>


Snapshot::Snapshot(const std::filesystem::path& root)
    : root_(root)
{

}

void Snapshot::Generate(const std::filesystem::path& notes_directory) {
    entries_.clear();

    for (const auto& path: std::filesystem::recursive_directory_iterator(notes_directory)) {
        if (path.is_regular_file()) {
            std::filesystem::path relative_path = std::filesystem::relative(path.path(), notes_directory);
            Note note = Note::LoadFromFile(path.path());

            SnapshotEntry entry = SnapshotEntry::SnapshotFromNote(note, relative_path);

            entries_.push_back(std::move(entry));
        }
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
        out << "    \"path\": \"" << entry.relative_path << "\",\n";
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

void Snapshot::LoadFromFile(const std::filesystem::path& path) {
    entries_.clear();
    std::ifstream in(path);

    std::string relative_path, hash;
    std::time_t modified;
    while (in >> relative_path >> hash >> modified) {
        entries_.push_back(SnapshotEntry{
                .relative_path = relative_path,
                .hash = hash,
                .modified = std::chrono::system_clock::from_time_t(modified)
        });
    }
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

