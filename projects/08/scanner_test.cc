#include <gtest/gtest.h>
#include <vector>
#include "scanner.h"
#include "opts.h"
#include "mock_fs_adapter.h"
#include "mock_file_reader.h"

using namespace std;
using namespace testing;

namespace fs = filesystem;

bool operator==(const OpenOpts& lhs, const OpenOpts& rhs) {
    return lhs.hashType == rhs.hashType
        && lhs.chunkSize == rhs.chunkSize;
}

unique_ptr<MockFSAdapter> mkMockFSAdapter() {
    MockFSAdapter* pMockFSAdapter = new MockFSAdapter();

    EXPECT_CALL(*pMockFSAdapter, scanDirs).WillRepeatedly(Return(vector<fs::path>{"some/default/path"}));
    EXPECT_CALL(*pMockFSAdapter, filterFilesByNames).WillRepeatedly(ReturnArg<0>());
    EXPECT_CALL(*pMockFSAdapter, filterFilesByMinSize).WillRepeatedly(ReturnArg<0>());

    return unique_ptr<MockFSAdapter>(pMockFSAdapter);
}

unique_ptr<MockFileReader> mkMockFileReader(fs::path filePath = "default/path", vector<string> chunks = {"default-chunk"}) {
    MockFileReader* pMockReader = new MockFileReader();

    vector<bool> eofReturnValues(chunks.size(), false);
    eofReturnValues.push_back(true);

    EXPECT_CALL(*pMockReader, eof).WillRepeatedly(ReturnRoundRobin(eofReturnValues));
    EXPECT_CALL(*pMockReader, getChunk).WillRepeatedly(ReturnRoundRobin(chunks));
    EXPECT_CALL(*pMockReader, path).WillRepeatedly(Return(filePath));

    return unique_ptr<MockFileReader>(pMockReader);
}

void mockFile(const fs::path& filePath, const vector<string>& chunks, MockFSAdapter& fsAdapter) {
    unique_ptr<IFileReader> pReader = mkMockFileReader(filePath, chunks);
    EXPECT_CALL(fsAdapter, openFile(filePath, _)).WillOnce(Return(move(pReader)));
}

TEST(Scanner_scan, ShouldScanForFilesWithPassedOpts) {
    NiceMock<MockFSAdapter> fsAdapter;

    EXPECT_CALL(fsAdapter, scanDirs(vector<fs::path>{"foo", "bar"}, vector<string>{"baz", "qux"}, 100500));

    Scanner(&fsAdapter)
        .scan(Opts{
            .dirs = {"foo", "bar"},
            .excludeDirs = {"baz", "qux"},
            .scanLevel = 100500
        });
}

TEST(Scanner_scan, ShouldFilterFilesByNames) {
    auto pFSAdapter = mkMockFSAdapter();

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo", "bar"}));
    EXPECT_CALL(*pFSAdapter, filterFilesByNames(vector<fs::path>{"foo", "bar"}, vector<string>{"baz", "qux"}));

    Scanner(pFSAdapter.get())
        .scan(Opts{
            .files = {"baz", "qux"}
        });
}

TEST(Scanner_scan, ShouldFilterFilesByMinSize) {
    auto pFSAdapter = mkMockFSAdapter();

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo", "bar"}));
    EXPECT_CALL(*pFSAdapter, filterFilesByMinSize(vector<fs::path>{"foo", "bar"}, 100500));

    Scanner(pFSAdapter.get())
        .scan(Opts{
            .minSize = 100500
        });
}

TEST(Scanner_scan, ShouldNotOpenSingleFile) {
    auto pFSAdapter = mkMockFSAdapter();

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo"}));

    EXPECT_CALL(*pFSAdapter, openFile).Times(Exactly(0));

    Scanner(pFSAdapter.get())
        .scan({});
}

TEST(Scanner_scan, ShouldOpenFilteredFiles) {
    auto pFSAdapter = mkMockFSAdapter();

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo", "bar"}));

    EXPECT_CALL(*pFSAdapter, openFile(fs::path("foo"), OpenOpts{HashType::crc32, 100500})).WillOnce(Return(mkMockFileReader()));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("bar"), OpenOpts{HashType::crc32, 100500})).WillOnce(Return(mkMockFileReader()));

    Scanner(pFSAdapter.get())
        .scan(Opts{
            .chunkSize = 100500,
            .hashType = HashType::crc32
        });
}

TEST(Scanner_scan, ShouldCallDuplicatesCallbackForTheSameFiles) {
    vector<fs::path> sameFiles;
    auto pFSAdapter = mkMockFSAdapter();

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo", "bar", "baz", "qux"}));

    EXPECT_CALL(*pFSAdapter, openFile(fs::path("foo"), _)).WillOnce(Return(mkMockFileReader("foo", {"foo", "bar"})));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("bar"), _)).WillOnce(Return(mkMockFileReader("bar", {"foo", "bar"})));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("baz"), _)).WillOnce(Return(mkMockFileReader("baz", {"foo", "baz"})));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("qux"), _)).WillOnce(Return(mkMockFileReader("qux", {"qux"})));

    Scanner(pFSAdapter.get())
        .onDuplicates([&sameFiles](const vector<fs::path>& duplicates) {
            sameFiles = duplicates;
        })
        .scan({});

    EXPECT_EQ(sameFiles, (vector<fs::path>{"foo", "bar"}));
}

TEST(Scanner_scan, ShouldNotGetNextChunkIfPreviousAreDifferent) {
    auto pFSAdapter = mkMockFSAdapter();
    auto pFooReaderMock = mkMockFileReader();
    auto pBarReaderMock = mkMockFileReader();

    EXPECT_CALL(*pFooReaderMock, eof).WillOnce(Return(false));
    EXPECT_CALL(*pBarReaderMock, eof).WillOnce(Return(false));
    EXPECT_CALL(*pFooReaderMock, getChunk).WillOnce(Return("foo"));
    EXPECT_CALL(*pBarReaderMock, getChunk).WillOnce(Return("bar"));

    EXPECT_CALL(*pFSAdapter, scanDirs).WillOnce(Return(vector<fs::path>{"foo", "bar"}));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("foo"), _)).WillOnce(Return(move(pFooReaderMock)));
    EXPECT_CALL(*pFSAdapter, openFile(fs::path("bar"), _)).WillOnce(Return(move(pBarReaderMock)));

    Scanner(pFSAdapter.get())
        .scan({});
}
