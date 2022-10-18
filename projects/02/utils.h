#pragma once

#include <array>
#include <cstdint>
#include <string>

using IPAddress = std::array<uint8_t, 4>;

IPAddress parseIPAddress(const std::string& str);
std::string toString(const IPAddress& ipAddress);
uint32_t toUInt32(const IPAddress& ipAddress);
