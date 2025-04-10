#include "cli.h"

#include <string>
#include <vector>


int main(int argc, char* argv[]) {

    std::vector<std::string> command_args(argv + 1, argv + argc);
    cli::DispatchCommand(command_args);

    return 0;
}

