#include "memory_mgr_mock.h"

std::unique_ptr<::testing::NiceMock<MemoryMgrMock>> MemoryMgrMock::instance_(nullptr);

namespace MemoryMgr {

void* allocate(std::size_t size) {
    return MemoryMgrMock::getInstance().Allocate(size);
}

void deallocate(void* ptr) {
    return MemoryMgrMock::getInstance().Deallocate(ptr);
}

}
