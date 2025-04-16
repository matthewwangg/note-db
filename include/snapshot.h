#ifndef NOTE_DB_SNAPSHOT_H
#define NOTE_DB_SNAPSHOT_H

#include <vector>

#include "snapshot_entry.h"

class Snapshot {
public:
    explicit Snapshot(const std::filesystem::path& root);

    void Generate(const std::filesystem::path& notes_directory);
    void SaveToFile() const;
    void LoadFromFile(const std::filesystem::path& path);
    void Diff(const Snapshot& other) const;

private:
    std::filesystem::path root_;
    std::vector<SnapshotEntry> entries_;
};

#endif //NOTE_DB_SNAPSHOT_H
