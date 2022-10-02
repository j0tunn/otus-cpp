#include "lib.h"
#include "version.h"

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  EXPECT_EQ(version(), PROJECT_VERSION_PATCH);
}
