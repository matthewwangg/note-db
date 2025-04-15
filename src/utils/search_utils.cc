#include "utils/search_utils.h"

#include <algorithm>
#include <functional>
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

std::string GetSortBy(const std::unordered_map<std::string, std::string>& flag_map) {
    std::string sort_by;

    auto it = flag_map.find("--sort-by");
    if (it != flag_map.end()) {
        sort_by = it->second;
    } else {
        sort_by = "";
    }

    return sort_by;
}

void SortNotes(std::vector<Note>& notes, const std::string& sort_by) {
    std::function<bool(const Note&, const Note&)> comparator;

    if (sort_by == "created") {
        comparator = [](const Note& a, const Note& b) {
            return a.created < b.created;
        };
    } else if (sort_by == "updated") {
        comparator = [](const Note& a, const Note& b) {
            return a.updated < b.updated;
        };
    } else if (sort_by == "title") {
        comparator = [](const Note& a, const Note& b) {
            return a.title < b.title;
        };
    } else {
        comparator = [](const Note& a, const Note& b) {
            return a.filename < b.filename;
        };
    }

    std::sort(notes.begin(), notes.end(), comparator);
}

}