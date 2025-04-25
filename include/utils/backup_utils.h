#ifndef NOTE_DB_BACKUP_UTILS_H
#define NOTE_DB_BACKUP_UTILS_H

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace backup_utils {

void SaveBackup(const std::filesystem::path& notes_dir);
void SetupBackupDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
void RestoreBackup(const std::filesystem::path& notes_dir, const std::filesystem::path& backup_file);

} // namespace backup_utils

#endif //NOTE_DB_BACKUP_UTILS_H
