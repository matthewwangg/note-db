#include "utils/command_utils.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace command_utils {

std::unordered_map<std::string, std::string> ParseFlags(const std::vector<std::string>& flags) {
    std::unordered_map<std::string, std::string> flag_map = {};

    for (int i = 0; i + 1 < flags.size(); i = i + 2) {
        flag_map[flags[i]] = flags[i+1];
    }

    return flag_map;
}

std::filesystem::path ResolveDirectory(const std::filesystem::path& path, const std::unordered_map<std::string, std::string>& flag_map) {
    auto it = flag_map.find("--directory");
    if (it != flag_map.end()) {
        return path / it->second;
    }
    return path;
}

bool ValidateArgs(const std::vector<std::string>& args, int required) {
    for (int i = 0; i < required; i++) {
        if (args[i].starts_with("--")) {
            return false;
        }
    }
    return true;
}

}