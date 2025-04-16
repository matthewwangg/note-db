#ifndef NOTE_DB_SNAPSHOT_H
#define NOTE_DB_SNAPSHOT_H

#include <vector>

#include "snapshot_entry.h"

class Snapshot {
    explicit Snapshot(const std::filesystem::path& root);

    void Generate();
    void SaveToFile() const;
    void LoadFromFile(const std::filesystem::path& path);

private:
    std::filesystem::path root_;
    std::vector<SnapshotEntry> entries_;
};

#endif //NOTE_DB_SNAPSHOT_H
