#ifndef NOTE_DB_SEARCH_UTILS_H
#define NOTE_DB_SEARCH_UTILS_H

#include <string>
#include <unordered_map>

#include "note.h"

namespace search_utils {

bool MatchesTagFilter(const Note& note, const std::string& tag_filter);
std::string GetTagFilter(const std::unordered_map<std::string, std::string>& flag_map);
int GetLimitFilter(const std::unordered_map<std::string, std::string>& flag_map);
std::string GetSortBy(const std::unordered_map<std::string, std::string>& flag_map);
void SortNotes(std::vector<Note>& notes, const std::string& sort_by);

}

#endif //NOTE_DB_SEARCH_UTILS_H
