#include "utils.h"
#include <regex>
#include <stdexcept>
#include <algorithm>
#include  <sstream>

using namespace std;

IPAddress parseIPAddress(const string& str) {
    const regex re("^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})(?:\\s.*)?$");
    smatch match;

    if (!regex_match(str, match, re)) {
        throw logic_error("Not an IP address string: " + str);
    }

    IPAddress result;
    transform(++match.begin(), match.end(), back_inserter(result), [&str](const string& piece) {
        const auto byte = stoul(piece);
        if (byte > 255) {
            throw logic_error("Not an IP address string: " + str);
        }

        return byte;
    });

    return result;
};

string toString(const IPAddress& ip) {
    stringstream s;

    s << to_string(ip[0]) << "." << to_string(ip[1]) << "." << to_string(ip[2]) << "." << to_string(ip[3]);

    return s.str();
}
