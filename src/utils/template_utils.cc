#include "utils/template_utils.h"

#include <filesystem>
#include <string>
#include <vector>

namespace template_utils {

void SetupTemplateDirectory(const std::vector<std::string>& args) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path template_dir = notes_dir / "templates";
    std::filesystem::create_directories(template_dir);
}

}
