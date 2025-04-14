#include "utils/input_validation_utils.h"

#include <algorithm>
#include <cctype>
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

    if (arg.length() > 30 || arg == ".md") {
        return false;
    }

    return true;
}

std::string NormalizeFilename(const std::string& name) {
    std::string normalized_filename;
    normalized_filename.reserve(name.size() + 3);

    for (char c : name) {
        normalized_filename.push_back(std::tolower(c));
    }

    if (name.length() < 3 || name.substr(name.length() - 3) != ".md") {
        normalized_filename += ".md";
    }

    return normalized_filename;
}

}