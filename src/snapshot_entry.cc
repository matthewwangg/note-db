#include "snapshot_entry.h"

#include "utils/snapshot_utils.h"

SnapshotEntry SnapshotEntry::SnapshotFromNote(const Note &note, const std::filesystem::path& relative_path) {
    SnapshotEntry entry = {
            relative_path.string(),
            std::to_string(snapshot_utils::FNV1a(note.content)),
            note.updated
    };
    return entry;
}

