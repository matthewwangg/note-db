cmake_minimum_required(VERSION 3.27)
project(note_db)

set(CMAKE_CXX_STANDARD 20)

add_library(note_db_lib
        src/cli.cc
        src/note.cc
        src/manager.cc
        src/index.cc
        src/snapshot.cc
        src/snapshot_entry.cc
        src/utils/display_utils.cc
        src/utils/input_validation_utils.cc
        src/utils/config_utils.cc
        src/utils/template_utils.cc
        src/utils/command_utils.cc
        src/utils/search_utils.cc
        src/utils/snapshot_utils.cc
        src/utils/automation_utils.cc
)

target_include_directories(note_db_lib PRIVATE include)

add_executable(note_db main.cc)
target_link_libraries(note_db PRIVATE note_db_lib)
target_include_directories(note_db PRIVATE include)

option(BUILD_TESTING "Build the test suite" OFF)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
