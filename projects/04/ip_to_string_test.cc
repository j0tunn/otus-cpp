#include "ip_to_string.h"

#include <gtest/gtest.h>
#include <vector>
#include <list>

TEST(IPToString, Int8) {
    EXPECT_EQ(ipToString(int8_t{0}), "0");
    EXPECT_EQ(ipToString(int8_t{-1}), "255");
}

TEST(IPToString, Int16) {
    EXPECT_EQ(ipToString(int16_t{0}), "0.0");
    EXPECT_EQ(ipToString(int16_t{0x007F}), "0.127");
    EXPECT_EQ(ipToString(int16_t{0x7F00}), "127.0");
    EXPECT_EQ(ipToString(int16_t{-1}), "255.255");
}

TEST(IPToString, UInt16) {
    EXPECT_EQ(ipToString(uint16_t{0x00FF}), "0.255");
    EXPECT_EQ(ipToString(uint16_t{0xFF00}), "255.0");
}

TEST(IPToString, Int32) {
    EXPECT_EQ(ipToString(int32_t{0}), "0.0.0.0");
    EXPECT_EQ(ipToString(int32_t{0x7F000001}), "127.0.0.1");
    EXPECT_EQ(ipToString(int32_t{-1}), "255.255.255.255");
}

TEST(IPToString, Int64) {
    EXPECT_EQ(ipToString(int64_t{0}), "0.0.0.0.0.0.0.0");
    EXPECT_EQ(ipToString(int64_t{0x7F00000000000001}), "127.0.0.0.0.0.0.1");
    EXPECT_EQ(ipToString(int64_t{-1}), "255.255.255.255.255.255.255.255");
}

TEST(IPToString, String) {
    EXPECT_EQ(ipToString(std::string{"127.0.0.1"}), "127.0.0.1");
}

TEST(IPToString, Vector) {
    EXPECT_EQ(ipToString(std::vector<int>{127, 0, 0, 1}), "127.0.0.1");
    EXPECT_EQ(ipToString(std::vector<int>{}), "");
}

TEST(IPToString, List) {
    EXPECT_EQ(ipToString(std::list<short>{127, 0, 0, 1}), "127.0.0.1");
    EXPECT_EQ(ipToString(std::list<short>{}), "");
}

TEST(IPToString, Tuple) {
    EXPECT_EQ(ipToString(std::make_tuple(127, 0, 0, 1)), "127.0.0.1");
    EXPECT_EQ(ipToString(std::make_tuple(127, 0, 0, 0, 1)), "127.0.0.0.1");
    EXPECT_EQ(ipToString(std::make_tuple()), "");
}
