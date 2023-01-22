#include <gtest/gtest.h>
#include "fs_adapter.h"
#include "mock_fs.h"

using namespace std;
using namespace testing;

namespace fs = filesystem;

TEST(FSAdapter_scanDirs, ShouldRetrieveContentOfPassedDirs) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar"))).WillOnce(Return(DirContent{.files = {"foo/bar/f1", "foo/bar/f2"}}));
    EXPECT_CALL(mockFS, readDir(fs::path("baz/qux"))).WillOnce(Return(DirContent{.files = {"baz/qux/f3"}}));

    const vector<fs::path> files = fsAdapter.scanDirs({"foo/bar", "baz/qux"}, {}, 0);

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar/f1", "foo/bar/f2", "baz/qux/f3"}));
}

TEST(FSAdapter_scanDirs, ShouldNotIncludeFilesOfIgnoredDirectories) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar"))).WillOnce(Return(DirContent{.files = {"foo/bar/f1", "foo/bar/f2"}}));
    EXPECT_CALL(mockFS, readDir(fs::path("baz/qux"))).Times(Exactly(0));

    const vector<fs::path> files = fsAdapter.scanDirs({"foo/bar", "baz/qux"}, {"qux"}, 0);

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar/f1", "foo/bar/f2"}));
}

TEST(FSAdapter_scanDirs, ShouldNotScanSubDirsWithScanLevel0) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar"))).WillOnce(Return(DirContent{
        .files = {"foo/bar/f1", "foo/bar/f2"},
        .dirs = {"foo/bar/baz"}
    }));
    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar/baz"))).Times(Exactly(0));

    fsAdapter.scanDirs({"foo/bar"}, {}, 0);
}

TEST(FSAdapter_scanDirs, ShouldNotScanSubDirsWithScanLevel1) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar"))).WillOnce(Return(DirContent{
        .files = {"foo/bar/f1", "foo/bar/f2"},
        .dirs = {"foo/bar/baz"}
    }));
    EXPECT_CALL(mockFS, readDir(fs::path("foo/bar/baz"))).WillOnce(Return(DirContent{
        .files = {"foo/bar/baz/f3"}
    }));

    const vector<fs::path> files = fsAdapter.scanDirs({"foo/bar"}, {}, 1);

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar/f1", "foo/bar/f2", "foo/bar/baz/f3"}));
}

TEST(FSAdapter_filterFilesByNames, ShouldReturnAsISIfNoNamesPassed) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    const vector<fs::path> files = fsAdapter.filterFilesByNames({"foo/bar", "bar/baz", "baz/qux"}, {});

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar", "bar/baz", "baz/qux"}));
}

TEST(FSAdapter_filterFilesByNames, ShouldFilterByFileNames) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    const vector<fs::path> files = fsAdapter.filterFilesByNames({"foo/bar", "bar/baz", "baz/qux"}, {"bar", "baz"});

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar", "bar/baz"}));
}

TEST(FSAdapter_filterFilesByNames, ShouldFilterCaseInsensitively) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    const vector<fs::path> files = fsAdapter.filterFilesByNames({"foo/bar", "bar/bAz", "baz/qux"}, {"bAr", "baz"});

    EXPECT_EQ(files, (vector<fs::path>{"foo/bar", "bar/bAz"}));
}

TEST(FSAdapter_filterFilesByMinSize, ShouldFilter) {
    MockFS mockFS;
    FSAdapter fsAdapter = FSAdapter(&mockFS);

    EXPECT_CALL(mockFS, fileSize(fs::path("foo/bar"))).WillOnce(Return(100));
    EXPECT_CALL(mockFS, fileSize(fs::path("bar/baz"))).WillOnce(Return(500));
    EXPECT_CALL(mockFS, fileSize(fs::path("baz/qux"))).WillOnce(Return(100500));

    const vector<fs::path> files = fsAdapter.filterFilesByMinSize({"foo/bar", "bar/baz", "baz/qux"}, 500);

    EXPECT_EQ(files, (vector<fs::path>{"bar/baz", "baz/qux"}));
}
