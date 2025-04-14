#ifndef NOTE_DB_MANAGER_H
#define NOTE_DB_MANAGER_H

#include <index.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class NoteManager {
public:
    explicit NoteManager(const std::string& notes_directory, std::shared_ptr<SearchIndex> index);
    ~NoteManager() = default;

    std::filesystem::path get_notes_directory();

    void CreateNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
    void ListNotes();
    void EditNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
    void DeleteNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
    void SearchNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
    void ImportNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);
    void TagNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map);

private:
    std::string editor_;
    std::shared_ptr<SearchIndex> index_;
    std::filesystem::path notes_directory_;

};

#endif //NOTE_DB_MANAGER_H
