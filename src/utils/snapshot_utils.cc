#include "utils/snapshot_utils.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace snapshot_utils {

void SetupSnapshotDirectory(const std::vector<std::string> &args, const std::unordered_map<std::string, std::string> &flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path template_dir = notes_dir / "snapshots";
    std::filesystem::create_directories(template_dir);
}

uint64_t FNV1a(const std::string& content) {
    uint64_t hash = 14695981039346656037ull;
    for (char c : content) {
        hash ^= static_cast<uint8_t>(c);
        hash *= 1099511628211ull;
    }
    return hash;
}

std::string ExtractValue(const std::string& json, const std::string& key) {
    size_t key_pos = json.find('\"' + key + '\"');
    if (key_pos == std::string::npos) return "";

    size_t colon = json.find(':', key_pos);
    size_t start = json.find_first_not_of(" \t\n\r", colon + 1);

    if (json[start] == '"') {
        size_t end = json.find('"', start + 1);
        return json.substr(start + 1, end - start - 1);
    } else {
        size_t end = json.find_first_of(",}", start);
        return json.substr(start, end - start);
    }
}

}

