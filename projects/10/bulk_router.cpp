#include "bulk_router.h"
#include <utility>

using namespace std;

BulkRouter::BulkRouter(unsigned int bulkSize)
    : bulkSize_(bulkSize)
    , pStaticBulkReader_(nullptr)
{
}

void BulkRouter::addCmd(const string& cmd, unsigned long sessionId) {
    if (!addDynamicCmd(cmd, sessionId)) {
        addStaticCmd(cmd, sessionId);
    }
}

bool BulkRouter::addDynamicCmd(const string& cmd, unsigned long sessionId) {
    auto itReader = dynamicBulkReaders_.find(sessionId);
    if (itReader == dynamicBulkReaders_.end()) {
        return false;
    }

    auto& pReader = itReader->second;
    pReader->addCmd(cmd);

    if (pReader->isInStaticMode()) {
        dynamicBulkReaders_.erase(itReader);
    }

    return true;
}

void BulkRouter::addStaticCmd(const string& cmd, unsigned long sessionId) {
    if (!pStaticBulkReader_) {
        pStaticBulkReader_ = make_unique<BulkReader>(bulkSize_);
        pStaticBulkReader_->addFlushObserver(this);
    }

    pStaticBulkReader_->addCmd(cmd);

    if (!pStaticBulkReader_->isInStaticMode()) {
        dynamicBulkReaders_.emplace(make_pair(sessionId, move(pStaticBulkReader_)));
    }
}

void BulkRouter::eof() {
    if (pStaticBulkReader_) {
        pStaticBulkReader_->eof();
    }

    for (auto& pair : dynamicBulkReaders_) {
        pair.second->eof();
    }
}

void BulkRouter::onFlush(const vector<Command>& bulk) {
    notifyFlush_(bulk);
};
