cmake_minimum_required(VERSION 3.27)
project(note_db)

set(CMAKE_CXX_STANDARD 20)

add_executable(note_db
        main.cc
        src/cli.cc
        src/note.cc
        src/manager.cc
        src/utils/display_utils.cc
        src/utils/input_validation_utils.cc
        src/index.cc
        src/utils/config_utils.cc
        src/utils/template_utils.cc
        src/utils/command_utils.cc
        src/utils/search_utils.cc)

target_include_directories(note_db PRIVATE include)
