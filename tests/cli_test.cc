#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "cli.h"

class CliTest : public ::testing::Test {
protected:
    std::string ReadOutput();
    void SetUp() override;
    void TearDown() override;

    std::streambuf* original_buf_;
    std::ofstream out_;
};

void CliTest::SetUp() {
    out_.open("output.txt");
    original_buf_ = std::cout.rdbuf(out_.rdbuf());
}

void CliTest::TearDown() {
    std::cout.rdbuf(original_buf_);
    out_.close();
    std::filesystem::remove("output.txt");
}

std::string CliTest::ReadOutput() {
    std::ifstream in("output.txt");
    return {(std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()};
}

TEST_F(CliTest, HelpCommandPrintsCommands) {
    cli::DispatchCommand({"help"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("new"), std::string::npos);

    EXPECT_NE(contents.find("init"), std::string::npos);
}

TEST_F(CliTest, InitCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully initialized"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, NewCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"new", "test.md", "--editor", "none", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully created"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, EditCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"new", "edit_test.md", "--editor", "none", "--directory", "cli_test_dir"});
    cli::DispatchCommand({"edit", "edit_test.md", "--editor", "none", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully edited"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DeleteCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"new", "delete_test.md", "--editor", "none", "--directory", "cli_test_dir"});
    cli::DispatchCommand({"delete", "delete_test.md", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully deleted"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"new", "tag_test.md", "--editor", "none", "--directory", "cli_test_dir"});
    cli::DispatchCommand({"tag", "tag_test.md", "testtag", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully tagged"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, SnapshotCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"snapshot"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Snapshot successfully saved"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    std::ofstream test_file("temp_import.md");
    test_file << "# Imported Note\n";
    test_file.close();

    cli::DispatchCommand({"import", "temp_import.md", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully imported"), std::string::npos);

    std::filesystem::remove("temp_import.md");
    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, NewCommandFailsWithoutArgs) {
    cli::DispatchCommand({"new"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command new!"), std::string::npos);
}

TEST_F(CliTest, NewCommandFailsIfAlreadyExistsWithoutOverwrite) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"new", "dupe.md", "--editor", "none", "--directory", "cli_test_dir"});
    cli::DispatchCommand({"new", "dupe.md", "--editor", "none", "--directory", "cli_test_dir"});  // no --overwrite
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("already exists"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, EditCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"edit", "ghost.md", "--editor", "none", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist yet"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DeleteCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"delete", "ghost.md", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandFailsWithMissingArgs) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"tag", "missing_tag.md", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command tag!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"tag", "ghost.md", "sometag", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist yet"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, SnapshotCommandFailsWithExtraArgs) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"snapshot", "unexpected_arg"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command snapshot!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DiffCommandFailsWithoutArgs) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"diff"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command diff!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandFailsWithMissingFile) {
    cli::DispatchCommand({"init", "cli_test_dir"});

    cli::DispatchCommand({"import", "no_such_file.md", "--directory", "cli_test_dir"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("File doesn't exist"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandFailsWithoutArgs) {
    cli::DispatchCommand({"import"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command import!"), std::string::npos);
}

TEST_F(CliTest, UnknownCommandPrintsHelp) {
    cli::DispatchCommand({"nonexistentcommand"});
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Unsupported command"), std::string::npos);

    EXPECT_NE(contents.find("new"), std::string::npos);
}
