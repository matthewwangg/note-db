#ifndef NOTE_DB_DISPLAY_UTILS_H
#define NOTE_DB_DISPLAY_UTILS_H

#include <string>
#include <vector>

#include "note.h"

namespace display_utils {

void Debug(const std::string& debug_message, bool debug);
void PrintCommands();
void PrintError(const std::string& message);
void PrintInfo(const std::string& message);
void PrintNotes(std::vector<Note>& notes);
void PrintSuccess(const std::string& message);
void PrintWarning(const std::string& message);

} // namespace display_utils

#endif //NOTE_DB_DISPLAY_UTILS_H
