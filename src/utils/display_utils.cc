#include "utils/display_utils.h"
#include "note.h"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace display_utils {
    void PrintNotes(std::vector<Note>& notes) {
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

    void PrintCommands() {
        std::cout << "Available commands:" << std::endl
                  << "  new <filename>           Create a new note" << std::endl
                  << "  edit <filename>          Edit a note" << std::endl
                  << "  delete <filename>        Delete a note" << std::endl
                  << "  list                     List all notes" << std::endl
                  << "  search <query>           Search note contents" << std::endl
                  << "  tag <filename> <tag>     Add a tag to a note" << std::endl
                  << "  help                     Show this message" << std::endl;
    }

    void Debug(std::string debug_message) {
        std::cout << debug_message << std::endl;
    }
}