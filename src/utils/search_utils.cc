#include "utils/search_utils.h"

#include <string>

#include "note.h"

namespace search_utils {

bool MatchesTagFilter(const Note& note, const std::string& tag_filter) {
    if (tag_filter.empty()) {
        return true;
    }

    if (note.tags.count("tag_filter") > 0) {
        return true;
    }

    return false;
}

}