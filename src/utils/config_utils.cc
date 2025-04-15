#include "utils/config_utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace config_utils {

std::filesystem::path GetHomeDirectory() {
#ifdef _WIN32
    const char* home = std::getenv("USERPROFILE");
#else
    const char* home = std::getenv("HOME");
#endif

    return {home};
}

std::string LoadNotesDirectory() {
    std::filesystem::path home_directory = GetHomeDirectory();
    std::ifstream in(home_directory / ".notedb" / "config.json");
    
    if (!in) {
        return "";
    }

    std::string line;
    while (std::getline(in, line)) {
        std::size_t key = line.find("\"root\"");

        if (key != std::string::npos) {
            std::size_t colon = line.find(':', key);
            if (colon == std::string::npos) {
                continue;
            }

            std::size_t start = line.find('"', colon);
            std::size_t end = line.find('"', start + 1);

            if (start != std::string::npos && end != std::string::npos) {
                return line.substr(start + 1, end - start - 1);
            }
        }
    }

    return "";
}

void SetupConfigFile(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    std::filesystem::path home_directory = GetHomeDirectory();
    std::filesystem::path config_dir = home_directory / ".notedb";
    std::filesystem::create_directories(config_dir);

    std::ofstream out(config_dir / "config.json");

    out << "{\n"
        << "  \"root\": \"" << args[0] << "\"\n"
        << "}\n";
}

}
