#include "file_reader.h"

using namespace std;
namespace fs = filesystem;

FileReader::FileReader(const fs::path& path, unsigned long chunkSize)
    : path_(path)
    , fileStream_(ifstream(path.c_str()))
    , chunkSize_(chunkSize)
{}

string FileReader::getChunk() {
    string chunk = string(chunkSize_, '\0');
    fileStream_.read(&chunk[0], chunkSize_);

    return chunk;
}

bool FileReader::eof() const {
    return fileStream_.eof();
};

fs::path FileReader::path() const {
    return path_;
}
