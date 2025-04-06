#ifndef NOTE_DB_CLI_H
#define NOTE_DB_CLI_H

#include <string>
#include <vector>

namespace cli {

void DispatchCommand(const std::vector<std::string>& tokens);
void HandleHelpCommand();

}

#endif //NOTE_DB_CLI_H
