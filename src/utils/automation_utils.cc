#include "utils/automation_utils.h"

#include <filesystem>
#include <fstream>
#include <optional>

#include "utils/config_utils.h"

namespace automation_utils {

std::optional<CommandDefinition> LoadCommandByName(const std::string& name) {
    std::filesystem::path path = config_utils::GetHomeDirectory() / ".note-db" / "commands.json";

    std::ifstream file(path);

    CommandDefinition command;
    std::string line;
    bool found_target = false;
    bool parsing_steps = false;

    while (std::getline(file, line)) {
        if (!found_target && line.find('\"' + name + '\"') != std::string::npos) {
            found_target = true;
            continue;
        }

        if (!found_target) {
            continue;
        }

        if (line.find("\"description\"") != std::string::npos) {
            auto start = line.find('"', line.find(':'));
            auto end = line.find('"', start + 1);
            command.description = line.substr(start + 1, end - start - 1);
        } else if (line.find("\"steps\"") != std::string::npos) {
            parsing_steps = true;
            continue;
        }

        if (parsing_steps) {
            if (line.find(']') != std::string::npos) break;
            auto q1 = line.find('"');
            auto q2 = line.find('"', q1 + 1);
            if (q1 != std::string::npos && q2 != std::string::npos) {
                command.steps.push_back(line.substr(q1 + 1, q2 - q1 - 1));
            }
        }
    }

    return (!command.steps.empty()) ? std::optional{command} : std::nullopt;
}

} // namespace automation_utils