#include "list.h"
#include "bulk_allocator.h"
#include "memory_mgr_mock.h"

#include <gtest/gtest.h>

using namespace ::testing;

class ListWithBulkAllocatorTest : public Test {
protected:
    void SetUp() override {
        MemoryMgrMock::updateInstance();
        MemoryMgrMock::getInstance().SetDefaultBehaviour();
    }

    void TearDown() override {
        MemoryMgrMock::dropInstance();
    }
};

TEST(List, Constructor_ShouldCreateEmptyList) {
    const List<int> list;

    EXPECT_EQ(list.begin(), list.end());
}

TEST(List, PushBack_ShouldAddOneElement) {
    List<int> list;

    list.push_back(100500);

    EXPECT_EQ(*list.begin(), 100500);
}

TEST(List, PushBack_ShouldAddTwoElements) {
    List<int> list;

    list.push_back(100500);
    list.push_back(500100);

    EXPECT_EQ(*list.begin(), 100500);
    EXPECT_EQ(*(++list.begin()), 500100);
}

TEST_F(ListWithBulkAllocatorTest, PushBack_ShouldAllocateOnce) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Allocate).Times(Exactly(1));

    List<int, BulkAllocator<int, 2>> list;

    list.push_back(100500);
    list.push_back(500100);

    EXPECT_EQ(*list.begin(), 100500);
    EXPECT_EQ(*(++list.begin()), 500100);
}

TEST_F(ListWithBulkAllocatorTest, Clear_ShouldDeallocateOnce) {
    EXPECT_CALL(MemoryMgrMock::getInstance(), Deallocate).Times(Exactly(1));

    List<int, BulkAllocator<int, 2>> list;

    list.push_back(100500);
    list.push_back(500100);

    list.clear();

    Mock::VerifyAndClearExpectations(&MemoryMgrMock::getInstance());
}
