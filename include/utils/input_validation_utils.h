#ifndef NOTE_DB_INPUT_VALIDATION_UTILS_H
#define NOTE_DB_INPUT_VALIDATION_UTILS_H

#include <string>
#include <vector>

namespace input_validation_utils {
    bool ValidateArgs(const std::vector<std::string>& args);
    bool IsValidArg(const std::string& arg);
}

#endif //NOTE_DB_INPUT_VALIDATION_UTILS_H
