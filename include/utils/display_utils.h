#ifndef NOTE_DB_DISPLAY_UTILS_H
#define NOTE_DB_DISPLAY_UTILS_H

#include "note.h"

#include <string>
#include <vector>

namespace display_utils {

void PrintNotes(std::vector<Note>& notes);
void PrintCommands();
void Debug(std::string debug_message, bool debug);

}

#endif //NOTE_DB_DISPLAY_UTILS_H
