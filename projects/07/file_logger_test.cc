#include <gtest/gtest.h>
#include <sstream>

#include "file_logger.h"
#include "mock_fs.h"

using namespace std;
using namespace testing;

class FileLoggerTest : public Test {
protected:
    void SetUp() override {
        MockFS::updateInstance();
    }

    void TearDown() override {
        MockFS::dropInstance();
    }
};

TEST_F(FileLoggerTest, onFlush_shouldCreateFileInPassedDir) {
    FileLogger logger("foo/bar");

    EXPECT_CALL(MockFS::getInstance(), writeFile(StartsWith("foo/bar"), _));

    logger.onFlush({
        Command("bar")
    });
}

TEST_F(FileLoggerTest, onFlush_shouldNotCreateFileOnEmptyBulk) {
    FileLogger logger("foo/bar");

    EXPECT_CALL(MockFS::getInstance(), writeFile).Times(Exactly(0));

    logger.onFlush({});
}

TEST_F(FileLoggerTest, onFlush_shouldUseFirstCommandTimeAsFileName) {
    FileLogger logger("foo");
    Command cmd("bar");
    cmd.timestamp = 100500;

    EXPECT_CALL(MockFS::getInstance(), writeFile(Eq("foo/bulk100500.log"), _));

    logger.onFlush({cmd});
}

TEST_F(FileLoggerTest, onFlush_shouldWriteAllCommands) {
    FileLogger logger("");

    EXPECT_CALL(MockFS::getInstance(), writeFile(_, "bulk: foo, bar\n"));

    logger.onFlush({
        Command("foo"), Command("bar")
    });
}
