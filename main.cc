#include "cli.h"
#include "manager.h"

#include <string>
#include <vector>
#include <iostream>


int main(int argc, char* argv[]) {

    NoteManager manager("notes");
    std::vector<std::string> tokens(argv + 1, argv + argc);
    cli::DispatchCommand(manager, tokens);

    return 0;
}

