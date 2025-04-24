#include "utils/command_utils.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace command_utils {

std::unordered_map<std::string, std::string> ExtractFlagMap(const std::vector<std::string>& args, int start_index) {
    std::vector<std::string> flags(args.begin() + start_index, args.end());
    return command_utils::ParseFlags(flags);
}

std::unordered_map<std::string, std::string> ParseFlags(const std::vector<std::string>& flags) {
    std::unordered_map<std::string, std::string> flag_map = {};

    const std::unordered_set<std::string> value_flags = { "--editor", "--template", "--directory", "--tag", "--limit", "--sort-by" };
    const std::unordered_set<std::string> boolean_flags = { "--overwrite" };

    for (size_t i = 0; i < flags.size(); i++) {
        const std::string& flag = flags[i];

        if (flag_map.find(flag) != flag_map.end()) {
            continue;
        }

        if (value_flags.count(flag)) {
            if (i + 1 < flags.size() && flags[i + 1].rfind("--", 0) != 0) {
                flag_map[flag] = flags[i + 1];
                i++;
            }
        } else if (boolean_flags.count(flag)) {
            flag_map[flag] = "true";
        } else {
            continue;
        }
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

} // namespace command_utils