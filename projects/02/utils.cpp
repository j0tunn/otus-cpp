#include "utils.h"
#include <regex>
#include <stdexcept>
#include <algorithm>
#include <sstream>

using namespace std;

IPAddress parseIPAddress(const string& str) {
    const regex re("^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})(?:\\s.*)?$");
    smatch match;

    if (!regex_match(str, match, re)) {
        throw logic_error("Not an IP address string");
    }

    const auto getByte = [](const string& piece) -> uint8_t {
        const auto byte = stoul(piece);
        if (byte > 255) {
            throw logic_error("Not an IP address string");
        }

        return byte;
    };

    return {
        getByte(match[1]),
        getByte(match[2]),
        getByte(match[3]),
        getByte(match[4])
    };
};

string toString(const IPAddress& ip) {
    stringstream s;

    s << to_string(ip[0]) << "." << to_string(ip[1]) << "." << to_string(ip[2]) << "." << to_string(ip[3]);

    return s.str();
}

uint32_t toUInt32(const IPAddress& ip) {
    return (ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3];
}
