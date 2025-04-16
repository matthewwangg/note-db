include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(note_db_tests

)

target_include_directories(note_db_tests PRIVATE ../include)

target_link_libraries(note_db_tests
        PRIVATE
        note_db_lib
        gtest
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(note_db_tests)
