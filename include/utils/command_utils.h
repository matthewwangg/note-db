#ifndef NOTE_DB_COMMAND_UTILS_H
#define NOTE_DB_COMMAND_UTILS_H

#include <string>
#include <unordered_map>
#include <vector>

namespace command_utils {

std::unordered_map<std::string, std::string> ParseFlags(const std::vector<std::string>& flags);

}

#endif //NOTE_DB_COMMAND_UTILS_H
