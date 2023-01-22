#pragma once

#include <vector>
#include <string>
#include <memory>
#include <filesystem>

#include "opts.h"
#include "file_reader_interface.h"

struct OpenOpts {
    HashType hashType;
    unsigned long chunkSize;
};

class IFSAdapter {
public:
    virtual ~IFSAdapter() = default;

    virtual std::vector<std::filesystem::path> scanDirs(const std::vector<std::filesystem::path>& dirs, const std::vector<std::string>& excludeDirs, unsigned int scanLevel) = 0;
    virtual std::vector<std::filesystem::path> filterFilesByNames(const std::vector<std::filesystem::path>& files, const std::vector<std::string>& names) = 0;
    virtual std::vector<std::filesystem::path> filterFilesByMinSize(const std::vector<std::filesystem::path>& files, unsigned long minSize) = 0;
    virtual std::unique_ptr<IFileReader> openFile(const std::filesystem::path& path, const OpenOpts& opts) = 0;
};
