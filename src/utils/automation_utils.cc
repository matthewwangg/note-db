#include "utils/automation_utils.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

#include "utils/config_utils.h"

namespace automation_utils {

std::string ExpandCommandVariables(const std::string& input) {
    std::string result = input;

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* tm = std::localtime(&now);

    char date_buf[11];
    std::strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", tm);

    std::string date = date_buf;
    std::string timestamp = std::to_string(now);

    size_t pos;
    while ((pos = result.find("{{date}}")) != std::string::npos) {
        result.replace(pos, 8, date);
    }
    while ((pos = result.find("{{timestamp}}")) != std::string::npos) {
        result.replace(pos, 13, timestamp);
    }

    return result;
}

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
                command.steps.push_back(ExpandCommandVariables(line.substr(q1 + 1, q2 - q1 - 1)));
            }
        }
    }

    return (!command.steps.empty()) ? std::optional{command} : std::nullopt;
}

void SetupBasicCommandFile() {
    std::filesystem::path home_directory = config_utils::GetHomeDirectory();
    std::filesystem::path config_dir = home_directory / ".note-db";

    std::ofstream out(config_dir / "commands.json");

    out << "{\n"
        << "  \"daily\": {\n"
        << "    \"description\": \"Create a new note for today using the daily template\",\n"
        << "    \"steps\": [\n"
        << "      \"new {{date}}.md --template daily --directory daily --editor none\",\n"
        << "      \"tag {{date}}.md daily --directory daily\",\n"
        << "      \"edit {{date}}.md daily --directory daily\"\n"
        << "    ]\n"
        << "  },\n"
        << "  \"scratch\": {\n"
        << "    \"description\": \"Quickly start a blank note for thinking or testing\",\n"
        << "    \"steps\": [\n"
        << "      \"new scratch-{{timestamp}}.md --template default --directory notes --editor none\",\n"
        << "      \"edit scratch-{{timestamp}}.md --directory notes\"\n"
        << "    ]\n"
        << "  }\n"
        << "}\n";
}

} // namespace automation_utils