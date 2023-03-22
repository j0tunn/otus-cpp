#include "fs.h"

#include "mock_fs.h"

using namespace std;
using namespace testing;

void writeFile(const filesystem::path& path, const string& content) {
    MockFS::getInstance().writeFile(path, content);
}

unique_ptr<NiceMock<MockFS>> MockFS::instance_ = nullptr;

NiceMock<MockFS>& MockFS::getInstance() {
    return *instance_.get();
}

void MockFS::updateInstance() {
    instance_ = std::make_unique<::testing::NiceMock<MockFS>>();
}

void MockFS::dropInstance() {
    instance_ = nullptr;
}
