#include <string>
#include <vector>

#include "cli.h"

int main(int argc, char* argv[]) {

    std::vector<std::string> command_args(argv + 1, argv + argc);
    bool successful = cli::DispatchCommand(command_args, false);

    return successful ? 0 : 1;
}

