#pragma once

#include <fstream>
#include "file_reader_interface.h"

class FileReader : public IFileReader {
public:
    FileReader(const std::filesystem::path& path, unsigned long chunkSize);

    std::string getChunk() override;
    bool eof() const override;
    std::filesystem::path path() const override;

private:
    std::filesystem::path path_;
    std::ifstream fileStream_;
    unsigned long chunkSize_;
};
