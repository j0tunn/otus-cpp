#pragma once

#include <vector>

using IPAddress = std::vector<uint8_t>;

IPAddress parseIPAddress(const std::string& str);
std::string toString(const IPAddress& ipAddress);
