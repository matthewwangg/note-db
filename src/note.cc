#include "note.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

Note Note::LoadFromFile(const std::filesystem::path& path) {
    Note note;
    note.filename = path.filename().string();
    std::ifstream in(path);

    if (!in) {
        return note;
    }

    std::string line;
    bool metadata = false;
    while(std::getline(in, line)) {
        if (line == "---") {
            metadata = !metadata;
        } else if (metadata) {
            if (line.rfind("title: ", 0) == 0) {
                note.title = line.substr(7);
            } else if (line.rfind("created: ", 0) == 0 || line.rfind("updated: ", 0) == 0) {
                std::string date_string = line.substr(9);

                int year = std::stoi(date_string.substr(0, 4));
                int month = std::stoi(date_string.substr(5, 2));
                int day = std::stoi(date_string.substr(8, 2));
                std::chrono::sys_days date = std::chrono::year(year) / month / day;

                if (line.rfind("created: ", 0) == 0) {
                    note.created = date;
                } else if (line.rfind("updated: ", 0) == 0) {
                    note.updated = date;
                }
            } else if (line.rfind("tags: [", 0) == 0) {
                std::string tags = line.substr(7, line.size() - 8);

                std::string delimiter = ", ";
                size_t start = 0;
                size_t end;

                while ((end = tags.find(delimiter, start)) != std::string::npos) {
                    std::string tag = tags.substr(start, end - start);
                    note.tags.insert(tag);
                    start = end + delimiter.length();
                }
                std::string final_tag = tags.substr(start);
                if (!final_tag.empty()) {
                    note.tags.insert(final_tag);
                }
            } else {
                continue;
            }
        } else {
            note.content += line + "\n";
        }
    }

    return note;
}

void Note::SaveToFile(const std::filesystem::path& path) const {
    std::filesystem::path final_path = path / filename;
    std::ofstream out(final_path, std::ios::binary);

    if (!out) {
        return;
    }

    const auto created_day = std::chrono::floor<std::chrono::days>(created);
    const auto updated_day = std::chrono::floor<std::chrono::days>(updated);

    out << "---" << std::endl;
    out << "title: " << title << std::endl;
    out << "created: " << created_day << std::endl;
    out << "updated: " << updated_day << std::endl;
    out << "tags: [";
    bool first = true;
    for (const std::string &tag: tags) {
        if (!first) {
            out << ", ";
        }
        out << tag;
        first = false;
    }
    out << "]" << std::endl;
    out << "---" << std::endl;
    out << content;
}