#include "utils/search_utils.h"

#include <algorithm>
#include <limits>
#include <string>
#include <unordered_map>

#include "note.h"

namespace search_utils {

bool MatchesTagFilter(const Note& note, const std::string& tag_filter) {
    if (tag_filter.empty()) {
        return true;
    }

    if (note.tags.count(tag_filter) > 0) {
        return true;
    }

    return false;
}

std::string GetTagFilter(const std::unordered_map<std::string, std::string>& flag_map) {
    std::string tag_filter;

    auto it = flag_map.find("--tag");
    if (it != flag_map.end()) {
        tag_filter = it->second;
    } else {
        tag_filter = "";
    }

    return tag_filter;
}

int GetLimitFilter(const std::unordered_map<std::string, std::string>& flag_map) {
    int limit;

    auto it = flag_map.find("--limit");
    if (it != flag_map.end()) {
        limit = std::max(0, std::stoi(it->second));
    } else {
        limit = std::numeric_limits<int>::max();
    }

    return limit;
}

}