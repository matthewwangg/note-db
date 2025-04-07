#ifndef NOTE_DB_NOTE_H
#define NOTE_DB_NOTE_H

#include <string>
#include <unordered_set>

struct Note {
    std::string filename;
    std::string title;
    std::string content;
    std::unordered_set<std::string> tags;


};

#endif //NOTE_DB_NOTE_H
