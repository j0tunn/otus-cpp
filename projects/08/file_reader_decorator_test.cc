#include <gtest/gtest.h>
#include "file_reader_decorator.h"
#include "mock_file_reader.h"

using namespace std;
using namespace testing;

namespace fs = filesystem;

TEST(FileReaderDecorator_getChunk, ShouldReturnDecoratedChunk) {
    unique_ptr<MockFileReader> pMockFileReader(new MockFileReader());

    EXPECT_CALL(*pMockFileReader, getChunk).WillOnce(Return("foo"));

    FileReaderDecorator fileReaderDecorator(move(pMockFileReader), [](const string& str) {
        return str + "bar";
    });

    EXPECT_EQ(fileReaderDecorator.getChunk(), "foobar");
}

TEST(FileReaderDecorator_eof, ShouldReturnResultAsIs) {
    unique_ptr<MockFileReader> pMockFileReader(new MockFileReader());

    EXPECT_CALL(*pMockFileReader, eof).WillOnce(Return(true));

    FileReaderDecorator fileReaderDecorator(move(pMockFileReader), [](const string& str) {
        return str + "bar";
    });

    EXPECT_EQ(fileReaderDecorator.eof(), true);
}

TEST(FileReaderDecorator_path, ShouldReturnResultAsIs) {
    unique_ptr<MockFileReader> pMockFileReader(new MockFileReader());

    EXPECT_CALL(*pMockFileReader, path).WillOnce(Return(fs::path("baz/qux")));

    FileReaderDecorator fileReaderDecorator(move(pMockFileReader), [](const string& str) {
        return str + "bar";
    });

    EXPECT_EQ(fileReaderDecorator.path(), fs::path("baz/qux"));
}
