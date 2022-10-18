#include "utils.h"

#include <gtest/gtest.h>

TEST(parseIPAddress, ValidAddress) {
  EXPECT_EQ(parseIPAddress("0.0.0.0"), IPAddress({0, 0, 0, 0}));
  EXPECT_EQ(parseIPAddress("127.0.0.1"), IPAddress({127, 0, 0, 1}));
  EXPECT_EQ(parseIPAddress("255.255.255.255"), IPAddress({255, 255, 255, 255}));

  EXPECT_EQ(parseIPAddress("127.0.0.1 "), IPAddress({127, 0, 0, 1}));
  EXPECT_EQ(parseIPAddress("127.0.0.1\t"), IPAddress({127, 0, 0, 1}));
  EXPECT_EQ(parseIPAddress("127.0.0.1 foo bar"), IPAddress({127, 0, 0, 1}));
  EXPECT_EQ(parseIPAddress("127.0.0.1\tfoo\tbar"), IPAddress({127, 0, 0, 1}));
}

TEST(parseIPAddress, InvalidAddress) {
  EXPECT_THROW(parseIPAddress("fooBar"), std::logic_error);
  EXPECT_THROW(parseIPAddress("foo.bar.baz.qux"), std::logic_error);

  EXPECT_THROW(parseIPAddress("0x0x0x0"), std::logic_error);

  EXPECT_THROW(parseIPAddress("-1.0.0.0"), std::logic_error);
  EXPECT_THROW(parseIPAddress("1111.0.0.0"), std::logic_error);
  EXPECT_THROW(parseIPAddress("0.0.0.1111"), std::logic_error);

  EXPECT_THROW(parseIPAddress("0.0.0.0.0"), std::logic_error);
  EXPECT_THROW(parseIPAddress("0.0.0"), std::logic_error);

  EXPECT_THROW(parseIPAddress("0.0.0."), std::logic_error);
  EXPECT_THROW(parseIPAddress(".0.0.0"), std::logic_error);

  EXPECT_THROW(parseIPAddress("256.0.0.0"), std::logic_error);
}

TEST(toString, ValidAddress) {
  EXPECT_EQ(toString({127, 0, 0, 1}), "127.0.0.1");
}

TEST(toUint32, Simple) {
  EXPECT_EQ(toUInt32({0, 0, 0, 0}), 0);
  EXPECT_EQ(toUInt32({127, 0, 0, 1}), 0x7F000001);
  EXPECT_EQ(toUInt32({255, 255, 255, 255}), 0xFFFFFFFF);
}
