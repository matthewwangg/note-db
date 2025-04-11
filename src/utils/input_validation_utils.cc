#include "utils/input_validation_utils.h"

#include <algorithm>
#include <regex>

namespace input_validation_utils {

bool ValidateArgs(const std::vector<std::string>& args) {
    for (const std::string& arg : args) {
        if (!IsValidArg(arg)) {
            return false;
        }
    }
    return true;
}

bool IsValidArg(const std::string& arg) {
    if (arg.empty() || std::all_of(arg.begin(), arg.end(), isspace)) {
        return false;
    }

    if (arg.find("..") != std::string::npos || arg.front() == '/' || arg.front() == '~') {
        return false;
    }

    static const std::regex illegal_chars(R"([<>:"/\\|?*])");
    if (std::regex_search(arg, illegal_chars)) {
        return false;
    }

    if (arg.length() > 30) {
        return false;
    }

    return true;
}

}