#ifndef NOTE_DB_MANAGER_H
#define NOTE_DB_MANAGER_H

#include <string>
#include <filesystem>
#include <vector>

class NoteManager {
public:
    explicit NoteManager(const std::string& notes_directory);
    ~NoteManager() = default;

    void CreateNote(const std::vector<std::string>& args);
    void ListNotes();
    void EditNote(const std::vector<std::string>& args);
    void DeleteNote(const std::vector<std::string>& args);
    void SearchNote(const std::vector<std::string>& args);
    void TagNote(const std::vector<std::string>& args);

private:
    std::filesystem::path notes_directory_;
    std::string editor_;
};

#endif //NOTE_DB_MANAGER_H
