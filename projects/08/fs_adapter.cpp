#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "fs_adapter.h"
#include "fs_interface.h"
#include "file_reader.h"
#include "file_reader_decorator.h"
#include "hash.h"

using namespace std;
namespace fs = filesystem;

FSAdapter::FSAdapter(IFS* pFS)
    : pFS_(pFS)
{}

vector<fs::path> FSAdapter::scanDirs(const vector<fs::path>& dirs, const vector<string>& excludeDirs, unsigned int scanLevel) {
    vector<fs::path> files;

    for(const fs::path& dir : dirs) {
        if (excludeDirs.end() != find(excludeDirs.begin(), excludeDirs.end(), dir.filename())) {
            continue;
        }

        const DirContent dirContent = pFS_->readDir(dir);
        files.insert(files.end(), dirContent.files.begin(), dirContent.files.end());

        if (scanLevel > 0 && dirContent.dirs.size() > 0) {
            const vector<fs::path> subFiles = scanDirs(dirContent.dirs, excludeDirs, scanLevel - 1);
            files.insert(files.end(), subFiles.begin(), subFiles.end());
        }
    };

    return files;
}

vector<fs::path> FSAdapter::filterFilesByNames(const vector<fs::path>& files, const vector<string>& names) {
    if (names.size() == 0) {
        return files;
    }

    vector<fs::path> filteredFiles;
    filteredFiles.reserve(files.size());

    copy_if(files.begin(), files.end(), back_inserter(filteredFiles), [&names](const fs::path& file) {
        const string filename = file.filename();

        return names.end() != find_if(names.begin(), names.end(), [&filename](const string& name) {
            return boost::iequals(filename, name);
        });
    });

    return filteredFiles;
}

vector<fs::path> FSAdapter::filterFilesByMinSize(const vector<fs::path>& files, unsigned long minSize) {
    vector<fs::path> filteredFiles;
    filteredFiles.reserve(files.size());

    copy_if(files.begin(), files.end(), back_inserter(filteredFiles), [this, minSize](const fs::path& file) {
        return pFS_->fileSize(file) >= minSize;
    });

    return filteredFiles;
}

unique_ptr<IFileReader> FSAdapter::openFile(const fs::path& path, const OpenOpts& opts) {
    unique_ptr<IFileReader> pReader(new FileReader(path, opts.chunkSize));

    return unique_ptr<IFileReader>(
        opts.hashType == HashType::crc32
            ? new FileReaderDecorator(move(pReader), ::hash::crc32)
            : new FileReaderDecorator(move(pReader), ::hash::md5)
    );
}
