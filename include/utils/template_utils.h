#ifndef NOTE_DB_TEMPLATE_UTILS_H
#define NOTE_DB_TEMPLATE_UTILS_H

#include <string>
#include <vector>

namespace template_utils {

void GenerateDefaultTemplate();
void SetupTemplateDirectory(const std::vector<std::string>& args);

}

#endif //NOTE_DB_TEMPLATE_UTILS_H
