#ifndef NOTE_DB_INPUT_VALIDATION_UTILS_H
#define NOTE_DB_INPUT_VALIDATION_UTILS_H

#include <string>
#include <vector>

namespace input_validation_utils {

bool IsValidArg(const std::string& arg);
std::string NormalizeFilename(const std::string& name);
bool ValidateArgs(const std::vector<std::string>& args);

} // namespace input_validation_utils

#endif //NOTE_DB_INPUT_VALIDATION_UTILS_H
