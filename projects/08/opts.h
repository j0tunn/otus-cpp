#pragma once

#include <vector>
#include <string>
#include <filesystem>

enum HashType {
    crc32,
    md5
};

struct Opts {
    std::vector<std::filesystem::path> dirs;
    std::vector<std::string> excludeDirs;
    std::vector<std::string> files;
    unsigned int scanLevel;
    unsigned long minSize;
    unsigned long chunkSize;
    HashType hashType;
};
