#ifndef NOTE_DB_CONFIG_UTILS_H
#define NOTE_DB_CONFIG_UTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace config_utils {

std::string LoadNotesDirectory(const std::filesystem::path& root_directory);
void SetupConfigFile(const std::vector<std::string>& args);

}

#endif //NOTE_DB_CONFIG_UTILS_H
