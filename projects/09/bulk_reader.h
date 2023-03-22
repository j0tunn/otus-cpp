#pragma once

#include <string>
#include <vector>
#include <memory>
#include "flush_observer.h"
#include "command.h"
#include "reader_state.h"

class BulkReader {
public:
    explicit BulkReader(unsigned int bulkSize);
    void addCmd(const std::string& cmd);
    void eof();

    void addFlushObserver(IFlushObserver* observer);

private:
    void notifyFlush_(const std::vector<Command>& bulk);
    void setNewState(std::unique_ptr<ReaderState>&& state);

    std::vector<IFlushObserver*> observers_;
    std::unique_ptr<ReaderState> state_;
};
