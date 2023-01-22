#include "fs.h"

using namespace std;
namespace fs = filesystem;

DirContent FS::readDir(const fs::path& path) {
    DirContent dirContent;

    for (auto const& dirEntry : fs::directory_iterator{path}) {
        if (dirEntry.is_directory()) {
            dirContent.dirs.push_back(dirEntry.path());
        } else if (dirEntry.is_regular_file()) {
            dirContent.files.push_back(dirEntry.path());
        }
    }

    return dirContent;
};

uintmax_t FS::fileSize(const fs::path& path) {
    return fs::file_size(path);
}
