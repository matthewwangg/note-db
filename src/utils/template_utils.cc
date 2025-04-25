#include "utils/template_utils.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "manager.h"

namespace template_utils {

void CreateUniqueTemplate(std::string editor, const std::filesystem::path& template_path) {
    std::filesystem::create_directories(template_path.parent_path());
    std::ofstream out(template_path);

    std::string command = editor + " " + (template_path).string();
    std::system(command.c_str());
}

void GenerateDailyTemplate(const std::filesystem::path& daily_template) {
    std::ofstream out(daily_template);
    if (out.is_open()) {
        out << "# Title" << '\n'
            << "This is a short description of what today's focus is" << '\n'
            << " \n"
            << "## Today's Agenda" << '\n'
            << "- Item 1" << '\n'
            << "- Item 2" << '\n'
            << " \n"
            << "## Notes" << '\n'
            << " \n"
            << "- Here is the start of the notes" << '\n';
    }
}

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
    std::filesystem::path template_dir = notes_dir / ".templates";
    std::filesystem::create_directories(template_dir);

    GenerateDefaultTemplate(template_dir / "default.md");
    GenerateDailyTemplate(template_dir / "daily.md");
}

}
