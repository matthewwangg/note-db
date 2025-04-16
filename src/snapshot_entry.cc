#include "snapshot_entry.h"

SnapshotEntry SnapshotEntry::SnapshotFromNote(const Note &note, const std::filesystem::path& relative_path) {
    SnapshotEntry entry = {relative_path.string(), "", note.updated};

    return entry;
}

