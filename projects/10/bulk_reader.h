#pragma once

#include <string>
#include <vector>
#include <memory>
#include "flush_observer.h"
#include "command.h"
#include "reader_state.h"

class BulkReader : public FlushObservable {
public:
    explicit BulkReader(unsigned int bulkSize);
    void addCmd(const std::string& cmd);
    void eof();
    bool isInStaticMode() const;

private:
    void setNewState(std::unique_ptr<ReaderState>&& state);

    std::unique_ptr<ReaderState> state_;
};
