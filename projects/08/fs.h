#pragma once

#include "fs_interface.h"

class FS : public IFS {
public:
    DirContent readDir(const std::filesystem::path& path) override;
    uintmax_t fileSize(const std::filesystem::path& path) override;
};
