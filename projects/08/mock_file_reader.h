#pragma once

#include <gmock/gmock.h>

#include "file_reader_interface.h"

class MockFileReader : public IFileReader {
public:
    MOCK_METHOD(std::string, getChunk, (), (override));
    MOCK_METHOD(bool, eof, (), (const, override));
    MOCK_METHOD(std::filesystem::path, path, (), (const, override));
};
