#ifndef NOTE_DB_SNAPSHOT_UTILS_H
#define NOTE_DB_SNAPSHOT_UTILS_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace snapshot_utils {

std::string ExtractValue(const std::string& json, const std::string& key);
uint64_t FNV1a(const std::string& content);
void SetupSnapshotDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);

} // namespace snapshot_utils

#endif //NOTE_DB_SNAPSHOT_UTILS_H
