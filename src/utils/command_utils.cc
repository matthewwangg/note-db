#include "utils/command_utils.h"

namespace command_utils {

std::unordered_map<std::string, std::string> ParseFlags(const std::vector<std::string>& flags) {
    std::unordered_map<std::string, std::string> flag_mapping;

    for (int i = 0; i < flags.size() - 1; i = i + 2) {
        flag_mapping[flags[i]] = flags[i+1];
    }

    return flag_mapping;
}

}