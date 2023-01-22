#pragma once

#include <gmock/gmock.h>

#include "fs_interface.h"

class MockFS : public IFS {
public:
    MOCK_METHOD(DirContent, readDir, (const std::filesystem::path&), (override));
    MOCK_METHOD(uintmax_t, fileSize, (const std::filesystem::path&), (override));
};
