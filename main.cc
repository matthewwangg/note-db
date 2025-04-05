#include <string>
#include <iostream>

void print_commands() {
    std::cout << "Commands List:" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_commands();
        return 0;
    }

    if (std::string(argv[1]) == "new") {
        std::cout << "Creating new note!" << std::endl;
    } else if (std::string(argv[1]) == "edit") {
        std::cout << "Editing a note!" << std::endl;
    } else if (std::string(argv[1]) == "delete") {
        std::cout << "Deleting a notes" << std::endl;
    } else if (std::string(argv[1]) == "list") {
        std::cout << "Listing existing notes!" << std::endl;
    } else {
        std::cout << "Unsupported command: " << std::string(argv[1]) << std::endl;
    }

    return 0;
}

