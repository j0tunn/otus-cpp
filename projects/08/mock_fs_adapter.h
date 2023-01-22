#pragma once

#include <gmock/gmock.h>

#include "fs_adapter_interface.h"

class MockFSAdapter : public IFSAdapter {
public:
    MOCK_METHOD(std::vector<std::filesystem::path>, scanDirs, (const std::vector<std::filesystem::path>&, const std::vector<std::string>&, unsigned int), (override));
    MOCK_METHOD(std::vector<std::filesystem::path>, filterFilesByNames, (const std::vector<std::filesystem::path>&, const std::vector<std::string>&), (override));
    MOCK_METHOD(std::vector<std::filesystem::path>, filterFilesByMinSize, (const std::vector<std::filesystem::path>&, unsigned long), (override));
    MOCK_METHOD(std::unique_ptr<IFileReader>, openFile, (const std::filesystem::path&, const OpenOpts&), (override));
};
