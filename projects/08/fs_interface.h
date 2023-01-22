#pragma once

#include <vector>
#include <filesystem>

struct DirContent {
    std::vector<std::filesystem::path> files;
    std::vector<std::filesystem::path> dirs;
};

class IFS {
public:
    virtual ~IFS() = default;

    virtual DirContent readDir(const std::filesystem::path& path) = 0;
    virtual uintmax_t fileSize(const std::filesystem::path& path) = 0;
};
