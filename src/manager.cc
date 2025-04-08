#include "manager.h"

#include <iostream>
#include <string>
#include <vector>

NoteManager::NoteManager(const std::string& notes_directory) {

}

void NoteManager::CreateNote(const std::vector<std::string>& args) {
    std::cout << "New" << std::endl;
}

void NoteManager::ListNotes() {
    std::cout << "List" << std::endl;
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

