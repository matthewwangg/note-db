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
    return {};
}

std::vector<std::string> SearchIndex::Tokenize(const std::string& input) {
    return {};
}

