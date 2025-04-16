#ifndef NOTE_DB_SNAPSHOT_ENTRY_H
#define NOTE_DB_SNAPSHOT_ENTRY_H

#include <chrono>
#include <filesystem>
#include <string>

#include "note.h"

struct SnapshotEntry {
    std::string relative_path;
    std::string hash;
    std::chrono::system_clock::time_point modified;

    static SnapshotEntry SnapshotFromNote(const Note& note, const std::filesystem::path& relative_path);
};

#endif //NOTE_DB_SNAPSHOT_ENTRY_H
