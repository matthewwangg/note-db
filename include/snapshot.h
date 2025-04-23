#ifndef NOTE_DB_SNAPSHOT_H
#define NOTE_DB_SNAPSHOT_H

#include <vector>

#include "snapshot_entry.h"

class Snapshot {
public:
    explicit Snapshot(const std::filesystem::path& root);

    void Diff(const Snapshot& other) const;
    void Generate(const std::filesystem::path& notes_directory);
    void LoadFromFile(const std::filesystem::path& path);
    void SaveToFile() const;

private:
    std::vector<SnapshotEntry> entries_;
    std::filesystem::path root_;
};

#endif //NOTE_DB_SNAPSHOT_H
