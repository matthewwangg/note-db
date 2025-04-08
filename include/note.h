#ifndef NOTE_DB_NOTE_H
#define NOTE_DB_NOTE_H

#include <string>
#include <unordered_set>
#include <filesystem>

struct Note {
    std::string filename;
    std::string title;
    std::string content;
    std::unordered_set<std::string> tags;

    static Note LoadFromFile(const std::filesystem::path& path);
    void SaveToFile(const std::filesystem::path& path) const;
};

#endif //NOTE_DB_NOTE_H
