#ifndef NOTE_DB_CLI_H
#define NOTE_DB_CLI_H

#include <string>
#include <vector>

#include "manager.h"

namespace cli {

void HandleNewCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleEditCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleDeleteCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleListCommand(NoteManager& manager);

void HandleSearchCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleSnapshotCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleDiffCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleImportCommand(NoteManager& manager, const std::vector<std::string>& args);
void HandleInitCommand(const std::vector<std::string>& args);
void HandleTagCommand(NoteManager& manager, const std::vector<std::string>& args);

void HandleHelpCommand();

void DispatchCommand(const std::vector<std::string>& tokens);

}

#endif //NOTE_DB_CLI_H
