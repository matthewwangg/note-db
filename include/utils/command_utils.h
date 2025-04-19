#ifndef NOTE_DB_COMMAND_UTILS_H
#define NOTE_DB_COMMAND_UTILS_H

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace command_utils {

std::unordered_map<std::string, std::string> ParseFlags(const std::vector<std::string>& flags);
std::filesystem::path ResolveDirectory(const std::filesystem::path& path, const std::unordered_map<std::string, std::string>& flag_map);
bool ValidateArgs(const std::vector<std::string>& args, int count);
std::unordered_map<std::string, std::string> ExtractFlagMap(const std::vector<std::string>& args, int start_index);

}

#endif //NOTE_DB_COMMAND_UTILS_H
