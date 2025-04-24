#ifndef NOTE_DB_CLI_H
#define NOTE_DB_CLI_H

#include <string>
#include <vector>

#include "manager.h"

namespace cli {

bool DispatchCommand(const std::vector<std::string>& tokens);
bool HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleDiffCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleHelpCommand();
bool HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleInitCommand(const std::vector<std::string>& args);
bool HandleListCommand(NoteManager& manager);
bool HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleRunCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleSnapshotCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args);
bool HandleTemplateCommand(NoteManager& manager, const std::vector<std::string>& args);

} // namespace cli

#endif //NOTE_DB_CLI_H
