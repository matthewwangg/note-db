#ifndef NOTE_DB_MANAGER_H
#define NOTE_DB_MANAGER_H

#include <string>

class NoteManager {
public:
    explicit NoteManager(const std::string& notes_directory);
    ~NoteManager() = default;

};

#endif //NOTE_DB_MANAGER_H
