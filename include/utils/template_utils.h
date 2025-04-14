#ifndef NOTE_DB_TEMPLATE_UTILS_H
#define NOTE_DB_TEMPLATE_UTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace template_utils {

void GenerateDefaultTemplate(const std::filesystem::path& default_template);
void SetupTemplateDirectory(const std::vector<std::string>& args);

}

#endif //NOTE_DB_TEMPLATE_UTILS_H
