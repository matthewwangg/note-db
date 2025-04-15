#include "manager.h"

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "index.h"
#include "note.h"
#include "utils/display_utils.h"
#include "utils/search_utils.h"

NoteManager::NoteManager(const std::string& notes_directory, std::shared_ptr<SearchIndex> index)
    : notes_directory_(notes_directory),
      index_(std::move(index))
{
    editor_ = std::getenv("NOTEDB_EDITOR") ? std::getenv("NOTEDB_EDITOR") : "nano";

    if (!std::filesystem::exists(notes_directory_)) {
        std::filesystem::create_directories(notes_directory_);
    }
}

void NoteManager::CreateNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    std::string title = args[0].substr(0, args[0].length() - 3);

    Note new_note = {filename, title, "", {}, std::chrono::system_clock::now(), std::chrono::system_clock::now()};
    if (!flag_map.empty() && flag_map.find("--template") != flag_map.end()) {
        new_note.content = Note::LoadFromFile(notes_directory_ / "templates" / (flag_map.at("--template") + ".md")).content;
    }
    new_note.SaveToFile(notes_directory_);

    std::string command = editor_ + " " + notes_directory_.string() + "/" + filename;
    std::system(command.c_str());
}

void NoteManager::ListNotes() {
    std::vector<Note> notes;

    for(auto const& path : std::filesystem::directory_iterator(notes_directory_)) {
        notes.push_back(Note::LoadFromFile(path));
    }

    display_utils::PrintNotes(notes);
}

void NoteManager::EditNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    Note note = Note::LoadFromFile(notes_directory_ / filename);
    note.updated = std::chrono::system_clock::now();

    note.SaveToFile(notes_directory_);

    std::string command = editor_ + " " + notes_directory_.string() + "/" + filename;
    std::system(command.c_str());
}

void NoteManager::DeleteNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    std::filesystem::path file_path = notes_directory_ / filename;

    if (std::filesystem::exists(file_path)) {
        std::filesystem::remove(file_path);
    }
}

void NoteManager::SearchNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& search_query = args[0];
    const std::vector<std::string> filenames = index_->Search(search_query);
    std::string tag_filter = search_utils::GetTagFilter(flag_map);
    int result_limit = search_utils::GetLimitFilter(flag_map);

    std::vector<Note> notes;

    for (const auto& path : std::filesystem::directory_iterator(notes_directory_)) {
        Note note = Note::LoadFromFile(path);
        bool note_exists = std::find(filenames.begin(), filenames.end(), note.filename) != filenames.end();
        if (note_exists && search_utils::MatchesTagFilter(note, tag_filter) && notes.size() < result_limit) {
            notes.push_back(note);
        }
    }

    display_utils::PrintNotes(notes);
}

void NoteManager::ImportNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::filesystem::path file_path = args[0];
    const std::string filename = file_path.filename().string();

    Note note = Note::LoadFromFile(file_path);
    note.title = filename.substr(0, filename.length() - 3);
    note.created = std::chrono::system_clock::now();
    note.updated = std::chrono::system_clock::now();
    note.SaveToFile(notes_directory_);
}

void NoteManager::TagNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    std::filesystem::path file_path = notes_directory_ / filename;

    Note note = Note::LoadFromFile(notes_directory_ / filename);
    note.tags.insert(args[1]);
    note.SaveToFile(notes_directory_);
}

std::filesystem::path NoteManager::GetNotesDirectory() {
    return notes_directory_;
}

