#pragma once

#include <string>
#include <filesystem>

class IFileReader {
public:
    virtual ~IFileReader() = default;

    virtual std::string getChunk() = 0;
    virtual bool eof() const = 0;
    virtual std::filesystem::path path() const = 0;
};
