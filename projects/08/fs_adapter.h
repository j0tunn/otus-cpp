#pragma once

#include "fs_adapter_interface.h"

class IFS;

class FSAdapter : public IFSAdapter {
public:
    FSAdapter(IFS* pFS);

    std::vector<std::filesystem::path> scanDirs(const std::vector<std::filesystem::path>& dirs, const std::vector<std::string>& excludeDirs, unsigned int scanLevel) override;
    std::vector<std::filesystem::path> filterFilesByNames(const std::vector<std::filesystem::path>& files, const std::vector<std::string>& names) override;
    std::vector<std::filesystem::path> filterFilesByMinSize(const std::vector<std::filesystem::path>& files, unsigned long minSize) override;
    std::unique_ptr<IFileReader> openFile(const std::filesystem::path& path, const OpenOpts& opts) override;

private:
    IFS* pFS_;
};
