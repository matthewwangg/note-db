#include "utils/template_utils.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "manager.h"

namespace template_utils {

void GenerateDefaultTemplate(const std::filesystem::path& default_template) {
    std::ofstream out(default_template);
    if (out.is_open()) {
        out << "# Title" << '\n'
            << "This is a short description of what the note is for." << '\n'
            << " \n"
            << "## Summary" << '\n'
            << "This is a summary of the information in the note." << '\n'
            << " \n"
            << "## Notes" << '\n'
            << " \n"
            << "- Here is the start of the notes" << '\n';
    }
}

void SetupTemplateDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    std::filesystem::path notes_dir = args[0];
    std::filesystem::path template_dir = notes_dir / "templates";
    std::filesystem::create_directories(template_dir);

    GenerateDefaultTemplate(template_dir / "default.md");
}

void CreateUniqueTemplate(NoteManager& manager, const std::filesystem::path& template_path) {
    std::filesystem::create_directories(template_path.parent_path());
    std::ofstream out(template_path);

    std::string command = manager.GetEditor() + " " + (template_path).string();
    std::system(command.c_str());
}

}
