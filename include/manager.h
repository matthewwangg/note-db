#ifndef NOTE_DB_MANAGER_H
#define NOTE_DB_MANAGER_H

#include <index.h>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class NoteManager {
public:
    explicit NoteManager(const std::string& notes_directory, std::shared_ptr<SearchIndex> index);
    ~NoteManager() = default;

    void CreateNote(const std::vector<std::string>& args);
    void ListNotes();
    void EditNote(const std::vector<std::string>& args);
    void DeleteNote(const std::vector<std::string>& args);
    void SearchNote(const std::vector<std::string>& args);
    void TagNote(const std::vector<std::string>& args);

private:
    std::string editor_;
    std::shared_ptr<SearchIndex> index_;
    std::filesystem::path notes_directory_;

};

#endif //NOTE_DB_MANAGER_H
