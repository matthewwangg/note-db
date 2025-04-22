#ifndef NOTE_DB_TEMPLATE_UTILS_H
#define NOTE_DB_TEMPLATE_UTILS_H

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace template_utils {

void GenerateDefaultTemplate(const std::filesystem::path& default_template);
void SetupTemplateDirectory(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
void CreateUniqueTemplate(std::string editor, const std::filesystem::path& template_path);

}

#endif //NOTE_DB_TEMPLATE_UTILS_H
