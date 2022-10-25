#pragma once

#include "memory_mgr.h"
#include <gmock/gmock.h>
#include <memory>

class MemoryMgrMock {
public:
    static ::testing::NiceMock<MemoryMgrMock>& getInstance() {
        return *instance_.get();
    }

    static void updateInstance() {
        instance_ = std::make_unique<::testing::NiceMock<MemoryMgrMock>>();
    }

    static void dropInstance() {
        instance_ = nullptr;
    }

    MOCK_METHOD(void*, Allocate, (size_t size), (const));
    MOCK_METHOD(void, Deallocate, (void* ptr), (const));

    void SetDefaultBehaviour() {
        ON_CALL(*this, Allocate).WillByDefault([this](size_t size) {
            return std::malloc(size);
        });

        ON_CALL(*this, Deallocate).WillByDefault([this](void* ptr) {
            std::free(ptr);
        });
    }

private:
    MemoryMgrMock() {}
    MemoryMgrMock(const MemoryMgrMock&);
    MemoryMgrMock& operator=(MemoryMgrMock&);

    static std::unique_ptr<::testing::NiceMock<MemoryMgrMock>> instance_;
    friend ::testing::NiceMock<MemoryMgrMock>;
};
