#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "file_reader_interface.h"

struct Opts;
class IFSAdapter;

using DuplicatesCallback = std::function<void(const std::vector<std::filesystem::path>&)>;

class Scanner {
public:
    Scanner(IFSAdapter* pFsAdapter);
    ~Scanner() = default;

    Scanner& onDuplicates(DuplicatesCallback cb);

    void scan(const Opts& opts);

private:
    void checkForDuplicates_(std::vector<std::unique_ptr<IFileReader> >& files);

    DuplicatesCallback duplicatesCb_;
    IFSAdapter* pFsAdapter_;
};
