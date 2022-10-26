#include "bulk_allocator.h"
#include "memory_mgr_mock.h"

#include <gtest/gtest.h>
#include <map>

using namespace ::testing;
using namespace std;

using Node = pair<const int, int>;

template <unsigned int N>
using Map = map<int, int, less<int>, BulkAllocator<Node, N>>;

class BulkAllocatorTest : public Test {
protected:
    void SetUp() override {
        MemoryMgrMock::updateInstance();
        MemoryMgrMock::getInstance().SetDefaultBehaviour();
    }

    void TearDown() override {
        MemoryMgrMock::dropInstance();
    }
};

TEST_F(BulkAllocatorTest, OnCreateEmptyContainer_ShouldNotAllocate) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(0));

    Map<1> m{};
}

TEST_F(BulkAllocatorTest, OnCreateEmptyContainer_ShouldNotDeallocate) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Deallocate).Times(Exactly(0));

    Map<1> m{};
}

TEST_F(BulkAllocatorTest, OnCreateContainerWithElements_ShouldAllocateWholeMemoryOnce) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(1));

    Map<3> m{{1, 1}, {2, 2}, {3, 3}};
}

TEST_F(BulkAllocatorTest, OnCreateContainerWithTooMuchElements_ShouldAllocateNextBulk) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(2));

    Map<2> m{{1, 1}, {2, 2}, {3, 3}};
}

TEST_F(BulkAllocatorTest, OnDestructContainerWithElements_ShouldDeallocateWholeMemoryOnce) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Deallocate).Times(Exactly(1));

    Map<3> m{{1, 1}, {2, 2}, {3, 3}};
}

TEST_F(BulkAllocatorTest, OnDestructContainerWithElements_ShouldDeallocateSameMemoryThatWasAllocated) {
    ON_CALL(MemoryMgrMock::getInstance(), Allocate).WillByDefault([](size_t size) {
        void* p = std::malloc(size);

        EXPECT_CALL(MemoryMgrMock::getInstance(), Deallocate(p));

        return p;
    });

    Map<3> m{{1, 1}, {2, 2}, {3, 3}};
}

TEST_F(BulkAllocatorTest, OnAddElements_ShouldAllocateWholeMemoryOnce) {
    Map<3> m{};

    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(1));

    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
}

TEST_F(BulkAllocatorTest, OnAddElements_ShouldUseFreedMemoryAtTheEnd) {
    Map<3> m{};

    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(1));

    m[1] = 1;
    m[2] = 2;
    m[3] = 3;

    m.erase(3);
    m[4] = 4;
}

TEST_F(BulkAllocatorTest, OnAddElements_ShouldUseFreedMemoryInTheMiddle) {
    Map<3> m{};

    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(1));

    m[1] = 1;
    m[2] = 2;
    m[3] = 3;

    m.erase(2);
    m[4] = 4;
}

TEST_F(BulkAllocatorTest, OnRemoveAllElements_ShouldDeallocateWholeMemoryOnce) {
    Map<2> m{{1, 1}, {2, 2}};

    EXPECT_CALL(MemoryMgrMock::getInstance(), Deallocate).Times(Exactly(1));

    m.erase(1);
    m.erase(2);

    Mock::VerifyAndClearExpectations(&MemoryMgrMock::getInstance());
}

TEST_F(BulkAllocatorTest, CopyConstructor_ShuldInitiateContainerCopyWithClearAllocator) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(2));

    Map<2> m{{1, 1}, {2, 2}};
    auto m2 = m;
}
