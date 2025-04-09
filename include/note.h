#ifndef NOTE_DB_NOTE_H
#define NOTE_DB_NOTE_H

#include <chrono>
#include <string>
#include <unordered_set>
#include <filesystem>

struct Note {
    std::string filename;
    std::string title;
    std::string content;
    std::unordered_set<std::string> tags;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point updated;

    static Note LoadFromFile(const std::filesystem::path& path);
    void SaveToFile(const std::filesystem::path& path) const;
};

#endif //NOTE_DB_NOTE_H
