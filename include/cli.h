#ifndef NOTE_DB_CLI_H
#define NOTE_DB_CLI_H

#include <string>
#include <vector>

namespace cli {

void HandleNewCommand(const std::vector<std::string>& args);
void HandleEditCommand(const std::vector<std::string>& args);
void HandleDeleteCommand(const std::vector<std::string>& args);
void HandleListCommand();

void HandleSearchCommand(const std::vector<std::string>& args);
void HandleTagCommand(const std::vector<std::string>& args);

void HandleHelpCommand();

void DispatchCommand(const std::vector<std::string>& tokens);

}

#endif //NOTE_DB_CLI_H
