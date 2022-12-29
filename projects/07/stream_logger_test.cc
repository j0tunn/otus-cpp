#include <gtest/gtest.h>
#include <sstream>

#include "stream_logger.h"

using namespace std;

TEST(StreamLogger_onFlush, shouldPrintAllCommands) {
    stringstream out;
    StreamLogger logger(out);

    logger.onFlush({
        Command("foo"), Command("bar")
    });

    EXPECT_EQ(out.str(), "bulk: foo, bar\n");
}
