#include "note.h"

#include <filesystem>
#include <fstream>


Note Note::LoadFromFile(const std::filesystem::path& path) {
    Note note;
    note.filename = path.filename().string();
    std::ifstream in(path);

    if (!in) {
        return note;
    }

    std::string line;
    bool first_line = true;
    while(std::getline(in, line)) {
        if (first_line) {
            if (line.rfind("# ", 0) == 0) {
                note.title = line.substr(2);
            } else {
                note.title = line;
            }
            first_line = false;
        } else {
            note.content += line + "\n";
        }
    }

    return note;
}

void Note::SaveToFile(const std::filesystem::path& path) const {
    std::filesystem::path final_path = path / filename;
    std::ofstream out(final_path);

    if (!out) {
        return;
    }

    out << "# " << title << std::endl;
    out << content;
}