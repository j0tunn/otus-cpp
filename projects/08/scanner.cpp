#include <map>
#include <algorithm>
#include "scanner.h"
#include "opts.h"
#include "fs_adapter_interface.h"

using namespace std;

Scanner::Scanner(IFSAdapter* pFsAdapter)
    : pFsAdapter_(pFsAdapter)
    , duplicatesCb_([](const auto&) {})
{}

Scanner& Scanner::onDuplicates(DuplicatesCallback cb) {
    duplicatesCb_ = cb;

    return *this;
}

template<typename T, typename U>
vector<U> convert(const vector<T>& values, std::function<U(const T&)> convertFn) {
    vector<U> res;
    res.reserve(values.size());

    transform(values.begin(), values.end(), back_inserter(res), convertFn);
    return res;
}

void Scanner::scan(const Opts& opts) {
    vector<filesystem::path> filePaths = pFsAdapter_->scanDirs(opts.dirs, opts.excludeDirs, opts.scanLevel);
    filePaths = pFsAdapter_->filterFilesByNames(filePaths, opts.files);
    filePaths = pFsAdapter_->filterFilesByMinSize(filePaths, opts.minSize);

    if (filePaths.size() < 2) {
        return;
    }

    vector<unique_ptr<IFileReader> > files = convert<filesystem::path, unique_ptr<IFileReader> >(filePaths, [this, &opts](const filesystem::path& path) {
        return pFsAdapter_->openFile(path, {
            .hashType = opts.hashType,
            .chunkSize = opts.chunkSize
        });
    });

    checkForDuplicates_(files);
}

void Scanner::checkForDuplicates_(vector<unique_ptr<IFileReader> >& files) {
    map<string, vector<unique_ptr<IFileReader> > > filesByChunk;

    for(auto& pFile : files) {
        if (!pFile->eof()) {
            filesByChunk[pFile->getChunk()].push_back(move(pFile));
        }
    }

    if (filesByChunk.size() == 0) {
        vector<filesystem::path> paths = convert<unique_ptr<IFileReader>, filesystem::path>(files, [](const unique_ptr<IFileReader>& pFile) -> string {
            return pFile->path();
        });

        duplicatesCb_(paths);
        return;
    }

    for_each(filesByChunk.begin(), filesByChunk.end(), [this](auto& pair) {
        if (pair.second.size() > 1) {
            checkForDuplicates_(pair.second);
        }
    });
}
