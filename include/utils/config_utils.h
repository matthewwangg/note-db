#ifndef NOTE_DB_CONFIG_UTILS_H
#define NOTE_DB_CONFIG_UTILS_H

#include <filesystem>
#include <string>

namespace config_utils {

std::string LoadNotesDirectory(const std::filesystem::path& root_directory);

}

#endif //NOTE_DB_CONFIG_UTILS_H
