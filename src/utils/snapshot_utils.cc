#include "utils/snapshot_utils.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace snapshot_utils {

void SetupSnapshotDirectory(const std::vector<std::string> &args, const std::unordered_map<std::string, std::string> &flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path template_dir = notes_dir / "templates";
    std::filesystem::create_directories(template_dir);
}

}

