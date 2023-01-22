#pragma once

#include <string>

namespace hash {
    std::string crc32(const std::string& str);
    std::string md5(const std::string& str);
}
