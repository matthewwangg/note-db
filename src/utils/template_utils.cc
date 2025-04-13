#include "utils/template_utils.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace template_utils {

void GenerateDefaultTemplate(const std::filesystem::path& default_template) {
    std::ofstream out(default_template);
    if (out.is_open()) {
        out << "# Title" << '\n'
            << '\n'
            << "---" << '\n'
            << '\n'
            << "## Summary" << '\n'
            << '\n'
            << "## Notes" << '\n';
    }
}

void SetupTemplateDirectory(const std::vector<std::string>& args) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path template_dir = notes_dir / "templates";
    std::filesystem::create_directories(template_dir);

    GenerateDefaultTemplate(template_dir / "default.md");
}

}
