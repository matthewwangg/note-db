#include "manager.h"
#include "note.h"
#include "utils/display_utils.h"

#include <filesystem>
#include <string>
#include <vector>

NoteManager::NoteManager(const std::string& notes_directory)
    : notes_directory_(notes_directory)
{
    editor_ = std::getenv("EDITOR") ? std::getenv("EDITOR") : "nano";

    if (!std::filesystem::exists(notes_directory_)) {
        std::filesystem::create_directories(notes_directory_);
    }
}

void NoteManager::CreateNote(const std::vector<std::string>& args) {
    std::string filename = args[0];
    std::string title = args[0].substr(0, args[0].length() - 3);
    Note new_note = {filename, title, "", {}, std::chrono::system_clock::now(), std::chrono::system_clock::now()};
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

void NoteManager::EditNote(const std::vector<std::string>& args) {
    std::string filename = args[0];
    Note note = Note::LoadFromFile(notes_directory_ / filename);
    note.updated = std::chrono::system_clock::now();
    note.SaveToFile(notes_directory_);

    std::string command = editor_ + " " + notes_directory_.string() + "/" + filename;
    std::system(command.c_str());
}

void NoteManager::DeleteNote(const std::vector<std::string>& args) {
    std::string filename = args[0];
    std::filesystem::path file_path = notes_directory_ / filename;

    if (std::filesystem::exists(file_path)) {
        std::filesystem::remove(file_path);
    }
}

void NoteManager::SearchNote(const std::vector<std::string>& args) {
    std::string search_query = args[0];
    std::vector<Note> notes;

    for (const auto& path : std::filesystem::directory_iterator(notes_directory_)) {
        Note note = Note::LoadFromFile(path);
        if (note.title.find(search_query) != std::string::npos || note.filename.find(search_query) != std::string::npos) {
            notes.push_back(note);
        }
    }

    display_utils::PrintNotes(notes);
}

void NoteManager::TagNote(const std::vector<std::string>& args) {
    std::string filename = args[0];
    std::filesystem::path file_path = notes_directory_ / filename;

    Note note = Note::LoadFromFile(notes_directory_ / filename);
    note.tags.insert(args[1]);
    note.SaveToFile(notes_directory_);
}

