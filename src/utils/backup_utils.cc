#include "utils/backup_utils.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace backup_utils {

void SetupBackupDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path backup_dir = notes_dir / ".backups";
    std::filesystem::create_directories(backup_dir);
}

} // namespace backup_utils
