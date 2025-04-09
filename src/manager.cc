#include "manager.h"
#include "note.h"

#include <filesystem>
#include <iostream>
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
    std::string filename = args[0] + ".md";
    Note new_note = {filename, args[0], "", {}, std::chrono::system_clock::now(), std::chrono::system_clock::now()};
    new_note.SaveToFile(notes_directory_);
    std::string command = editor_ + " " + notes_directory_.string() + "/" + filename;
    std::system(command.c_str());
}

void NoteManager::ListNotes() {
    std::vector<Note> notes;
    for(auto const& path : std::filesystem::directory_iterator(notes_directory_)) {
        notes.push_back(Note::LoadFromFile(path));
    }

    const int filename_width = 20;
    const int title_width = 20;
    const int date_width = 20;
    const int content_preview_width = 50;

    std::cout << std::left
              << std::setw(filename_width) << "Filename"
              << std::setw(title_width) << "Title"
              << std::setw(date_width) << "Created"
              << std::setw(date_width) << "Updated"
              << std::setw(content_preview_width) << "Preview"
              << std::endl;

    for (const Note& n : notes) {
        auto created_day = std::chrono::floor<std::chrono::days>(n.created);
        auto updated_day = std::chrono::floor<std::chrono::days>(n.updated);
        auto preview_length = std::min<size_t>(static_cast<size_t>(content_preview_width - 3), n.content.find('\n'));
        auto content_preview = n.content.substr(0, preview_length) + "...";

        std::cout << std::left
                  << std::setw(filename_width) << n.filename
                  << std::setw(title_width) << n.title
                  << std::setw(date_width) << created_day
                  << std::setw(date_width) << updated_day
                  << std::setw(content_preview_width) << content_preview
                  << std::endl;
    }
}

void NoteManager::EditNote(const std::vector<std::string>& args) {
    std::cout << "Edit" << std::endl;
}

void NoteManager::DeleteNote(const std::vector<std::string>& args) {
    std::cout << "Delete" << std::endl;
}

void NoteManager::SearchNote(const std::vector<std::string>& args) {
    std::cout << "Search" << std::endl;
}

void NoteManager::TagNote(const std::vector<std::string>& args) {
    std::cout << "Tag" << std::endl;
}

