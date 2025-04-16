#ifndef NOTE_DB_SNAPSHOT_UTILS_H
#define NOTE_DB_SNAPSHOT_UTILS_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace snapshot_utils {

void SetupSnapshotDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
uint64_t FNV1a(const std::string& content);
std::string ExtractValue(const std::string& json, const std::string& key);

}
#endif //NOTE_DB_SNAPSHOT_UTILS_H
