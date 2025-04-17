#include <chrono>
#include <filesystem>
#include <gtest/gtest.h>
#include <string>
#include <thread>

#include "cli.h"
#include "note.h"
#include "utils/config_utils.h"

class CliTest : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;

    std::string saved_notes_directory_;
    std::filesystem::path test_directory_;
};

void CliTest::SetUp() {
    saved_notes_directory_ = config_utils::LoadNotesDirectory();
    test_directory_ = std::filesystem::current_path() / ".." / "tests" / "temp";
    std::filesystem::remove_all(test_directory_);
    std::filesystem::create_directory(test_directory_);
    cli::DispatchCommand({"init", test_directory_.string()});
}

void CliTest::TearDown() {
    std::filesystem::remove_all(test_directory_);
    config_utils::SetupConfigFile({saved_notes_directory_}, {});
}

TEST_F(CliTest, NewCommandCreatesFile) {
    std::string filename = "test_create.md";
    std::filesystem::path expected_path = test_directory_ / filename;

    ASSERT_FALSE(std::filesystem::exists(expected_path));

    cli::DispatchCommand({"new", filename, "--editor", "none"});

    EXPECT_TRUE(std::filesystem::exists(expected_path));
}

TEST_F(CliTest, EditCommandUpdatesFile) {
    std::string filename = "test_edit.md";
    std::filesystem::path expected_path = test_directory_ / filename;

    cli::DispatchCommand({"new", filename, "--editor", "none"});

    ASSERT_TRUE(std::filesystem::exists(expected_path));

    auto original_write_time = std::filesystem::last_write_time(expected_path);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cli::DispatchCommand({"edit", filename, "--editor", "none"});
    auto new_write_time = std::filesystem::last_write_time(expected_path);

    EXPECT_GT(new_write_time, original_write_time);
}

TEST_F(CliTest, DeleteCommandRemovesFile) {
    std::string filename = "test_delete.md";
    std::filesystem::path expected_path = test_directory_ / filename;

    cli::DispatchCommand({"new", filename, "--editor", "none"});

    ASSERT_TRUE(std::filesystem::exists(expected_path));

    cli::DispatchCommand({"delete", filename});

    EXPECT_FALSE(std::filesystem::exists(expected_path));
}