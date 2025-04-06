cmake_minimum_required(VERSION 3.27)
project(note_db)

set(CMAKE_CXX_STANDARD 14)

add_executable(note_db
        main.cc
        src/cli.cc)

target_include_directories(note_db PRIVATE include)
