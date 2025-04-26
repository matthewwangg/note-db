#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "cli.h"
#include "utils/automation_utils.h"
#include "utils/backup_utils.h"
#include "utils/config_utils.h"
#include "utils/snapshot_utils.h"
#include "utils/template_utils.h"

class CliTest : public ::testing::Test {
protected:
    std::string ReadOutput();
    void SetUp() override;
    void TearDown() override;

    std::streambuf* original_buf_;
    std::ofstream out_;
    std::string saved_notes_directory_;
};

void CliTest::SetUp() {
    saved_notes_directory_ = config_utils::LoadNotesDirectory();
    config_utils::SetupConfigFile({"cli_test_dir"}, {});
    automation_utils::SetupBasicCommandFile();
    template_utils::SetupTemplateDirectory({"cli_test_dir"}, {});
    snapshot_utils::SetupSnapshotDirectory({"cli_test_dir"}, {});
    backup_utils::SetupBackupDirectory({"cli_test_dir"}, {});

    out_.open("output.txt");
    original_buf_ = std::cout.rdbuf(out_.rdbuf());
}

void CliTest::TearDown() {
    std::cout.rdbuf(original_buf_);
    out_.close();
    std::filesystem::remove("output.txt");

    config_utils::SetupConfigFile({saved_notes_directory_}, {});
}

std::string CliTest::ReadOutput() {
    std::ifstream in("output.txt");
    return {(std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()};
}

TEST_F(CliTest, HelpCommandPrintsCommands) {
    cli::DispatchCommand({"help"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("new"), std::string::npos);

    EXPECT_NE(contents.find("init"), std::string::npos);
}

TEST_F(CliTest, InitCommandPrintsSuccess) {
    cli::DispatchCommand({"init", "notes"}, false);
    cli::DispatchCommand({"init", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully initialized"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, NewCommandPrintsSuccess) {
    cli::DispatchCommand({"new", "test.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully created"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, EditCommandPrintsSuccess) {
    cli::DispatchCommand({"new", "edit_test.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    cli::DispatchCommand({"edit", "edit_test.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully edited"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DeleteCommandPrintsSuccess) {
    cli::DispatchCommand({"new", "delete_test.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    cli::DispatchCommand({"delete", "delete_test.md", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully deleted"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandPrintsSuccess) {
    cli::DispatchCommand({"new", "tag_test.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    cli::DispatchCommand({"tag", "tag_test.md", "testtag", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully tagged"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, SnapshotCommandPrintsSuccess) {
    cli::DispatchCommand({"snapshot"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully saved"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandPrintsSuccess) {
    std::ofstream test_file("temp_import.md");
    test_file << "# Imported Note\n";
    test_file.close();

    cli::DispatchCommand({"import", "temp_import.md", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("successfully imported"), std::string::npos);

    std::filesystem::remove("temp_import.md");
    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, BackupCommandPrintsSuccess) {
    cli::DispatchCommand({"backup"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Backup"), std::string::npos);
    EXPECT_NE(contents.find("created successfully"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, RestoreCommandPrintsSuccess) {
    cli::DispatchCommand({"backup"}, false);

    std::filesystem::path backup_file;
    for (const auto& entry : std::filesystem::directory_iterator("cli_test_dir/.backups")) {
        backup_file = entry.path().filename();
    }

    ASSERT_FALSE(backup_file.empty());

    cli::DispatchCommand({"restore", backup_file.string()}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Backup restored successfully"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}


TEST_F(CliTest, NewCommandFailsWithoutArgs) {
    cli::DispatchCommand({"new"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command new!"), std::string::npos);
}

TEST_F(CliTest, NewCommandFailsIfAlreadyExistsWithoutOverwrite) {
    cli::DispatchCommand({"new", "dupe.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    cli::DispatchCommand({"new", "dupe.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("already exists"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, EditCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"edit", "ghost.md", "--editor", "none", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist yet"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DeleteCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"delete", "ghost.md", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandFailsWithMissingArgs) {
    cli::DispatchCommand({"tag", "missing_tag.md", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command tag!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, TagCommandFailsIfNoteDoesNotExist) {
    cli::DispatchCommand({"tag", "ghost.md", "sometag", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("doesn't exist yet"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, SnapshotCommandFailsWithExtraArgs) {
    cli::DispatchCommand({"snapshot", "unexpected_arg"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command snapshot!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, DiffCommandFailsWithoutArgs) {
    cli::DispatchCommand({"diff"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command diff!"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandFailsWithMissingFile) {
    cli::DispatchCommand({"import", "no_such_file.md", "--directory", "cli_test_dir"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("File doesn't exist"), std::string::npos);

    std::filesystem::remove_all("cli_test_dir");
}

TEST_F(CliTest, ImportCommandFailsWithoutArgs) {
    cli::DispatchCommand({"import"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Invalid usage of command import!"), std::string::npos);
}

TEST_F(CliTest, UnknownCommandPrintsHelp) {
    cli::DispatchCommand({"nonexistentcommand"}, false);
    std::string contents = ReadOutput();

    EXPECT_NE(contents.find("Unsupported command"), std::string::npos);

    EXPECT_NE(contents.find("new"), std::string::npos);
}


