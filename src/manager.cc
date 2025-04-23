#include "manager.h"

#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "index.h"
#include "note.h"
#include "utils/command_utils.h"
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
    std::filesystem::path path = command_utils::ResolveDirectory(notes_directory_, flag_map);
    std::filesystem::create_directories(path);
    new_note.SaveToFile(path);

    if (flag_map.count("--editor") && flag_map.at("--editor") == "none") {
        return;
    }

    if (flag_map.count("--editor")) {
        editor_ = flag_map.at("--editor");
    }

    std::string command = editor_ + " " + (path / filename).string();
    std::system(command.c_str());
}

void NoteManager::DeleteNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    std::filesystem::path path = command_utils::ResolveDirectory(notes_directory_, flag_map);

    if (std::filesystem::exists(path / filename)) {
        std::filesystem::remove(path / filename);
    }
}

void NoteManager::EditNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];

    std::filesystem::path path = command_utils::ResolveDirectory(notes_directory_, flag_map);
    std::filesystem::create_directories(path);

    Note note = Note::LoadFromFile(path / filename);
    note.updated = std::chrono::system_clock::now();

    note.SaveToFile(path);

    if (flag_map.count("--editor") && flag_map.at("--editor") == "none") {
        return;
    }

    if (flag_map.count("--editor")) {
        editor_ = flag_map.at("--editor");
    }

    std::string command = editor_ + " " + (path / filename).string();
    std::system(command.c_str());
}

void NoteManager::ImportNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::filesystem::path file_path = args[0];
    const std::string filename = file_path.filename().string();

    Note note = Note::LoadFromFile(file_path);
    note.title = filename.substr(0, filename.length() - 3);
    note.created = std::chrono::system_clock::now();
    note.updated = std::chrono::system_clock::now();

    std::filesystem::path path = command_utils::ResolveDirectory(notes_directory_, flag_map);
    std::filesystem::create_directories(path);
    note.SaveToFile(path);
}

void NoteManager::ListNotes() {
    std::vector<Note> notes;

    for(auto const& path : std::filesystem::recursive_directory_iterator(notes_directory_)) {
        if (!path.is_regular_file() || path.path().extension().string() != ".md") {
            continue;
        }
        notes.push_back(Note::LoadFromFile(path));
    }

    display_utils::PrintNotes(notes);
}

void NoteManager::SearchNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& search_query = args[0];
    const std::vector<std::string> filenames = index_->Search(search_query);
    std::string tag_filter = search_utils::GetTagFilter(flag_map);
    int result_limit = search_utils::GetLimitFilter(flag_map);
    std::string sort_by = search_utils::GetSortBy(flag_map);

    std::vector<Note> notes;

    for (const auto& path : std::filesystem::recursive_directory_iterator(notes_directory_)) {
        if (!path.is_regular_file() || path.path().extension().string() != ".md") {
            continue;
        }
        Note note = Note::LoadFromFile(path);
        bool note_exists = std::find(filenames.begin(), filenames.end(), note.filename) != filenames.end();
        if (note_exists && search_utils::MatchesTagFilter(note, tag_filter) && notes.size() < result_limit) {
            notes.push_back(note);
        }
    }

    if (!sort_by.empty()) {
        search_utils::SortNotes(notes, sort_by);
    }

    display_utils::PrintNotes(notes);
}

void NoteManager::TagNote(const std::vector<std::string>& args, const std::unordered_map<std::string, std::string>& flag_map) {
    const std::string& filename = args[0];
    std::filesystem::path path = command_utils::ResolveDirectory(notes_directory_, flag_map);
    std::filesystem::create_directories(path);

    Note note = Note::LoadFromFile(path / filename);
    note.tags.insert(args[1]);
    note.SaveToFile(path);
}

std::string NoteManager::GetEditor() {
    return editor_;
}

std::filesystem::path NoteManager::GetNotesDirectory() {
    return notes_directory_;
}

