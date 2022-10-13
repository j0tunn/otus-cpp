#pragma once

#include <vector>
#include <cstdint>
#include <string>

using IPAddress = std::vector<uint8_t>;

IPAddress parseIPAddress(const std::string& str);
std::string toString(const IPAddress& ipAddress);
