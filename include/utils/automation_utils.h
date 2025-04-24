#ifndef NOTE_DB_AUTOMATION_UTILS_H
#define NOTE_DB_AUTOMATION_UTILS_H

#include <optional>
#include <string>
#include <vector>

struct CommandDefinition {
    std::string name;
    std::string description;
    std::vector<std::string> steps;
};

namespace automation_utils {

std::string ExpandCommandVariables(const std::string& input);
std::optional<CommandDefinition> LoadCommandByName(const std::string& name);
void SetupBasicCommandFile();

}  // namespace automation_utils

#endif //NOTE_DB_AUTOMATION_UTILS_H
