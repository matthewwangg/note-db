#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "cli.h"
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

