#ifndef NOTE_DB_SEARCH_UTILS_H
#define NOTE_DB_SEARCH_UTILS_H

#include <string>

#include "note.h"

namespace search_utils {

bool MatchesTagFilter(const Note& note, const std::string& tag_filter);

}

#endif //NOTE_DB_SEARCH_UTILS_H
