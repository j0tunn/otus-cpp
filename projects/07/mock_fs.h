#pragma once

#include <gmock/gmock.h>
#include <memory>
#include <filesystem>

class MockFS {
public:
    static ::testing::NiceMock<MockFS>& getInstance();
    static void updateInstance();
    static void dropInstance();

    MOCK_METHOD(void, writeFile, (const std::filesystem::path&, const std::string&));

private:
    MockFS() {}
    MockFS(const MockFS&);
    MockFS& operator=(MockFS&);

    static std::unique_ptr<::testing::NiceMock<MockFS>> instance_;
    friend ::testing::NiceMock<MockFS>;
};
