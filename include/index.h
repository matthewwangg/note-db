#ifndef NOTE_DB_INDEX_H
#define NOTE_DB_INDEX_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class SearchIndex {
public:
    std::vector<std::string> Search(const std::string& query);

private:
    std::vector<std::string> Tokenize(const std::string& input);

    std::unordered_map<std::string, std::unordered_set<std::string>> index_;
};

#endif //NOTE_DB_INDEX_H
