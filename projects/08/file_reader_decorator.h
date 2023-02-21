#pragma once

#include <memory>
#include <functional>
#include "file_reader_interface.h"

using ChunkDecorator = std::function<std::string(const std::string&)>;

class FileReaderDecorator : public IFileReader {
public:
    FileReaderDecorator(std::unique_ptr<IFileReader>&& pFileReader, ChunkDecorator decorator)
        : pFileReader_(std::move(pFileReader))
        , decorator_(decorator)
    {}

    std::string getChunk() override {
        return decorator_(pFileReader_->getChunk());
    }

    bool eof() const override {
        return pFileReader_->eof();
    };

    std::filesystem::path path() const override {
        return pFileReader_->path();
    };

private:
    std::unique_ptr<IFileReader> pFileReader_;
    ChunkDecorator decorator_;
};
