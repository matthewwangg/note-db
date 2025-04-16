#include "snapshot.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>


Snapshot::Snapshot(const std::filesystem::path& root)
    : root_(root)
{

}

void Snapshot::Generate() {
    entries_.clear();

    for (const auto& path: std::filesystem::recursive_directory_iterator(root_)) {
        if (path.is_regular_file()) {
            std::filesystem::path relative_path = std::filesystem::relative(path.path(), root_);
            Note note = Note::LoadFromFile(path.path());

            SnapshotEntry entry = SnapshotEntry::SnapshotFromNote(note, relative_path);

            entries_.push_back(std::move(entry));
        }
    }
}

void Snapshot::SaveToFile() const {
    time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string filename = std::to_string(current_time) + ".snap";

    std::ofstream out(root_ / filename);

    for (SnapshotEntry entry : entries_) {
        out << entry.relative_path << '\t'
            << entry.hash << '\t'
            << std::chrono::system_clock::to_time_t(entry.modified)
            << '\n';
    }
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
