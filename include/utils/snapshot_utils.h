#ifndef NOTE_DB_SNAPSHOT_UTILS_H
#define NOTE_DB_SNAPSHOT_UTILS_H

#include <string>
#include <unordered_map>
#include <vector>

namespace snapshot_utils {

void SetupSnapshotDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);

}
#endif //NOTE_DB_SNAPSHOT_UTILS_H
