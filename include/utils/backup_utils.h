#ifndef NOTE_DB_BACKUP_UTILS_H
#define NOTE_DB_BACKUP_UTILS_H

#include <string>
#include <unordered_map>
#include <vector>

namespace backup_utils {

void SetupBackupDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);

} // namespace backup_utils

#endif //NOTE_DB_BACKUP_UTILS_H
