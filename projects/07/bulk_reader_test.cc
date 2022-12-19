#include <gtest/gtest.h>

#include "bulk_reader.h"
#include "mock_flush_observer.h"

using namespace std;
using namespace testing;

TEST(BulkReader_eof, shouldNotifyFlushObserver) {
    BulkReader bulkReader(100500);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    EXPECT_CALL(flushObserver, onFlush(A<const vector<Command>&>())).Times(Exactly(1));

    bulkReader.eof();
}

TEST(BulkReader_eof, shouldNotifyAllFlushObserversWithSameBulk) {
    BulkReader bulkReader(100500);

    NiceMock<MockFlushObserver> fooObserver, barObserver;
    bulkReader.addFlushObserver(&fooObserver);
    bulkReader.addFlushObserver(&barObserver);

    const vector<Command>* p1 = nullptr;
    const vector<Command>* p2 = nullptr;

    EXPECT_CALL(fooObserver, onFlush(_)).WillOnce(Invoke([&p1](const vector<Command>& bulk) { p1 = &bulk; }));
    EXPECT_CALL(barObserver, onFlush(_)).WillOnce(Invoke([&p2](const vector<Command>& bulk) { p2 = &bulk; }));

    bulkReader.eof();

    EXPECT_EQ(p1, p2);
}

TEST(BulkReader_eof, shouldDropBulkAfterEachFlush) {
    BulkReader bulkReader(100500);

    NiceMock<MockFlushObserver> fooObserver;
    bulkReader.addFlushObserver(&fooObserver);

    vector<unsigned int> bulksSize;

    EXPECT_CALL(fooObserver, onFlush(_)).WillRepeatedly(Invoke([&bulksSize](const vector<Command>& bulk) { bulksSize.push_back(bulk.size()); }));

    bulkReader.addCmd("foo");
    bulkReader.eof();
    bulkReader.eof();

    EXPECT_NE(bulksSize[0], bulksSize[1]);
    EXPECT_EQ(bulksSize[1], 0);
}

TEST(BulkReader_eof, shouldIgnoreOpenedDynamicBlock) {
    BulkReader bulkReader(100500);

    NiceMock<MockFlushObserver> fooObserver;
    bulkReader.addFlushObserver(&fooObserver);

    bulkReader.addCmd("foo");
    bulkReader.addCmd("{");

    EXPECT_CALL(fooObserver, onFlush(_)).Times(Exactly(0));

    bulkReader.addCmd("bar");
    bulkReader.eof();
}

TEST(BulkReader_addCmd, shouldAddCmdToBulk) {
    BulkReader bulkReader(100500);

    NiceMock<MockFlushObserver> flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    ON_CALL(flushObserver, onFlush(_)).WillByDefault(SaveArg<0>(&bulk));

    bulkReader.addCmd("foo");
    bulkReader.eof();

    ASSERT_EQ(bulk.size(), 1);
    EXPECT_EQ(bulk[0].val, "foo");
}

TEST(BulkReader_addCmd, shouldNotAutoFlushIfLimitHasNotBeenReached) {
    BulkReader bulkReader(2);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    EXPECT_CALL(flushObserver, onFlush(_)).Times(Exactly(0));

    bulkReader.addCmd("foo");
}

TEST(BulkReader_addCmd, shouldAutoFlushOnLimitExceeding) {
    BulkReader bulkReader(2);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillOnce(SaveArg<0>(&bulk));

    bulkReader.addCmd("foo");
    bulkReader.addCmd("bar");

    ASSERT_EQ(bulk.size(), 2);
    EXPECT_EQ(bulk[0].val, "foo");
    EXPECT_EQ(bulk[1].val, "bar");
}

TEST(BulkReader_addCmd, shouldFlushOnStartDynamicBlock) {
    BulkReader bulkReader(100500);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillOnce(SaveArg<0>(&bulk));

    bulkReader.addCmd("foo");
    bulkReader.addCmd("{");

    ASSERT_EQ(bulk.size(), 1);
    EXPECT_EQ(bulk[0].val, "foo");
}

TEST(BulkReader_addCmd, shouldFlushOnEndDynamicBlock) {
    BulkReader bulkReader(100500);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillRepeatedly(SaveArg<0>(&bulk));

    bulkReader.addCmd("foo");
    bulkReader.addCmd("{");
    bulkReader.addCmd("bar");
    bulkReader.addCmd("}");

    ASSERT_EQ(bulk.size(), 1);
    EXPECT_EQ(bulk[0].val, "bar");
}

TEST(BulkReader_addCmd, shouldIgnoreDynamicBlockEndWithoutStart) {
    BulkReader bulkReader(100500);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    EXPECT_CALL(flushObserver, onFlush(_)).Times(Exactly(0));

    bulkReader.addCmd("foo");
    bulkReader.addCmd("}");
}

TEST(BulkReader_addCmd, shouldIgnoreNestedDynamicBlockMarkup) {
    BulkReader bulkReader(100500);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillRepeatedly(SaveArg<0>(&bulk));

    bulkReader.addCmd("{");
    bulkReader.addCmd("foo");
    bulkReader.addCmd("{");
    bulkReader.addCmd("bar");
    bulkReader.addCmd("}");
    bulkReader.addCmd("}");

    ASSERT_EQ(bulk.size(), 2);
    EXPECT_EQ(bulk[0].val, "foo");
    EXPECT_EQ(bulk[1].val, "bar");
}

TEST(BulkReader_addCmd, shouldIgnoreBulkSizeWhileProcessingDynamicBlock) {
    BulkReader bulkReader(2);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillRepeatedly(SaveArg<0>(&bulk));

    bulkReader.addCmd("{");
    bulkReader.addCmd("foo");
    bulkReader.addCmd("bar");
    bulkReader.addCmd("baz");
    bulkReader.addCmd("}");

    ASSERT_EQ(bulk.size(), 3);
    EXPECT_EQ(bulk[0].val, "foo");
    EXPECT_EQ(bulk[1].val, "bar");
    EXPECT_EQ(bulk[2].val, "baz");
}

TEST(BulkReader_addCmd, shouldReturnToAutoFlushAfterDynamicBlock) {
    BulkReader bulkReader(2);

    MockFlushObserver flushObserver;
    bulkReader.addFlushObserver(&flushObserver);

    vector<Command> bulk;

    EXPECT_CALL(flushObserver, onFlush(_)).WillRepeatedly(SaveArg<0>(&bulk));

    bulkReader.addCmd("{");
    bulkReader.addCmd("foo");
    bulkReader.addCmd("}");
    bulkReader.addCmd("bar");
    bulkReader.addCmd("baz");

    ASSERT_EQ(bulk.size(), 2);
    EXPECT_EQ(bulk[0].val, "bar");
    EXPECT_EQ(bulk[1].val, "baz");
}
