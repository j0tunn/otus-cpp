#pragma once

#include <string>
#include <map>
#include <memory>
#include "flush_observer.h"
#include "bulk_reader.h"

class BulkRouter : public FlushObservable, public IFlushObserver {
public:
    explicit BulkRouter(unsigned int bulkSize);
    void addCmd(const std::string& cmd, unsigned long sessionId);
    void eof();

    void onFlush(const std::vector<Command>& bulk) override;

private:
    void addStaticCmd(const std::string& cmd, unsigned long sessionId);
    bool addDynamicCmd(const std::string& cmd, unsigned long sessionId);

    unsigned int bulkSize_;
    std::unique_ptr<BulkReader> pStaticBulkReader_;
    std::map<unsigned long, std::unique_ptr<BulkReader> > dynamicBulkReaders_;
};
