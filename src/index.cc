#include "index.h"
#include "note.h"

#include <filesystem>
#include <string>
#include <vector>

void SearchIndex::BuildIndex(const std::filesystem::path& notes_directory) {
    for (const auto& path : std::filesystem::directory_iterator(notes_directory)) {
        Note note = Note::LoadFromFile(path);
        IndexNote(note);
    }
}

void SearchIndex::IndexNote(const Note& note) {
    std::vector<std::string> tokens = Tokenize(note.content);
    tokens.push_back(note.title);
    tokens.push_back(note.filename);

    for (const std::string& token : tokens) {
        index_[token].insert(note.filename);
    }
}

std::vector<std::string> SearchIndex::Search(const std::string &query) {
    std::vector<std::string> processed_query = Tokenize(query);

    if (processed_query.empty()) {
        return {};
    }

    const std::string& query_token = processed_query[0];
    auto it = index_.find(query_token);

    if (it == index_.end()) {
        return {};
    }

    return {it->second.begin(), it->second.end()};
}

std::vector<std::string> SearchIndex::Tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current_token;

    for (int i = 0; i < input.length(); i++) {
        if (std::isalnum(input[i])) {
            current_token += std::tolower(input[i]);
        } else {
            tokens.push_back(current_token);
            current_token = "";
        }
    }

    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }

    return tokens;
}

